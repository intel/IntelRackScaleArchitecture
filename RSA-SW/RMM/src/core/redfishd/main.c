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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "libutils/dump.h"

#include "libutils/sock.h"
#include "libutils/rmm.h"
#include "libredfish/rf_event.h"
#include "libredfish/msg_reg.h"
#include "librmmcfg/rmm_cfg.h"
#include "rf_memdb.h"
#include "rf_log.h"
#include "libutils/curl_ref.h"
#include "libutils/string.h"
#include "libinit/libinit.h"

static void record_rmm_log(int32 level, int8 *module_name, int8 *func_name, int8 *msg)
{
	rmm_log_ex(module_name, func_name, level, "%s", msg);
}

static void record_rf_log(int32 level, int32 msg_num, int8 *str_args)
{
	RF_RECORD_EVT(level, "%d %s", msg_num, str_args);
}

/**
*
* Asset index could be queried via offset and msg args. Msg entity would be queried via msg sn.
* Redfish daemon maintain a map of msg unique string and memdb node id, we can find redfish event node via
* msg unique string which could be queried via msg sn.
*
* For redfish log recording:
* 1. All redfish msg are required to be recorded in rmm log, like other modules. It requires to query
*    rmm msg format via msg_sn and msg args
* 2. Redfish msg is also recorded in redfish log for redfish log query.
* 3. Redfish event msg will be recorded in step 1 & 2 and posted to listeners
*
* Diagram for step 1 and 2:
* .--------------------.   .----------------.   .------------.     .-------------.
* |query location index|-->|query msg entity|-->| record log |--> < process event >
* `--------------------'   `----------------'   `------------'     `-------------"
*
* Diagram for step 3:
* .------------------.   .-----------------------.   .------------------.
* |query msg identity|-->|query listeners via map|-->|post redfish event|
* `------------------'   `-----------------------'   `------------------'
*/
static int32 rf_msg_handler(struct rf_log_req_info *req_info)
{
	int32 location_idx = 0;
	int8 msg_id_str[256] = {0};
	struct dest_info *listener;
	struct dest_info *tmp;
	struct dest_info *tmp2;
	int8 msg[256] = {0};

	int32 msg_sn = req_info->data.fmt1.msg_sn;
	int32 level = req_info->data.fmt1.level;

	location_idx = msg_reg_get_location_idx(msg_sn, req_info->data.fmt1.str_args);
	msg_reg_get_msg_str(msg, msg_sn, req_info->data.fmt1.str_args);
	printf("msg:		%s", msg);

	/*record log and event.*/
	record_rf_log(level, msg_sn, req_info->data.fmt1.str_args);

	record_rmm_log(level,
			req_info->data.fmt1.module_name,
			req_info->data.fmt1.func_name,
			msg);

	/*If the request is event, post event to destination*/
	if (req_info->data.fmt1.is_event == RF_EVENT) {
		msg_reg_get_msg_id_str(msg_id_str, 256, msg_sn);
		listener = (struct dest_info *)malloc(sizeof(struct dest_info));
		if (listener == NULL) {
			rmm_log(ERROR, "malloc failed\n");
			return -1;
		}

		memset(listener, 0, sizeof(struct dest_info));

		rf_memdb_get_listeners(msg_id_str, location_idx, listener);
		tmp = listener;
		while (tmp) {
			if (strstr(tmp->dest, "http") != NULL){
				curl_post_msg(tmp->dest, msg);
			}
			tmp2 = tmp->pnext;
			free(tmp);
			tmp = tmp2;
		}
	}

	return 0;
}

static void set_socket_addr(struct sockaddr_in *addr, int32 port)
{
	memset(addr, 0, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

static void parser_log(int8 *data, int8 *out)
{
	int32 line_num = 0;
	int8 date[16] = {0};
	int8 ct[32] = {0};
	int8 level[16] = {0};
	int8 func_name[32] = {0};
	int32 msg_sn = 0;
	int8 str_args[128] = {0};

	int8 msg[RF_MSG_MAX_LEN] = {0};

	if (sscanf(data, "%d %s %s %s %s %d %s", &line_num, date, ct, level, func_name, &msg_sn, str_args) == -1)
		rmm_log(ERROR, "%s", "sscanf return error.\n");
	msg_reg_get_msg_str(msg, msg_sn, str_args);
	snprintf(out, RF_MSG_MAX_LEN, "%d %s %s %s %s %s", line_num, date, ct, level, func_name, msg);
}

static void parser_logs(int8 *resp_data, int32 read_lines_count)
{
	int32 index = 0;
	int8 out[RF_MSG_MAX_LEN] = {0};

	for (index = 0; index < read_lines_count; index++) {
		memset(out, 0, RF_MSG_MAX_LEN);
		parser_log(resp_data + RF_MSG_MAX_LEN * index, out);
		strncpy_safe(resp_data + RF_MSG_MAX_LEN * index, out, RF_MSG_MAX_LEN, RF_MSG_MAX_LEN - 1);
	}
}

static void rf_get_log_handler(int32 fd, struct sockaddr_in addr_from, int32 msg_req_count)
{
	struct rf_log_desc	resp_disc;
	int8 *resp_data = NULL;
	int32 val = 0;
	int32 read_lines_count = 0;
	int32 msg_count = 0;
	int32 index = 0;
	int32 msg_length = 0;
	int32 addrlen = sizeof(addr_from);

	if (msg_req_count > 0) {
		resp_data = (char *)malloc(abs(msg_req_count) * RF_MSG_MAX_LEN);
		read_lines_count = rf_log_get(msg_req_count, resp_data);
		parser_logs(resp_data, read_lines_count);
		resp_disc.count = read_lines_count;
		resp_disc.length = read_lines_count * RF_MSG_MAX_LEN;
	} else {
		resp_disc.count = 0;
		resp_disc.length = 0;
	}

	val = fcntl(fd, F_GETFL, 0);
	if (fcntl(fd, F_SETFL, val & ~O_NONBLOCK) < 0)
		return;

	sendto(fd, (unsigned char *)(&resp_disc), sizeof(struct rf_log_desc), 0, (struct sockaddr *)&addr_from, addrlen);
	usleep(100);
	while (index * BUF_SIZE < resp_disc.length) {
		if ((index  + 1) * BUF_SIZE > resp_disc.length)
			msg_length = resp_disc.length - index * BUF_SIZE;
		else
			msg_length = BUF_SIZE;
		sendto(fd, (unsigned char *)resp_data + index * BUF_SIZE, msg_length, 0, (struct sockaddr *)&addr_from, addrlen);
		usleep(100);
		index++;
	}

	if (fcntl(fd, F_SETFL, val & O_NONBLOCK) < 0)
		return;
	if (resp_data)
		free(resp_data);
	resp_data = NULL;
}

static void sigterm_handler(int32 signum)
{
	/* do cleanup jobs here */
	exit(0);
}

int main(int32 argc, int8 **argv)
{
	int32 rc;
	int32 fd;
	fd_set fds;
	int32 port;
	socklen_t addrlen;
	struct sockaddr_in addr_from;
	struct rf_log_req_info req_info;

	reg_sigterm_handler(sigterm_handler);

	if(rmm_modules_init(MODULEINIT_COREDUMP | MODULEINIT_LOG, 0,0))
		exit(-1);

	port = rmm_cfg_get_port(REDFISHD_PORT);
	if (port == 0) {
		rmm_log(ERROR, "%s", "Fail to call rmm_cfg_get_redfishd_port.\n");
		exit(-1);
	}
	/*Prepare to listening information.*/

	fd = create_udp_listen(INADDR_LOOPBACK, port, 0, 0);
	if (fd < 0)
		return -1;

	printf("redfish daemon is Running ...\n");

	/*redfish log init.*/
	rf_log_init();

	/*Redfish MessageRegistry init, load default config file.*/
	msg_reg_init(NULL);
	rf_memdb_event_node_init();
	curl_init();

	set_socket_addr(&addr_from, port);
	addrlen = sizeof(addr_from);
	for (;;) {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		rc = select(fd + 1, &fds, NULL, NULL, NULL);
		if (rc < 0)
			continue;

		rc = recvfrom(fd, &req_info, sizeof(struct rf_log_req_info), 0, (struct sockaddr *)&addr_from, &addrlen);
		if (rc <= 0)
			continue;
		switch (req_info.type) {
		case RF_MSG_REQ:
			rf_msg_handler(&req_info);
			break;
		case RF_GET_LOG_BY_LIMIT:
			rf_get_log_handler(fd, addr_from, req_info.data.fmt2.count);
			break;
		default:
			printf("Error message type: %d", req_info.type);
			break;
		}
	}

	close(fd);
	curl_uninit();
	return 0;
}

