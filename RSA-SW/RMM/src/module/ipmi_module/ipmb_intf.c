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


#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libutils/sock.h"
#include "librmmcfg/rmm_cfg.h"
#include "ipmi_log.h"
#include "event.h"
#include "ipmi.h"

#define IPMB_MSG_QUEUE_NUM		256

struct ipmb_recv_msg {
	struct ipmb_recv_msg *next;		/* free buffer link */

	int           len;
	unsigned char msg[IPMI_MAX_MSG_LENGTH];
};

struct ipmb_msg_head {
	struct ipmb_recv_msg  *head;
	struct ipmb_recv_msg **tail;
};

static struct fd_event       ipmb_fd_event;

/**
  *  @brief open IPMB interface
  *
  *  @param
  *  @return
  */
static inline int open_ipmb_interface(void)
{
	int port;

	port = rmm_cfg_get_port(IPMIIPMB_PORT);
	if (port == 0) {
		IPMI_LOG_ERR("Failed to call rmm_cfg_get_ipmi_ipmb_port\n");
		exit(-1);
	}

	return create_udp_listen(INADDR_LOOPBACK, port, 0, 0);
}

/**
  *  @brief send IPMB outband msg
  *
  *  @param[in] msg the msg need to be send
  *  @param[in] len msg length
  *  @return
  */
void ipmb_send_outbound_msg(unsigned char *msg, int len)
{
	struct sockaddr_in dest;
	int port;

#ifdef DEBUG_IPMI
	{
		int __i;

		IPMI_LOG_DEBUG("IPMB OUT: ");
		for (__i = 0; __i < len; __i++)
			IPMI_LOG_DEBUG("%02X ", msg[__i]);
		IPMI_LOG_DEBUG("\n");
	}
#endif

	port = rmm_cfg_get_port(IPMII2CD_PORT);
	if (port == 0) {
		IPMI_LOG_ERR("Failed to call rmm_cfg_get_ipmi_i2cd_port\n");
		exit(-1);
	}
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	sendto(ipmb_fd_event.fd, msg, len, 0, (struct sockaddr *)&dest, sizeof(dest));
}

/**
  *  @brief receive IPMB inband msg
  *
  *  @param[in] fd socket fd
  *  @return
  */
static void ipmb_recv_inbound_msg(int fd)
{
	int rc;
	struct ipmb_recv_msg msg = {0};

	for (; ipmi_module_exit == false;) {
		memset(&msg, 0, sizeof(struct ipmb_recv_msg));

		rc = recv(fd, msg.msg, sizeof(msg.msg), 0);
		if (rc <= 0)
			return;

		msg.len = rc;

		handle_ipmb_msg(msg.msg, msg.len);
	}
}

/**
  *  @brief start IPMB interface
  *
  *  @param
  *  @return
  */
void register_ipmb_resp(void)
{
	init_ipmb_msg_handler();

	ipmb_fd_event.fd = open_ipmb_interface();
	if (ipmb_fd_event.fd == -1)
		FATAL("Failed to open IPMB interface!\n");
	ipmb_fd_event.handle_fd = ipmb_recv_inbound_msg;
	fd_event_add(&ipmb_fd_event);

	IPMI_LOG_INFO("IPMB-Intf is started successfully!\n");
}

