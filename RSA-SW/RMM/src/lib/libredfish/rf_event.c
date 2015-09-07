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


#include <stdlib.h>
#include <stdarg.h>

#include "libutils/sock.h"
#include "libutils/string.h"
#include "libredfish/rf_types.h"
#include "libredfish/msg_reg.h"
#include "libredfish/snmp_event.h"

#define _GNU_SOURCE
extern const char *__progname;

static int redfish_fd	= -1;
static int snmp_subagentd_fd = -1;

int rf_connect(unsigned int host, int port)
{
	redfish_fd = udp_connect(host, port);
	if (redfish_fd < 0) {
		return -1;
	}

	msg_reg_init(NULL);

	return 0;
}

int snmp_subagentd_connect(unsigned int host, int port)
{
	snmp_subagentd_fd = udp_connect(host, port);
	if (snmp_subagentd_fd < 0) {
		return -1;
	}

	msg_reg_init(NULL);

	return 0;
}

static int _msg_sent(int fd, const char *func_name, int level, int is_evt, int msg_sn, char *args)
{
	struct rf_log_req_info req_info;

	req_info.type = RF_MSG_REQ;
	req_info.data.fmt1.is_event = is_evt;
	req_info.data.fmt1.msg_sn = msg_sn;
	req_info.data.fmt1.level = level;
	strncpy_safe(req_info.data.fmt1.func_name, func_name, MAX_FUNC_NAME_LEN, MAX_FUNC_NAME_LEN - 1);
	strncpy_safe(req_info.data.fmt1.module_name, __progname, MAX_MODULE_NAME_LEN, MAX_MODULE_NAME_LEN - 1);
	strncpy_safe(req_info.data.fmt1.str_args, args, RF_MSG_MAX_LEN, RF_MSG_MAX_LEN - 1);

	return socket_send(fd,	&req_info, sizeof(struct rf_log_req_info));
}

int rf_log_get_by_limit(int count, struct rf_log_desc *desc_rsp, char *data)
{
	int rc = -1;
	int val;
	int index = 0;
	int msg_length = 0;
	struct rf_log_req_info req_info;

	req_info.type = RF_GET_LOG_BY_LIMIT;
	req_info.data.fmt2.count = count;
	val = fcntl(redfish_fd, F_GETFL, 0);
	if (fcntl(redfish_fd, F_SETFL, val & ~O_NONBLOCK) < 0)
		return -1;

	rc = send(redfish_fd, &req_info, sizeof(struct rf_log_req_info), 0);
	if (rc < 0)
		goto expt_end;

	usleep(10);
	rc = socket_recv(redfish_fd, desc_rsp, sizeof(struct rf_log_desc));
	if (rc <= 0) {
		goto expt_end;
	}
	printf("info length: %d", desc_rsp->length);
	if (desc_rsp->length) {
		while (index * BUF_SIZE < desc_rsp->length) {
			if ((index  + 1) * BUF_SIZE > desc_rsp->length)
				msg_length = desc_rsp->length - index * BUF_SIZE;
			else
				msg_length = BUF_SIZE;
			rc = socket_recv(redfish_fd, data + index * BUF_SIZE, msg_length);
			if (rc <= 0) {
				goto expt_end;
			}
			index++;
		}
	}
	rc = 0;
expt_end:
	fcntl(redfish_fd, F_SETFL, val & O_NONBLOCK);
	return rc;
}


/**
* Before sending redfish msg, need to transform msg into compact redfish msg format
*
*/
int rf_msg_send(const char *func_name, int level, int is_evt, int msg_sn, ...)
{
	int ret = 0;
	char msg_args_fmt[RF_MSG_MAX_LEN] = {0};
	char str_args[RF_MSG_MAX_LEN] = {0};
	va_list args;

	/*get msg args types, such as "%s,%d,%d,%s" to express the args format*/
	ret = msg_reg_get_args_format(msg_args_fmt, RF_MSG_MAX_LEN, msg_sn);
	if (ret) {
		printf("get msg type failed, ret is %d .......\n", ret);
		return RF_ERR;
	}

	va_start(args, msg_sn);
	vsnprintf(str_args, RF_MSG_MAX_LEN, msg_args_fmt, args);
	va_end(args);

	_msg_sent(redfish_fd, func_name, level, is_evt, msg_sn, str_args);

	return RF_SUCCESS;
}

int snmp_subagentd_msg_send(const char *func_name, int level, int is_evt, int msg_sn, ...)
{
	int ret = 0;
	char msg_args_fmt[RF_MSG_MAX_LEN] = {0};
	char str_args[RF_MSG_MAX_LEN] = {0};
	va_list args;

	/*get msg args types, such as "%s,%d,%d,%s" to express the args format*/
	ret = msg_reg_get_args_format(msg_args_fmt, RF_MSG_MAX_LEN, msg_sn);
	if (ret) {
		printf("get msg type failed, ret is %d .......\n", ret);
		return RF_ERR;
	}

	va_start(args, msg_sn);
	vsnprintf(str_args, RF_MSG_MAX_LEN, msg_args_fmt, args);
	va_end(args);

	_msg_sent(snmp_subagentd_fd, func_name, level, is_evt, msg_sn, str_args);

	return RF_SUCCESS;
}

int snmp_subagentd_set(int is_remove, char *ip, int port)
{
	struct rf_log_req_info req_info;
	memset(&req_info, 0, sizeof(struct rf_log_req_info));

	req_info.type = RF_SET_TRAP_IP;
	req_info.data.fmt3.is_remove = is_remove;
	memcpy (req_info.data.fmt3.ip, ip, strlen(ip));
	req_info.data.fmt3.port = port;
	return socket_send(snmp_subagentd_fd, &req_info, sizeof(struct rf_log_req_info));
}