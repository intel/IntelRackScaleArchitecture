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


#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "libwrap/wrap.h"
#include "libjsonrpc/jsonrpc.h"
#include "libjsonrpcapi/assetd_api.h"
#include "libjsonrpcapi/asset_module_socket.h"
#include "libassetd/assetd_jrpc_def.h"
#include "libassetd/assetd_type.h"
#include "libassetmodule/am_action.h"
#include "librmmcfg/rmm_cfg.h"
#include "librmmlog/rmmlog.h"



int fill_array_elem(json_t *array, char *attr_name, void *value_ptr, json_type type)
{
	json_t *elem = json_object();
	json_t *obj = json_object();

	switch(type) {
		case JSON_STRING:
			add_json_string(obj, JRPC_ACTION_VALUE_STR, (unsigned char *)value_ptr);
			break;
		case JSON_INTEGER:
			add_json_integer(obj, JRPC_ACTION_VALUE_NUM, *(int *)value_ptr);
			break;
		case JSON_TRUE:
			add_json_bool(obj, JRPC_ACTION_VALUE_BOOL, *(int *)value_ptr);
		default:
			return -1;
	}
	
	json_object_add(elem, JRPC_ACTION_ATTR_NAME, json_string(attr_name));
	json_object_add(elem, JRPC_ACTION_VALUE, obj);

	json_array_add(array, elem);
	return 0;
}

void fill_jrpc_action_pkg(jrpc_req_pkg_t *req_pkg, int action_id, json_t *attr)
{
	if (action_id < sizeof(am_action_map)/sizeof(am_action_map[0]))
		fill_param(req_pkg, JRPC_ACTION_NAME, am_action_map[action_id].action_name, JSON_STRING);
	fill_param(req_pkg, JRPC_ACTION_ATTR, attr, JSON_OBJECT);

	return;
}


static int get_mbp_host_ip(memdb_integer node_id, int64 *host64)
{
	int rc;
	int host = 0;

	rc = libdb_attr_get_int(DB_RMM, node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(rc != 0) {
		return (int)rc;
	}
	*host64 = host;

	return 0;
}


int am_get_fan_pwm_by_id(memdb_integer* cm_node_id, memdb_integer* fan_node_id, int64 fan_idx)
{
	int rc = -1;
	int64 host64 = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};

	rc = get_mbp_host_ip(*cm_node_id, &host64);
	if(rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_CM_NODE_ID, cm_node_id, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_FAN_NODE_ID, fan_node_id, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_FAN_LID, &fan_idx, JSON_INTEGER);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_GET_FAN_PWM_BY_ID, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	if (rc == 0) {
		json_t *json_pwm = json_object_get(rsp_pkg.data.result.value_obj, JRPC_FAN_PWM);
		char *pwm = json_string_value(json_pwm);
		
		if (libdb_attr_set_string(DB_RMM, *fan_node_id,
			FAN_DESIRED_SPD_PWM_STR,0x0, pwm,
			SNAPSHOT_NEED_NOT, LOCK_ID_NULL) == -1) {
			return RESULT_ATTR_ERR;
		}
	}

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}

int am_set_fan_pwm(int64 tzone_idx, int64 fan_idx, int64 pwm)
{
	int rc = -1;
	int64 host64 = 0;
	memdb_integer cm_node_id = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};
	int cm_lid = 0;

	cm_lid = get_cm_lid(MC_TYPE_TZONE, tzone_idx);
	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	rc = get_mbp_host_ip(cm_node_id, &host64);
	if(rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_TZ_LID, &tzone_idx, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_FAN_LID, &fan_idx, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_PWM, &pwm, JSON_INTEGER);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_SET_FAN_PWM, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}


int am_peripheral_hard_reset(int64 cm_idx, int64 peripheral_id, int32 * result)
{
	int rc = -1;
	int64 host64 = 0;
	int64 cm_lid = cm_idx;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};

	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, MC_TYPE_RMC, MC_TYPE_CM);
	rc = get_mbp_host_ip(cm_node_id, &host64);
	if(rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_CM_LID, &cm_lid, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_PERI_ID, &peripheral_id, JSON_INTEGER);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_RESET_HARD_RESET, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}


int am_post_psu_by_node_id(int64 rack_pzone_idx, int64 psu_idx, int64 request_enabled_state)
{
	int rc = -1;
	int64 host64 = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};
	int cm_lid = get_cm_lid(MC_TYPE_TZONE, rack_pzone_idx);
	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);

	rc = get_mbp_host_ip(cm_node_id, &host64);
	if(rc != 0) {
		rmm_log(ERROR, "get cm ip fail\n");
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_PZ_LID, &rack_pzone_idx, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_PSU_LID, &psu_idx, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_PSU_ENABLE_STATE, &request_enabled_state, JSON_INTEGER);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_POST_PSU_BY_NODE_ID, attr_array);

	memdb_integer psu_node_id = 0;
	libwrap_get_psu_node_id_by_idx(rack_pzone_idx, psu_idx, &psu_node_id);
	rc = libdb_attr_set_int(DB_RMM, psu_node_id, PSU_ENABLE_ST_STR,
						  0x0, request_enabled_state, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
			rmm_log(ERROR, "memdb set psu enable state fail\n");
			return -1;
	}

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}

static json_t *integer_array_to_json(int *data, int max)
{
	int i = 0;
	json_t *jdata = json_array();

	for (i = 0; i < max; i++) {
		json_array_add(jdata, json_integer(data[i]));
	}

	return jdata;
}


int am_set_id_field(memdb_integer *node_id, int64 field_type, int64 field_instance, int64 byte_num, int32 *data)
{
	int rc = -1;
	int64 host64 = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};

	rc = get_mbp_host_ip(*node_id, &host64);
	if(rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_NODE_ID, node_id, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_FIELD_TYPE, &field_type, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_FIELD_INSTANCE, &field_instance, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_BYTE_NUM, &byte_num, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_DATA, integer_array_to_json(data, byte_num), JSON_ARRAY);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_SET_ID_FIELD, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}


int am_get_id_field(memdb_integer *node_id, int64 field_instance)
{
	int rc = -1;
	int64 host64 = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};

	rc = get_mbp_host_ip(*node_id, &host64);
	if(rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_NODE_ID, node_id, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_FIELD_INSTANCE, &field_instance, JSON_INTEGER);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_GET_ID_FIELD, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}


int am_get_aggregate_thermal_sensor(memdb_integer *node_id)
{
	int rc = -1;
	int64 host64 = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};

	rc = get_mbp_host_ip(*node_id, &host64);
	if(rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_NODE_ID, node_id, JSON_INTEGER);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_GET_THERMAL_SENSOR, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}

int am_get_aggregate_health_sensor(memdb_integer *node_id)
{
	int rc = -1;
	int64 host64 = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};

	rc = get_mbp_host_ip(*node_id, &host64);
	if(rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_NODE_ID, node_id, JSON_INTEGER);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_GET_HEALTH_SENSOR, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}

int am_get_tray_power(memdb_integer *node_id)
{
	int rc = -1;
	int64 host64 = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};

	rc = get_mbp_host_ip(*node_id, &host64);
	if(rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_NODE_ID, node_id, JSON_INTEGER);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_GET_TRAY_POWER, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

	return rc;
}

int am_uart_switch(int64 cm_lid, int64 tray_lid, int64 tgt_lid)
{
	int rc = -1;
	int64 host64 = 0;
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};

	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	rc = get_mbp_host_ip(cm_node_id, &host64);
	if (rc != 0) {
		return (int)rc;
	}

	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_MBP_HOST_IP, &host64, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_CM_LID, &cm_lid, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_TARGET_IDX, &tgt_lid, JSON_INTEGER);
	if (is_platform_bdcr())
		fill_array_elem(attr_array, JRPC_TRAY_IDX, &tray_lid, JSON_INTEGER);

	fill_jrpc_action_pkg(&req_pkg, AMACTION_MBP_UART_SWITCH, attr_array);

	rc = send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	return rc;
}
