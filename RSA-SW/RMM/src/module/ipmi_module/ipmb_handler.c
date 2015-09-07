/**
 * Copyright (c)  2015, Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "libutils/list.h"
#include "ipmi.h"
#include "ipmi_log.h"
#include "rmcp.h"

#define IPMB_REQ_MIN_LEN		7
#define IPMB_RSP_MIN_LEN		8

#define IPMB_RSPHNDL_QUEUE_SIZE	256

/**
  *  @brief IPMB: 2.6.1 The Seq Field and Retries:
  *  The Requester should check the following fields (after verifying data
  *  integrity using the checksums) to match a response to a given request:
  *
  *          x rsSA
  *          x rsLUN
  *          x Seq
  *          x netFn
  *          x cmd
  */
struct ipmb_match_info {
	unsigned char sa;
	unsigned char lun;
	unsigned char seq;
	unsigned char netfn;
	unsigned char cmd;
};

struct rsp_ipmb_hndl {
	struct rsp_ipmb_hndl *next;	/** free buffer link */


	struct list_head seq_list;	/** link in 'seq_table[i]' list */
	struct list_head list;		/** all ipmb response handle in 'all_hndls' list */

	struct ipmb_match_info match;
	unsigned int           msgid;
	unsigned short         user_port;
	ipmi_json_ipc_header_t header;
#ifdef DEBUG_IPMI
	struct timespec start_time;
#endif
	unsigned int   timeout;
	int            broadcast;
};

struct rsp_ipmb_table {
	pthread_mutex_t  lock;

	struct rsp_ipmb_hndl *freelist;

	unsigned int     curr_seq;
	struct list_head all_hndls;
	struct list_head seq_table[IPMB_SEQ_SIZE];	/** Fast indexed handle by seq */

	pthread_t time_tid;
};

static struct rsp_ipmb_table ipmb_rsp_table;

/**
  *  @brief IPMB timeout monitor thread
  *
  *  @param[in] unused
  *  @return
  */
static void *ipmb_time_thread(void *unused)
{
	struct timespec expire;
	struct rsp_ipmb_hndl *hndl, *nxt;
	struct rsp_ipmb_table *table = &ipmb_rsp_table;
	pthread_cond_t nul_cond = PTHREAD_COND_INITIALIZER;

	prctl(PR_SET_NAME, "ipmb_time_thread");

	for (; ipmi_module_exit == false;) {
		clock_gettime(CLOCK_REALTIME, &expire);
		expire.tv_sec += IPMI_PERIOD_TIME_MS;
		expire.tv_nsec += (IPMI_PERIOD_TIME_MS % 1000) * (1000*1000);
		if (expire.tv_nsec >= BILLION) {
			expire.tv_sec += expire.tv_nsec / BILLION;
			expire.tv_nsec %= BILLION;
		}

		pthread_mutex_lock(&table->lock);
		while (pthread_cond_timedwait(&nul_cond, &table->lock, &expire) != ETIMEDOUT)
			/* Nothing! */;

#ifdef DEBUG_IPMI
		clock_gettime(CLOCK_REALTIME, &expire);
#endif
		list_for_each_entry_safe(hndl, nxt, &table->all_hndls, list) {
			hndl->timeout -= IPMI_PERIOD_TIME_MS;
			if (hndl->timeout > 0)
				continue;

			IPMI_LOG_DEBUG("IPMB(%02X:%02X) Response handle timeout %ld:%ld -> %ld:%ld\n",
						   hndl->match.netfn, hndl->match.cmd,
						   hndl->start_time.tv_sec, hndl->start_time.tv_nsec,
						   expire.tv_sec, expire.tv_nsec);

			list_del(&hndl->list);
			list_del(&hndl->seq_list);

			hndl->next = table->freelist;
			table->freelist = hndl;
		}
		pthread_mutex_unlock(&table->lock);
	}

	return NULL;
}

/**
  *  @brief in IPMB interface, find IPMB session by sequence number
  *
  *  @param[in] match match information include session context
  *  @param[out] msgid if find the session, set the msgid
  *  @param[out] user_port if find the session, set the socket port
  *  @return retrun value
  *  @retval -1 failure
  *  @retval 0 sucessful
  */
static int ipmb_intf_find_seq(struct ipmb_match_info *match,
			unsigned int *msgid, unsigned short *user_port, ipmi_json_ipc_header_t *header)
{
	int rv = -1;
	unsigned char netfn = match->netfn & 0x3E;	/* request netfn */
	struct rsp_ipmb_hndl *hndl = NULL, *pos;
	struct rsp_ipmb_table *table = &ipmb_rsp_table;
	struct list_head *head = &table->seq_table[IPMB_SEQ_HASH(match->seq)];

	pthread_mutex_lock(&table->lock);
	list_for_each_entry(pos, head, seq_list) {
		if (pos->match.sa == match->sa &&
		    pos->match.lun == match->lun &&
		    pos->match.netfn == netfn &&
		    pos->match.cmd == match->cmd) {
			hndl = pos;
			break;
		}
	}

	if (hndl == NULL)
		goto ret;

	*msgid = hndl->msgid;
	*user_port = hndl->user_port;

	/*copy header*/
	memset(header->method, 0, sizeof(JRPC_METHOD_MAX));
	header->ip = hndl->header.ip;
	header->port = hndl->header.port;
	header->json_ipc_id = hndl->header.json_ipc_id;
	if (strlen((const char *)hndl->header.method) < JRPC_METHOD_MAX)
		memcpy(header->method, hndl->header.method, strlen((const char *)(hndl->header.method)));

	if (!hndl->broadcast) {
		list_del(&hndl->seq_list);
		list_del(&hndl->list);

		hndl->next = table->freelist;
		table->freelist = hndl;
	}

	rv = 0;

ret:
	pthread_mutex_unlock(&table->lock);

	return rv;
}

/**
  *  @brief in IPMB interface, find IPMB session by sequence number
  *
  *  @param[in] match match information include session context
  *  @param[out] msgid if find the session, set the msgid
  *  @param[out] user_port if find the session, set the socket port
  *  @return retrun value
  *  @retval -1 failure
  *  @retval 0 sucessful
  */
static int ipmb_intf_next_seq(struct ipmb_match_info *match,
			unsigned int timeo,
			unsigned int msgid, unsigned short user_port, ipmi_json_ipc_header_t header)
{
	int rv = -1;
	unsigned int i;
	unsigned char netfn = match->netfn & 0x3E;	/* request netfn */
	unsigned char seq = IPMB_SEQ_SIZE;
	struct rsp_ipmb_hndl *hndl, *pos;
	struct rsp_ipmb_table *table = &ipmb_rsp_table;

	pthread_mutex_lock(&table->lock);
	hndl = table->freelist;
	if (hndl == NULL) {
		IPMI_LOG_ERR("NXT SEQ no buff, adjust #IPMB_RSPHNDL_QUEUE_SIZE(%d)!\n",
					 IPMB_RSPHNDL_QUEUE_SIZE);
		goto ret;
	}

	for (i = table->curr_seq; IPMB_SEQ_HASH(i+1) != table->curr_seq && i < IPMB_SEQ_SIZE; i = IPMB_SEQ_HASH(i+1)) {
		int conflict = 0;

		list_for_each_entry(pos, &table->seq_table[i], seq_list) {
			if (pos->match.sa == match->sa &&
			    pos->match.lun == match->lun &&
			    pos->match.netfn == netfn &&
			    pos->match.cmd == match->cmd) {
				conflict = 1;
				break;
			}
		}

		if (!conflict) {
			IPMI_LOG_DEBUG("IPMB SA:%02X,LUN:%02X,NetFn:%02X,Cmd:%02X uses Seq:%02X\n",
						   match->sa, match->lun, netfn, match->cmd, i);
			seq = i;
			break;
		}
	}

	if (seq == IPMB_SEQ_SIZE) {
		IPMI_LOG_ERR("No avail seq number for IPMI msg in IPMB interface!\n");
		goto ret;
	}

	table->freelist = hndl->next;

	hndl->next = NULL;
	list_add_tail(&hndl->seq_list, &table->seq_table[seq]);
	list_add_tail(&hndl->list, &table->all_hndls);

	hndl->match.sa = match->sa;
	hndl->match.lun = match->lun;
	hndl->match.seq = seq;
	hndl->match.netfn = netfn;
	hndl->match.cmd = match->cmd;

	hndl->msgid = msgid;
	hndl->user_port = user_port;

	/*copy header*/
	memset(hndl->header.method, 0, sizeof(JRPC_METHOD_MAX));
	hndl->header.ip = header.ip;
	hndl->header.port = header.port;
	hndl->header.json_ipc_id = header.json_ipc_id;
	if (strlen((const char *)header.method) < JRPC_METHOD_MAX)
		memcpy(hndl->header.method, header.method, strlen((const char *)header.method));

#ifdef DEBUG_IPMI
	clock_gettime(CLOCK_REALTIME, &hndl->start_time);
#endif
	hndl->timeout = timeo;
	hndl->broadcast = is_ipmb_addr_bcast(match->sa);

	/* IPMB: 2.6.1 The Seq Field and Retries
	 * The Requester changes the Seq field whenever it issues a new instance
	 * of a command (request).
	 */
	match->seq = seq;	/* saved the seq result back */
	table->curr_seq = IPMB_SEQ_HASH(seq + 1);

	rv = 0;

ret:
	pthread_mutex_unlock(&table->lock);

	return rv;
}

/**
  *  @brief handle IPMB response msg
  *
  *  @param[in] match
  *  @param[in] rsp IPMB response data msg
  *  @param[in] rsp_len IPMB response data length
  *  @return
  */
static void handle_ipmb_msg_rsp(struct ipmb_match_info *match,
			unsigned char *rsp, int rsp_len)
{
	unsigned int msgid;
	unsigned short user_port;
	struct appmsg_ipmi_msg *ipmi;
	union app_msg_union msg;
	ipmi_json_ipc_header_t header;

	if (ipmb_intf_find_seq(match, &msgid, &user_port, &header) != 0) {
		IPMI_LOG_DEBUG("IPMB: No match request for this response!\n");
		return;
	}

	msg.hdr.type    = APPMSG_IPMI_MSG;
	msg.hdr.datalen = APPMSG_IPMI_MSG_LEN(rsp_len);

	ipmi = (struct appmsg_ipmi_msg *)msg.hdr.data;
	ipmi->id = msgid;

	memset(&ipmi->addr, 0, sizeof(ipmi->addr));
	ipmi->addr.type = IPMI_ADDR_TYPE_IPMB;
	ipmi->addr.addr.ipmb.sa = match->sa;
	ipmi->addr.addr.ipmb.lun = match->lun;
	ipmi->addr.addr.ipmb.seq = match->seq;

	ipmi->msg.netfn = match->netfn;
	ipmi->msg.cmd   = match->cmd;
	ipmi->msg.data_len = rsp_len;
	if ((ipmi->msg.data_len > 0) && (ipmi->msg.data_len <= IPMI_MAX_DATA_LENGTH))
		memcpy(ipmi->msg.data, rsp, rsp_len);

	deliver_app_msg_to_user(&msg.hdr, user_port, header);
}

/**
  *  @brief request to handle IPMB msg
  *
  *  @param[in] match
  *  @param[in] req IPMB request msg info
  *  @param[in] req_len IPMB request data length
  *  @return
  */
static void handle_ipmb_msg_req(struct ipmb_match_info *match,
			unsigned char *req, int req_len)
{
	unsigned short user_port;
	struct appmsg_ipmi_msg *ipmi;
	union app_msg_union msg;

	user_port = find_user_port(match->netfn, match->cmd);
	if (user_port == INVALID_PORT) {
		IPMI_LOG_DEBUG("IPMB: No user can handle this request!\n");
		return;
	}

	msg.hdr.type    = APPMSG_IPMI_MSG;
	msg.hdr.datalen = APPMSG_IPMI_MSG_LEN(req_len);

	ipmi = (struct appmsg_ipmi_msg *)msg.hdr.data;
	ipmi->id = 0;
	ipmi->timeo = 0;

	memset(&ipmi->addr, 0, sizeof(ipmi->addr));
	ipmi->addr.type = IPMI_ADDR_TYPE_IPMB;
	ipmi->addr.addr.ipmb.sa = match->sa;
	ipmi->addr.addr.ipmb.lun = match->lun;
	ipmi->addr.addr.ipmb.seq = match->seq;

	ipmi->msg.netfn = match->netfn;
	ipmi->msg.cmd   = match->cmd;
	ipmi->msg.data_len = req_len;
	if ((ipmi->msg.data_len > 0) && (ipmi->msg.data_len <= IPMI_MAX_DATA_LENGTH))
		memcpy(ipmi->msg.data, req, req_len);

	deliver_app_msg_to_user(&msg.hdr, user_port, ipmi->header);
}

/**
  *  @brief handle IPMB msg
  *
  *  @param[in] msg the msg need to be handled
  *  @param[in] len msg length
  *  @return
  */
void handle_ipmb_msg(unsigned char *msg, int len)
{
	struct ipmb_match_info match;

#ifdef DEBUG_IPMI
	{
		int __i;

		IPMI_LOG_DEBUG("IPMB IN: ");
		for (__i = 0; __i < len; __i++)
			IPMI_LOG_DEBUG("%02X ", msg[__i]);
		IPMI_LOG_DEBUG("\n");
	}
#endif

	if (msg[IPMB_RES_SLAVE_ADDR_OFFSET] != IPMI_IPMB_ADDR ||
		(msg[IPMB_RES_LUN_OFFSET] & IPMB_LUN_MASK) != IPMI_IPMB_LUN) {
		IPMI_LOG_DEBUG("The IPMB message is not for Me!\n");
		return;
	}

	if (len < 3 || ipmi_csum(&msg[IPMB_RES_SLAVE_ADDR_OFFSET], 3) != 0 ||
		ipmi_csum(&msg[IPMB_REQ_SLAVE_ADDR_OFFSET], len - 3) != 0) {
		IPMI_LOG_DEBUG("Invalid IPMB message!\n");
		return;
	}

	match.netfn = msg[IPMB_NETFN_OFFSET] >> 2;
	match.sa    = msg[IPMB_REQ_SLAVE_ADDR_OFFSET];
	match.seq   = msg[IPMB_SEQ_OFFSET] >> 2;
	match.lun   = msg[IPMB_REQ_LUN_OFFSET] & 0x03;
	match.cmd   = msg[IPMB_CMD_OFFSET];

	if (match.netfn & 0x01) {
		if (len < IPMB_RSP_MIN_LEN) {
			IPMI_LOG_DEBUG("The IPMB response is too small!\n");
			return;
		}

		handle_ipmb_msg_rsp(&match, &msg[IPMB_SENSOR_NUMBER_OFFSET], len - IPMB_RSP_MIN_LEN + 1); /* + ccode */
	} else {
		if (len < IPMB_REQ_MIN_LEN) {
			IPMI_LOG_DEBUG("The IPMB request is too small!\n");
			return;
		}

		handle_ipmb_msg_req(&match, &msg[IPMB_SENSOR_NUMBER_OFFSET], len - IPMB_REQ_MIN_LEN);
	}
}

/**
  *  @brief
  *
  *  @param
  *  @return
  */
static int format_ipmb_msg(unsigned char *buff, struct ipmi_msg *ipmi,
			unsigned char dst, unsigned dst_lun, unsigned char seq,
			unsigned char src, unsigned src_lun)
{
	buff[0] = dst;
	buff[1] = (ipmi->netfn << 2) | (dst_lun & 0x03);
	buff[2] = ipmi_csum(&buff[0], 2);

	buff[3] = src;
	buff[4] = (seq << 2) | (src_lun & 0x03);
	buff[5] = ipmi->cmd;

	if (ipmi->data_len > 0)
		memcpy(&buff[6], ipmi->data, ipmi->data_len);

	buff[6 + ipmi->data_len] = ipmi_csum(&buff[3], 3 + ipmi->data_len);

	return (7 + ipmi->data_len);
}

/**
  *  @brief deliver IPMI msg via IPMB
  *
  *  @param[in] msg the msg to be delivered via IPMB
  *  @return
  */
void deliver_ipmi_msg_by_ipmb(struct appmsg_ipmi_msg *msg)
{
	struct ipmi_msg *ipmi_msg;
	unsigned char seq;
	int           ipmb_msg_len;
	unsigned char ipmb_msg[IPMI_MAX_MSG_LENGTH];

	ipmi_msg = &msg->msg;
	if (ipmi_msg->data_len > IPMI_MAX_DATA_LENGTH) {
		IPMI_LOG_ERR("Too big IPMI message length[%d], ignore!\n", ipmi_msg->data_len);
		return;
	}

	if (ipmi_msg->data_len + offset_of(struct ipmi_msg, data)  > IPMI_MAX_MSG_LENGTH) {
		IPMI_LOG_ERR("Too big IPMI message to send, ignore!\n");
		return;
	}

	if (ipmi_msg->netfn & 0x01) {
		seq = msg->addr.addr.ipmb.seq;
	} else {
		struct ipmb_match_info match;

		match.sa  = msg->addr.addr.ipmb.sa;
		match.lun = msg->addr.addr.ipmb.lun;
		match.netfn = ipmi_msg->netfn;
		match.cmd   = ipmi_msg->cmd;

		if (msg->timeo > IPMI_MAX_TIMEOUT_MS)
			msg->timeo = IPMI_MAX_TIMEOUT_MS;
		if (msg->timeo == 0)
			msg->timeo = IPMI_DFLT_TIMEOUT_MS;

		if (ipmb_intf_next_seq(&match, msg->timeo, msg->id, msg->user_port, msg->header) != 0)
			return;

		seq = match.seq;
	}

	ipmb_msg_len = format_ipmb_msg(ipmb_msg, ipmi_msg,
						msg->addr.addr.ipmb.sa,
						msg->addr.addr.ipmb.lun,
						seq,
						IPMI_IPMB_ADDR, IPMI_IPMB_LUN);

	ipmb_send_outbound_msg(ipmb_msg, ipmb_msg_len);
}

/**
  *  @brief init the thread to handle IPMB msg
  *
  *  @param
  *  @return
  */
void init_ipmb_msg_handler(void)
{
	int i;
	struct rsp_ipmb_table *table = &ipmb_rsp_table;

	pthread_mutex_init(&table->lock, NULL);

	table->freelist = mem_freelist_create(IPMB_RSPHNDL_QUEUE_SIZE,
									sizeof(struct rsp_ipmb_hndl));
	if (table->freelist == NULL)
		FATAL("Failed to alloc ipmb rsp handle queue!\n");

	INIT_LIST_HEAD(&table->all_hndls);
	for (i = 0; i < IPMB_SEQ_SIZE; i++)
		INIT_LIST_HEAD(&table->seq_table[i]);

	table->curr_seq = 0;

	if (pthread_create(&table->time_tid, NULL, ipmb_time_thread, NULL) != 0)
		FATAL("Failed to create IPMB time thread!\n");
}

