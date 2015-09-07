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


#include <stdio.h>
#include "libjson/json.h"
#include "libjsonrpcapi/assetd_socket.h"
#include "libjsonrpcapi/registerd_socket.h"
#include "libjsonrpcapi/asset_module_api.h"
#include "libjsonrpcapi/assetd_api.h"
#include "libjsonrpcapi/utils.h"
#include "libjsonrpc/jsonrpc.h"
#include "libassetd/assetd_type.h"
#include "libassetd/assetd_jrpc_def.h"

int gami_register_req(gami_reg_t *reg_info)
{
	jrpc_req_pkg_t req_pkg = {};
	json_t *cap_array = NULL;
	json_t *cap = NULL;
	int i = 0;
	int rc;
	int64 ip = reg_info->ip_addr;
	int64 port = reg_info->port;

	printf("gami_register, name %s, ip %d, port %d.\n", reg_info->name, reg_info->ip_addr, reg_info->port);
	fill_param(&req_pkg, JRPC_GAMI_REG_NAME, reg_info->name, JSON_STRING);
	fill_param(&req_pkg, JRPC_GAMI_REG_IP, &ip, JSON_INTEGER);
	fill_param(&req_pkg, JRPC_GAMI_REG_PORT, &port, JSON_INTEGER);

	/*mbp*/
	cap_array = json_array();
	if (cap_array == NULL) {
		return -1;
	}

	for (i = 0; i < GAMI_MAX_CAP_NUM; i++) {
		if (strlen((char *)reg_info->capability.cap_mbp[i]) != 0) {
			cap = NULL;
			cap = json_object();
			if (cap == NULL) {
				return -1;
			}
			add_json_string(cap, JRPC_GAMI_CAP_NAME, (unsigned char *)reg_info->capability.cap_mbp[i]);
			json_array_add(cap_array, cap);
		}
	}
	fill_param(&req_pkg, JRPC_GAMI_REG_CAP_MBP, cap_array, JSON_ARRAY);

	/*psu*/
	cap_array = json_array();
	if (cap_array == NULL) {
		return -1;
	}

	for (i = 0; i < GAMI_MAX_CAP_NUM; i++) {
		if (strlen((char *)reg_info->capability.cap_psu[i]) != 0) {
			cap = NULL;
			cap = json_object();
			if (cap == NULL) {
				return -1;
			}
			add_json_string(cap, JRPC_GAMI_CAP_NAME, (unsigned char *)reg_info->capability.cap_psu[i]);
			json_array_add(cap_array, cap);
		}
	}
	fill_param(&req_pkg, JRPC_GAMI_REG_CAP_PSU, cap_array, JSON_ARRAY);

	/*fan*/
	cap_array = json_array();
	if (cap_array == NULL) {
		return -1;
	}

	for (i = 0; i < GAMI_MAX_CAP_NUM; i++) {
		if (strlen((char *)reg_info->capability.cap_fan[i]) != 0) {
			cap = NULL;
			cap = json_object();
			if (cap == NULL) {
				return -1;
			}
			add_json_string(cap, JRPC_GAMI_CAP_NAME, (unsigned char *)reg_info->capability.cap_fan[i]);
			json_array_add(cap_array, cap);
		}
	}
	fill_param(&req_pkg, JRPC_GAMI_REG_CAP_FAN, cap_array, JSON_ARRAY);

	/*drawer*/
	cap_array = json_array();
	if (cap_array == NULL) {
		return -1;
	}

	for (i = 0; i < GAMI_MAX_CAP_NUM; i++) {
		if (strlen((char *)reg_info->capability.cap_drawer[i]) != 0) {
			cap = NULL;
			cap = json_object();
			if (cap == NULL) {
				return -1;
			}
			add_json_string(cap, JRPC_GAMI_CAP_NAME, (unsigned char *)reg_info->capability.cap_drawer[i]);
			json_array_add(cap_array, cap);
		}
	}
	fill_param(&req_pkg, JRPC_GAMI_REG_CAP_DRAWER, cap_array, JSON_ARRAY);

	rc = send_msg_to_registerd(&req_pkg, REG_APP);

	return rc;
}



