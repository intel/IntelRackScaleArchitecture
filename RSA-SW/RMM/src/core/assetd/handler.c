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
#include <stdio.h>
#include "libassetd/assetd_type.h"
#include "libassetd/assetd_jrpc_def.h"
#include "libjsonrpcapi/assetd_api.h"
#include "libwrap/wrap.h"
#include "libmemdb/memdb.h"
#include "librmmlog/rmmlog.h"
#include "librmmcfg/rmm_cfg.h"
#include "libjsonrpc/jsonrpc.h"
#include "libredfish/mr_def.h"
#include "libutils/rack.h"
#include "libutils/rmm.h"
#include "libutils/rmm_json.h"
#include "handler.h"
#include "attribute.h"
#include "map.h"

struct reset_param {
	int host;
	int cm_idx;
};

struct _mbp_hw_info {
	unsigned char hw_addr[8];
};

static struct _mbp_hw_info mbp_hw_info[MAX_CM_NUM] = {
	{"usb:2-3"},
	{"usb:2-4"},
};

typedef int (*cmd_handle_fn) (jrpc_req_pkg_t *req, json_t *resp);
typedef int (*handle_fn_add) (char *uuid, char *parent_uuid, json_t *req, json_t *resp);
typedef int (*handle_fn_change) (char *uuid, json_t *req, json_t *resp);

static int on_add(jrpc_req_pkg_t *req, json_t *resp);
static int on_remove(jrpc_req_pkg_t *req, json_t *resp);
static int on_change(jrpc_req_pkg_t *req, json_t *resp);

#if 0
static int set_id_field(jrpc_req_pkg_t *req, json_t *resp);
static int get_id_field(jrpc_req_pkg_t *req, json_t *resp);
static int get_thermal_sensor(jrpc_req_pkg_t *req, json_t *resp);
static int get_health_sensor(jrpc_req_pkg_t *req, json_t *resp);
static int get_tray_power(jrpc_req_pkg_t *req, json_t *resp);

static int get_fan_pwm_by_id(jrpc_req_pkg_t *req, json_t *resp);
static int set_fan_pwm(jrpc_req_pkg_t *req, json_t *resp);
static int post_psu_by_node_id(jrpc_req_pkg_t *req, json_t *resp);
static int peripheral_hard_reset(jrpc_req_pkg_t *req, json_t *resp);
#endif
static int rmm_factory_reset(jrpc_req_pkg_t *req, json_t *resp);

static cmd_handle_fn cmd_handles[MAX_EVT] = {
	[ADD_EVT]				= on_add,
	[REMOVE_EVT]			= on_remove,
	[CHANGE_EVT]			= on_change,
	#if 0
	[SET_ID_FIELD]			= set_id_field,
	[GET_ID_FIELD]			= get_id_field,
	[GET_THERMAL_SENSOR]	= get_thermal_sensor,
	[GET_HEALTH_SENSOR]		= get_health_sensor,
	[GET_TRAY_POWER]		= get_tray_power,
	[GET_FAN_PWM_BY_ID]		= get_fan_pwm_by_id,
	[SET_FAN_PWM]			= set_fan_pwm,
	[RESET_HARD_RESET]		= peripheral_hard_reset,
	[POST_PSU_BY_NODE_ID]	= post_psu_by_node_id,
	#endif
	[RMM_FACTORY_RESET]		= rmm_factory_reset
};

typedef struct cmd_func_map {
	int type;
	handle_fn_add		on_add;
	handle_fn_change	on_change;
	/* data */
} cmd_func_map_t;

static void remove_node(memdb_integer node_id, memdb_integer parent, memdb_integer type)
{
	int msg_sn = 0;
	int rc = 0;
	char uuid[64] = {0};
	char lid_str[64] = {0};

	switch (type) {
	case MC_TYPE_CM:
		msg_sn = MSGMbpRemove;
		break;
	case MC_TYPE_PZONE:
		msg_sn = MSGPZoneRemove;
		break;
	case MC_TYPE_PSU:
		msg_sn = MSGPsuRemove;
		break;
	case MC_TYPE_TZONE:
		msg_sn = MSGTZoneRemove;
		break;
	case MC_TYPE_FAN:
		msg_sn = MSGFanRemove;
		break;
	case MC_TYPE_DRAWER:
		msg_sn = MSGDrawerRemove;
		break;
	case MC_TYPE_DZONE:
		msg_sn = MSGDZoneRemove;
		break;
	default:
		break;
	}

	rc = libdb_attr_get_string(DB_RMM, node_id, WRAP_UUID_STR, uuid, sizeof(uuid), LOCK_ID_NULL);
	if (rc == -1 || strlen(uuid) == 0) {
		rmm_log(ERROR, "memdb get uuid fail\n");
		return;
	}
	nmap_remove_by_node_id(node_id);

	if (msg_sn != 0) {
		rc = libdb_attr_get_string(DB_RMM, node_id, WRAP_LOC_ID_STR, lid_str, sizeof(lid_str), LOCK_ID_NULL);
		if (rc == -1 || strlen(uuid) == 0) {
			rmm_log(ERROR, "memdb get location id fail\n");
			return;
		}
		rf_snmp_evt(INFO, msg_sn, lid_str);
	}
}

static void ad_remove_node(char *uuid)
{
	int i = 0;
	struct node_info *subnode;
	struct node_info *cnode;
	int nodenum = 0;

	memdb_integer node_id = 0;
	if (nmap_get_node_id_by_uuid(&node_id, uuid) == -1)
		return;

	subnode = libdb_list_subnode(DB_RMM, node_id, &nodenum, NULL,LOCK_ID_NULL);
	cnode = libdb_get_node_by_node_id(DB_RMM, node_id, LOCK_ID_NULL);
	for (i = 0; i < nodenum; i++) {
		remove_node(subnode[i].node_id, subnode[i].parent, subnode[i].type);
	}

	if (cnode != NULL) {
		remove_node(cnode->node_id, cnode->parent, cnode->type);
	}
	libdb_destroy_node(DB_RMM, node_id, LOCK_ID_NULL);
}

void set_chassis_fw_ver(uint8 ver_low, uint8 ver_hi, memdb_integer node_id)
{
	char tmp[8] = {0};
	char ver_l[8] = {0};
	char ver_h[8] = {0};
	char buff[32] = {0};
	int ret;

	snprintf(tmp, sizeof(tmp), "%02X", ver_low);
	ret = sscanf(tmp, "%[0-9a-fA-F]", ver_l);
	if (ret != 1)
		return;

	snprintf(tmp, sizeof(tmp), "%02X", ver_hi);
	ret = sscanf(tmp, "%[0-9a-fA-F]", ver_h);
	if (ret != 1)
		return;

	snprintf(buff, sizeof(buff), "%s.%s", ver_h, ver_l);

	if (libdb_attr_set_string(DB_RMM, node_id, MBP_FW_VER_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL) == -1) {
		rmm_log(ERROR, "memdb set mbp fw ver fail\n");
	}
}

result_t get_mbp_hw_addr(unsigned int pos, unsigned char *hw_addr)
{
	if ((1 == pos) || (2 == pos)) {
		memcpy((char *)hw_addr, (char *)mbp_hw_info[pos - 1].hw_addr,
			   strlen((char *)(mbp_hw_info[pos - 1].hw_addr)));
	}

	return RESULT_OK;
}

int on_cm_add(char *uuid, char *parent_uuid, json_t *req, json_t *resp)
{
	char buff[16] = {0};
	memdb_integer node_id = 0;
	int64 cm_lid = 0;

	if (jrpc_get_named_param_value(req, JRPC_CM_LID, JSON_INTEGER, &cm_lid) != JSONRPC_SUCCESS)
		return -1;

	node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	if (node_id == INVALID_NODE_ID) {
		/* cm node not exist, creating cm node and cooling zone node, set all attribute */
		node_id = libdb_create_node(DB_RMM, MC_TYPE_RMC,
								 MC_TYPE_CM, SNAPSHOT_NEED, LOCK_ID_NULL);
		rmm_log(INFO, "new node created, id:%lu\n", node_id);
		char lid_str[64] = {0};
		snprintf(lid_str, sizeof(lid_str), "%lld", cm_lid);
		rf_snmp_evt(INFO, MSGMbpAdd, lid_str);
	}

	if (libdb_attr_set_string(DB_RMM, node_id, MBP_UUID_STR, 0x0, uuid, SNAPSHOT_NEED_NOT, LOCK_ID_NULL) == -1) {
		rmm_log(ERROR, "memdb set mbp uuid attr fail\n");
		return -1;
	}
	nmap_add(uuid, node_id);

	return 0;
}

int on_cm_change(char* uuid, json_t *req, json_t *resp)
{
	int64 cm_lid = 0;
	int64 cm_uloc = 0;
	char buff[16] = {0};
	int64 ip_address = 0;
	int64 ver_high;
	int64 ver_low;
	int rc = 0;
	memdb_integer node_id = 0;

	nmap_get_node_id_by_uuid(&node_id, uuid);

	if (jrpc_get_named_param_value(req, JRPC_CM_LID, JSON_INTEGER, &cm_lid) == JSONRPC_SUCCESS) {
		if (cm_lid != 0) {
			unsigned char hw_addr[8] = {0};
			snprintf(buff, sizeof(buff), "%d", (int)cm_lid);
			rc = libdb_attr_set_string(DB_RMM, node_id, MBP_LOC_ID_STR, 0x0, buff, SNAPSHOT_NEED, LOCK_ID_NULL);
			if (rc == -1) {
				rmm_log(ERROR, "memdb set mbp loc id fail\n");
				return -1;
			}
			get_mbp_hw_addr(cm_lid, hw_addr);
			rc = libdb_attr_set_string(DB_RMM, node_id, MBP_HW_ADDR_STR,
				0x0, (char *)hw_addr, SNAPSHOT_NEED, LOCK_ID_NULL);
			if (rc == -1) {
				rmm_log(ERROR, "memdb set hw addr fail\n");
				return -1;
			}
			rc = libdb_attr_set_string(DB_RMM, node_id, MBP_MBPID_STR, 0x0, buff, SNAPSHOT_NEED, LOCK_ID_NULL);
			if (rc == -1) {
				rmm_log(ERROR, "memdb set mbp id fail\n");
				return -1;
			}
			init_mbp_attr(&node_id, cm_lid, PERSISTENT_ALL);
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_CM_ULOC, JSON_INTEGER, &cm_uloc) == JSONRPC_SUCCESS) {
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%lld", cm_uloc);
		rc = libdb_attr_set_string(DB_RMM, node_id, MBP_U_LOC_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc != 0) {
			rmm_log(ERROR, "memdb set cm ulocation fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_IPADDRESS, JSON_INTEGER, &ip_address) == JSONRPC_SUCCESS) {
		if (ip_address != 0) {
			/* save ip address */
			rc = libdb_attr_set_int(DB_RMM, node_id, MBP_IP_ADDR_STR, 0x0, ip_address, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
			if (rc == -1) {
				rmm_log(ERROR, "memdb set mbp ip addr fail\n");
				return -1;
			}
		}
	}

	if((jrpc_get_named_param_value(req, JRPC_VER_HIGH, JSON_INTEGER, &ver_high) == JSONRPC_SUCCESS) &&
			(jrpc_get_named_param_value(req, JRPC_VER_LOW, JSON_INTEGER, &ver_low) == JSONRPC_SUCCESS)) {
		set_chassis_fw_ver((uint8)ver_low, (uint8)ver_high, node_id);
	}

	return 0;
}

int on_pzone_add(char *uuid, char *parent_uuid, json_t *req, json_t *resp)
{
	memdb_integer cm_nid = 0;
	memdb_integer pz_nid = 0;
	int64 pz_lid = 0;
	int rc = 0;

	nmap_get_node_id_by_uuid(&cm_nid, parent_uuid);
	if (cm_nid == INVALID_NODE_ID) {
		rmm_log(ERROR, "get cm node id fail, uuid is %s\n", uuid);
		return -1;
	}

	if (jrpc_get_named_param_value(req, JRPC_PZ_LID, JSON_INTEGER, &pz_lid) != JSONRPC_SUCCESS)
		return -1;

	pz_nid = get_subnode_id_by_lid(pz_lid, cm_nid, MC_TYPE_PZONE);
	if (pz_nid == INVALID_NODE_ID) {
		pz_nid = libdb_create_node(DB_RMM, cm_nid,
										 MC_TYPE_PZONE, SNAPSHOT_NEED, LOCK_ID_NULL);
		rmm_log(INFO, "pzone created, id:%lu\n", pz_nid);
		char lid_str[64] = {0};
		snprintf(lid_str, sizeof(lid_str), "%lld", pz_lid);
		rf_snmp_evt(INFO, MSGPZoneAdd, lid_str);
	}

	json_object_add(resp, JRPC_NODE_ID, json_integer(pz_nid));
	rc = libdb_attr_set_string(DB_RMM, pz_nid, WRAP_UUID_STR, 0x0, uuid, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
	if (rc == -1) {
		rmm_log(ERROR, "memdb set uuid fail\n");
		return -1;
	}
	nmap_add(uuid, pz_nid);

	return 0;
}

int on_pzone_change(char *uuid, json_t *req, json_t *resp)
{
	struct node_info *pnode = NULL;
	memdb_integer cm_nid = 0;
	memdb_integer pz_nid = 0;
	int pz_lid = 0;
	int cm_lid = 0;
	int64 rc = 0;
	int64 pz_uloc = 0;
	char buff[16] = {};

	nmap_get_node_id_by_uuid(&pz_nid, uuid);
	if (pz_nid == INVALID_NODE_ID) {
		rmm_log(ERROR, "get pzone node id fail, uuid is %d\n", uuid);
		return -1;
	}

	if (jrpc_get_named_param_value(req, JRPC_PZ_LID, JSON_INTEGER, &pz_lid)!= JSONRPC_SUCCESS) {
		rmm_log(ERROR, "get pzone loc id fail, uuid is %d\n", uuid);
		return -1;
	}

	pnode = libdb_get_node_by_node_id(DB_RMM, pz_nid, LOCK_ID_NULL);
	if (pnode == NULL) {
		rmm_log(ERROR, "get pzone node id fail\n");
		return -1;
	}

	rc = libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &cm_lid, LOCK_ID_NULL);
	if (rc != 0) {
		rmm_log(ERROR, "get cm loc id fail\n");
		return rc;
	}

	init_pzone_attr(&pz_nid, pz_lid, cm_lid, PERSISTENT_ALL);

	if (jrpc_get_named_param_value(req, JRPC_PZ_ULOC, JSON_INTEGER, &pz_uloc) == JSONRPC_SUCCESS) {
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%lld", pz_uloc);
		rc = libdb_attr_set_string(DB_RMM, pz_nid, PZONE_U_LOC_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc != 0) {
			rmm_log(ERROR, "memdb set tzone ulocation fail\n");
			return -1;
		}
	}

	return 0;
}

int on_tzone_add(char *uuid, char *parent_uuid, json_t *req, json_t *resp)
{
	memdb_integer cm_nid = 0;
	memdb_integer tz_nid = 0;
	int64 tz_lid = 0;
	int rc = 0;

	nmap_get_node_id_by_uuid(&cm_nid, parent_uuid);
	if (cm_nid == INVALID_NODE_ID) {
		rmm_log(ERROR, "get cm node id fail, uuid is %s\n", uuid);
		return -1;
	}

	if (jrpc_get_named_param_value(req, JRPC_TZ_LID, JSON_INTEGER, &tz_lid) != JSONRPC_SUCCESS)
		return -1;

	tz_nid = get_subnode_id_by_lid(tz_lid, cm_nid, MC_TYPE_TZONE);
	if (tz_nid == INVALID_NODE_ID) {
		tz_nid = libdb_create_node(DB_RMM, cm_nid,
										 MC_TYPE_TZONE, SNAPSHOT_NEED, LOCK_ID_NULL);
		rmm_log(INFO, "tzone created, id:%lu\n", tz_nid);
		char lid_str[64] = {0};
		snprintf(lid_str, sizeof(lid_str), "%lld", tz_lid);
		rf_snmp_evt(INFO, MSGTZoneAdd, lid_str);
	}

	rc = libdb_attr_set_string(DB_RMM, tz_nid, WRAP_UUID_STR, 0x0, uuid, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
	if (rc != 0) {
		rmm_log(ERROR, "memdb set uuid fail\n");
		return -1;
	}
	nmap_add(uuid, tz_nid);
	return 0;
}


int on_tzone_change(char *uuid, json_t *req, json_t *resp)
{
	struct node_info *pnode = NULL;
	memdb_integer tz_nid = 0;
	int64 tz_lid = 0;
	int64 tz_uloc = 0;
	int64 rc = 0;
	char buff[16] = {};
	int cm_lid = 0;

	nmap_get_node_id_by_uuid(&tz_nid, uuid);
	if (tz_nid == INVALID_NODE_ID) {
		rmm_log(ERROR, "get tzone node id fail, uuid is %d\n", uuid);
		return -1;
	}

	if (jrpc_get_named_param_value(req, JRPC_TZ_LID, JSON_INTEGER, &tz_lid) != JSONRPC_SUCCESS) {
		rmm_log(ERROR, "get tzone node loc id fail, uuid is %d\n", uuid);
			return -1;
	}

	pnode = libdb_get_node_by_node_id(DB_RMM, tz_nid, LOCK_ID_NULL);
	if (pnode == NULL) {
		rmm_log(ERROR, "get tzone node id fail\n");
		return -1;
	}

	rc = libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &cm_lid, LOCK_ID_NULL);
	if (rc != 0) {
		rmm_log(ERROR, "get cm loc id fail\n");
		return -1;
	}

	init_tzone_attr(&tz_nid, tz_lid, cm_lid, PERSISTENT_ALL);

	if (jrpc_get_named_param_value(req, JRPC_TZ_ULOC, JSON_INTEGER, &tz_uloc) == JSONRPC_SUCCESS) {
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%lld", tz_uloc);
		rc = libdb_attr_set_string(DB_RMM, tz_nid, TZONE_U_LOC_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set tzone ulocation fail\n");
			return -1;
		}
	}

	return 0;
}

int on_dzone_add(char *uuid, char *parent_uuid, json_t *req, json_t *resp)
{
	memdb_integer cm_nid = 0;
	memdb_integer dz_nid = 0;
	int64 dz_lid = 0;
	int rc = 0;

	nmap_get_node_id_by_uuid(&cm_nid, parent_uuid);
	if (cm_nid == INVALID_NODE_ID) {
		rmm_log(ERROR, "get cm node id fail, uuid is %s\n", uuid);
		return -1;
	}

	if (jrpc_get_named_param_value(req, JRPC_DZ_LID, JSON_INTEGER, &dz_lid) != JSONRPC_SUCCESS)
		return -1;

	dz_nid = get_subnode_id_by_lid(dz_lid, cm_nid, MC_TYPE_DZONE);
	if (dz_nid == INVALID_NODE_ID) {
		dz_nid = libdb_create_node(DB_RMM, cm_nid,
										 MC_TYPE_DZONE, SNAPSHOT_NEED, LOCK_ID_NULL);
		rmm_log(INFO, "dzone created, id:%lu\n", dz_nid);
		char lid_str[64] = {0};
		snprintf(lid_str, sizeof(lid_str), "%lld", dz_lid);
		rf_snmp_evt(INFO, MSGDZoneAdd, lid_str);
	}

	rc = libdb_attr_set_string(DB_RMM, dz_nid, WRAP_UUID_STR, 0x0, uuid, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
	if (rc == -1) {
		rmm_log(ERROR, "memdb set uuid fail\n");
		return -1;
	}
	nmap_add(uuid, dz_nid);
	return 0;
}

int on_dzone_change(char *uuid, json_t *req, json_t *resp)
{
	struct node_info *pnode = NULL;
	int64 present = 0;
	int64 alert = 0;
	memdb_integer node_id = 0;
	int64 dz_lid = 0;
	int64 dz_uloc = 0;
	int cm_lid = 0;
	int rc = 0;
	char buff[128] = {0};

	nmap_get_node_id_by_uuid(&node_id, uuid);
	if (jrpc_get_named_param_value(req, JRPC_PRESENT_INFO, JSON_INTEGER, &present) == JSONRPC_SUCCESS) {
		rc = libdb_attr_set_char(DB_RMM, node_id, DRAWER_PRESENT_STR, 0x0, (char)present, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set drawer present fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_DRAWER_ALERT, JSON_INTEGER, &alert) == JSONRPC_SUCCESS) {
		rc = libdb_attr_set_char(DB_RMM, node_id, ALERT_STR, 0x0, (char)present, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set drawer alert fail\n");
			return -1;
		}
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%lld", (alert & 0xFF));
		rf_snmp_evt(INFO, MSGDrawerAlert, buff);
	}

	if (jrpc_get_named_param_value(req, JRPC_DZ_LID, JSON_INTEGER, &dz_lid) == JSONRPC_SUCCESS) {
		pnode = libdb_get_node_by_node_id(DB_RMM, node_id, LOCK_ID_NULL);
		if (pnode == NULL) {
			rmm_log(ERROR, "get dzone node id fail\n");
			return -1;
		}

		libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &cm_lid, LOCK_ID_NULL);
		init_tzone_attr(&node_id, dz_lid, cm_lid, PERSISTENT_ALL);
	}

	if (jrpc_get_named_param_value(req, JRPC_DZ_ULOC, JSON_INTEGER, &dz_uloc) == JSONRPC_SUCCESS) {
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%lld", dz_uloc);
		rc = libdb_attr_set_string(DB_RMM, node_id, DZONE_U_LOC_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set drawer ulocation fail\n");
			return -1;
		}
	}

	return 0;
}

int on_psu_add(char *uuid, char *parent_uuid, json_t *req, json_t *resp)
{
	int64 psu_lid = 0;
	memdb_integer pz_nid = 0;
	memdb_integer psu_nid = 0;
	int rc = 0;

	nmap_get_node_id_by_uuid(&pz_nid, parent_uuid);
	if (pz_nid == INVALID_NODE_ID) {
		rmm_log(ERROR, "get pzone node id fail, uuid is %s\n", uuid);
		return -1;
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_LID, JSON_INTEGER, &psu_lid) != JSONRPC_SUCCESS)
		return -1;

	psu_nid = get_subnode_id_by_lid(psu_lid, pz_nid, MC_TYPE_PSU);
	if (psu_nid == INVALID_NODE_ID) {
		psu_nid = libdb_create_node(DB_RMM, pz_nid,
										 MC_TYPE_PSU, SNAPSHOT_NEED, LOCK_ID_NULL);
		rmm_log(INFO, "psu created, id:%lu\n", psu_nid);
		char lid_str[64] = {0};
		snprintf(lid_str, sizeof(lid_str), "%lld", psu_lid);
		rf_snmp_evt(INFO, MSGPsuAdd, lid_str);
	}

	rc = libdb_attr_set_string(DB_RMM, psu_nid, WRAP_UUID_STR, 0x0, uuid, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
	if (rc == -1) {
		rmm_log(ERROR, "memdb set uuid fail\n");
		return -1;
	}
	nmap_add(uuid, psu_nid);
	return 0;
}

int on_psu_change(char *uuid, json_t *req, json_t *resp)
{
	int64 pin;
	int64 co;
	char buff[16] = {};
	char *str_ptr;
	memdb_integer node_id = 0;
	struct node_info *pnode = NULL;
	int64 psu_lid = 0;
	int64 psu_xloc = 0;
	int64 psu_uloc = 0;
	int pz_lid = 0;
	int cm_lid = 0;
	int rc = 0;

	nmap_get_node_id_by_uuid(&node_id, uuid);

	if (jrpc_get_named_param_value(req, JRPC_PSU_SERIAL, JSON_STRING, &str_ptr) == JSONRPC_SUCCESS) {
		rc = libdb_attr_set_string(DB_RMM, node_id, PSU_SER_NUM_STR, 0x0, str_ptr, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set psu serial num fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_MANUFACTURE, JSON_STRING, &str_ptr) == JSONRPC_SUCCESS) {
		rc = libdb_attr_set_string(DB_RMM, node_id, PSU_MANUFACT_STR, 0x0, str_ptr, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set psu manufacture fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_MODEL, JSON_STRING, &str_ptr) == JSONRPC_SUCCESS) {
		rc = libdb_attr_set_string(DB_RMM, node_id, PSU_MODEL_STR, 0x0, str_ptr, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set psu model fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_FW_VER, JSON_STRING, &str_ptr) == JSONRPC_SUCCESS) {
		rc = libdb_attr_set_string(DB_RMM, node_id, PSU_FW_VER_STR, 0x0, str_ptr, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set psu fw ver fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_POWER_IN, JSON_INTEGER, &pin) == JSONRPC_SUCCESS) {
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%d", (int)pin);
		rc = libdb_attr_set_string(DB_RMM, node_id, PSU_TT_PWR_IN_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set ps tt pwr in fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_CURRENT_OUT, JSON_INTEGER, &co) == JSONRPC_SUCCESS) {
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%d", (int)co);
		rc = libdb_attr_set_string(DB_RMM, node_id, PSU_TT_CURRENT_OUT_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set psu current out fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_LID, JSON_INTEGER, &psu_lid) == JSONRPC_SUCCESS) {
		pnode = libdb_get_node_by_node_id(DB_RMM, node_id, LOCK_ID_NULL);
		if (pnode == NULL) {
			rmm_log(ERROR, "get psu node fail, node id is %llu, uuid is %s\n", node_id, uuid);
			return -1;
		}
		libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &pz_lid, LOCK_ID_NULL);

		pnode = libdb_get_node_by_node_id(DB_RMM, pnode->parent, LOCK_ID_NULL);
		if (pnode == NULL) {
			rmm_log(ERROR, "get pzone node id fail\n");
			return -1;
		}
		libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &cm_lid, LOCK_ID_NULL);
		rmm_log(INFO, "initial psu attr, psu loc is %lld at cm %d\n", psu_lid, cm_lid);
		init_psu_attr(&node_id, psu_lid, pz_lid, cm_lid, PERSISTENT_ALL);
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_XLOC, JSON_INTEGER, &psu_xloc) == JSONRPC_SUCCESS) {
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%d", (int)psu_xloc);
		rc = libdb_attr_set_string(DB_RMM, node_id, PSU_X_LOC_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set psu xlocation fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_PSU_ULOC, JSON_INTEGER, &psu_uloc) == JSONRPC_SUCCESS) {
		memset(buff, 0, sizeof(buff));
		snprintf(buff, sizeof(buff), "%d", (int)psu_uloc);
		rc = libdb_attr_set_string(DB_RMM, node_id, PSU_U_LOC_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set psu ulocation fail\n");
			return -1;
		}
	}

	return 0;
}

int on_drawer_add(char *uuid, char *present_uuid, json_t *req, json_t *resp)
{
	memdb_integer dz_nid = 0;
	memdb_integer drawer_nid = 0;
	int64 drawer_lid = 0;
	int rc = 0;

	nmap_get_node_id_by_uuid(&dz_nid, present_uuid);
	if (dz_nid == INVALID_NODE_ID) {
		rmm_log(ERROR, "get dzone node id fail, uuid is %s\n", uuid);
		return -1;
	}

	if (jrpc_get_named_param_value(req, JRPC_DRAWER_LID, JSON_INTEGER, &drawer_lid) != JSONRPC_SUCCESS)
		return -1;

	drawer_nid = get_subnode_id_by_lid(drawer_lid, dz_nid, MC_TYPE_DRAWER);
	if (drawer_nid == INVALID_NODE_ID) {
		drawer_nid = libdb_create_node(DB_RMM, dz_nid,
										 MC_TYPE_DRAWER, SNAPSHOT_NEED, LOCK_ID_NULL);
		rmm_log(INFO, "drawer created, id:%lu\n", drawer_nid);
		char lid_str[64] = {0};
		snprintf(lid_str, sizeof(lid_str), "%lld", drawer_lid);
		rf_snmp_evt(INFO, MSGDrawerAdd, lid_str);
	}

	rc = libdb_attr_set_string(DB_RMM, drawer_nid, WRAP_UUID_STR, 0x0, uuid, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
	if (rc == -1) {
		rmm_log(ERROR, "memdb set uuid fail\n");
		return -1;
	}
	nmap_add(uuid, drawer_nid);
	return 0;
}

int on_drawer_change(char *uuid, json_t *req, json_t *resp)
{
	struct node_info *pnode = NULL;
	int64 temp = 0;
	int64 drawer_lid = 0;
	int dz_lid = 0;
	int cm_lid = 0;
	char temp_str[16] = {};
	jrpc_data_string ip;
	memdb_integer node_id = 0;
	int rc = 0;
	int64 drawer_uloc = 0;
	int64 drawer_xloc = 0;

	nmap_get_node_id_by_uuid(&node_id, uuid);
	if (jrpc_get_named_param_value(req, JRPC_INFO, JSON_INTEGER, &temp) == JSONRPC_SUCCESS) {
		snprintf(temp_str, sizeof(temp_str), "%d", (int)temp);
		rc = libdb_attr_set_string(DB_RMM, node_id, CHASSIS_TEMP_STR, 0x0, temp_str, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set chassis temp fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_IPADDRESS, JSON_STRING, &ip) == JSONRPC_SUCCESS) {
		rc = libdb_attr_set_string(DB_RMM, node_id, DRAWER_TMC_IP_STR, 0x0, ip, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set tmc ip fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_DRAWER_LID, JSON_INTEGER, &drawer_lid) == JSONRPC_SUCCESS) {
		pnode = libdb_get_node_by_node_id(DB_RMM, node_id, LOCK_ID_NULL);
		if (pnode == NULL) {
			rmm_log(ERROR, "get drawer node id fail, node id is %llu, uuid is %s\n", node_id, uuid);
			return -1;
		}
		libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &dz_lid, LOCK_ID_NULL);

		pnode = libdb_get_node_by_node_id(DB_RMM, pnode->parent, LOCK_ID_NULL);
		if (pnode == NULL) {
			rmm_log(ERROR, "get dzone node id fail\n");
			return -1;
		}
		libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &cm_lid, LOCK_ID_NULL);
		rmm_log(INFO, "initial drawer attr, drawer loc is %lld at cm %d\n", drawer_lid, cm_lid);
		init_drawer_attr(&node_id, drawer_lid, dz_lid, cm_lid, PERSISTENT_ALL);
	}

	if (jrpc_get_named_param_value(req, JRPC_DRAWER_ULOC, JSON_INTEGER, &drawer_uloc) == JSONRPC_SUCCESS) {
		memset(temp_str, 0, sizeof(temp_str));
		snprintf(temp_str, sizeof(temp_str), "%lld", drawer_uloc);
		rc = libdb_attr_set_string(DB_RMM, node_id, DRAWER_U_LOC_STR, 0x0, temp_str, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set drawer ulocation fail\n");
			return -1;
		}
	}

	return 0;
}

int on_fan_add(char *uuid, char *present_uuid, json_t *req, json_t *resp)
{
	memdb_integer tz_nid = 0;
	memdb_integer fan_nid = 0;
	int64 fan_lid = 0;
	int rc = 0;

	nmap_get_node_id_by_uuid(&tz_nid, present_uuid);
	if (tz_nid == INVALID_NODE_ID) {
		rmm_log(ERROR, "get tz node id fail, uuid is %s\n", uuid);
		return -1;
	}

	if (jrpc_get_named_param_value(req, JRPC_FAN_LID, JSON_INTEGER, &fan_lid) != JSONRPC_SUCCESS)
		return -1;

	fan_nid = get_subnode_id_by_lid(fan_lid, tz_nid, MC_TYPE_FAN);
	if (fan_nid == INVALID_NODE_ID) {
		fan_nid = libdb_create_node(DB_RMM, tz_nid,
										 MC_TYPE_FAN, SNAPSHOT_NEED, LOCK_ID_NULL);
		rmm_log(INFO, "fan created, id:%lu\n", fan_nid);
		char lid_str[64] = {0};
		snprintf(lid_str, sizeof(lid_str), "%lld", fan_lid);
		rf_snmp_evt(INFO, MSGFanAdd, lid_str);
	}

	rc = libdb_attr_set_string(DB_RMM, fan_nid, WRAP_UUID_STR, 0x0, uuid, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
	if (rc == -1) {
		rmm_log(ERROR, "memdb set uuid fail\n");
		return -1;
	}

	nmap_add(uuid, fan_nid);
	return 0;
}

int on_fan_change(char *uuid, json_t *req, json_t *resp)
{
	struct node_info *pnode = NULL;
	int64 speed = 0;
	char string[64] = {0};
	memdb_integer node_id = 0;
	int64 fan_lid = 0;
	int64 fan_uloc = 0;
	int64 fan_xloc = 0;
	int tz_lid = 0;
	int cm_lid = 0;
	int rc = 0;

	nmap_get_node_id_by_uuid(&node_id, uuid);
	if (jrpc_get_named_param_value(req, JRPC_INFO, JSON_INTEGER, &speed) == JSONRPC_SUCCESS) {
		snprintf(string, sizeof(string), "%d", (int)speed);
		rc = libdb_attr_set_string(DB_RMM, node_id, FAN_TACH_READ_STR, 0x0, string, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc == -1) {
			rmm_log(ERROR, "memdb set fan tach fail\n");
			return -1;
		}
		json_object_add(resp, JRPC_NODE_ID, json_integer(node_id));
	}

	if (jrpc_get_named_param_value(req, JRPC_FAN_LID, JSON_INTEGER, &fan_lid) == JSONRPC_SUCCESS) {
		pnode = libdb_get_node_by_node_id(DB_RMM, node_id, LOCK_ID_NULL);
		if (pnode == NULL) {
			rmm_log(ERROR, "get fan node id fail, node id is %llu, uuid is %s\n", node_id, uuid);
			return -1;
		}
		libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &tz_lid, LOCK_ID_NULL);

		pnode = libdb_get_node_by_node_id(DB_RMM, pnode->parent, LOCK_ID_NULL);
		if (pnode == NULL) {
			rmm_log(ERROR, "get tzone node id fail\n");
			return -1;
		}
		libdb_attr_get_int(DB_RMM, pnode->parent, MBP_LOC_ID_STR, &cm_lid, LOCK_ID_NULL);
		rmm_log(INFO, "initial fan attr, fan loc is %lld at cm %d\n", fan_lid, cm_lid);
		init_fan_attr(&node_id, fan_lid, tz_lid, cm_lid, PERSISTENT_ALL);
	}

	if (jrpc_get_named_param_value(req, JRPC_FAN_ULOC, JSON_INTEGER, &fan_uloc) == JSONRPC_SUCCESS) {
		memset(string, 0, sizeof(string));
		snprintf(string, sizeof(string), "%lld", fan_uloc);
		rc = libdb_attr_set_string(DB_RMM, node_id, FAN_U_LOC_STR, 0x0, string, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc != 0) {
			rmm_log(ERROR, "memdb set fan ulocation fail\n");
			return -1;
		}
	}

	if (jrpc_get_named_param_value(req, JRPC_FAN_XLOC, JSON_INTEGER, &fan_xloc) == JSONRPC_SUCCESS) {
		memset(string, 0, sizeof(string));
		snprintf(string, sizeof(string), "%lld", fan_xloc);
		rc = libdb_attr_set_string(DB_RMM, node_id, FAN_X_LOC_STR, 0x0, string, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
		if (rc != 0) {
			rmm_log(ERROR, "memdb set fan xlocation fail\n");
			return -1;
		}
	}

	return 0;
}

cmd_func_map_t cmd_func_maps[] = {
	{ CM_TYPE,		on_cm_add,		on_cm_change },
	{ PZ_TYPE,		on_pzone_add,	on_pzone_change },
	{ TZ_TYPE,		on_tzone_add,	on_tzone_change },
	{ DZ_TYPE,		on_dzone_add,	on_dzone_change },
	{ PSU_TYPE,		on_psu_add,		on_psu_change},
	{ FAN_TYPE,		on_fan_add,		on_fan_change},
	{ DRAWER_TYPE,	on_drawer_add,	on_drawer_change}
};


cmd_func_map_t *find_cmd_func(int type)
{
	int index = 0;

	for (index = 0; index < sizeof(cmd_func_maps) / sizeof(cmd_func_map_t); index++) {
		if (cmd_func_maps[index].type == type)
			return &cmd_func_maps[index];
	}

	return NULL;
}

#if 0
static int set_id_field_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	memdb_integer * node_id = (memdb_integer*)cb_data;
	char buff[128] = {0};
	char tmp[8] = {0};
	int i = 0;

	if(result == -1 || rsp[0] != IPMI_CC_OK) {
		return -1;
	}

	return 0;
}

static int set_id_field(jrpc_req_pkg_t *req, json_t *resp)
{
	struct jipmi_msg ipmi_req = {};
	int host;
	int i;
	int64 error_code = 0;
	memdb_integer node_id;
	int field_type;
	int field_instance;
	int byte_num;
	json_t *array;

	jrpc_get_named_param_value(req->json, JRPC_NODE_ID, JSON_INTEGER, &node_id);
	jrpc_get_named_param_value(req->json, JRPC_FIELD_TYPE, JSON_INTEGER, &field_type);
	jrpc_get_named_param_value(req->json, JRPC_FIELD_INSTANCE, JSON_INTEGER, &field_instance);
	jrpc_get_named_param_value(req->json, JRPC_BYTE_NUM, JSON_INTEGER, &byte_num);
	jrpc_get_named_param_value(req->json, JRPC_DATA, JSON_ARRAY, (void *)&array);

	ipmi_req.data[0] = field_type;
	ipmi_req.data[1] = field_instance;
	ipmi_req.data[2] = byte_num;

	for (i = 0; i < byte_num; i++) {
		json_t * data = json_array_get(array, i);
		if (-1 == (ipmi_req.data[i + 3] = json_integer_value(data)))
			continue;
	}

	FILL_INT(ipmi_req.netfn,	IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		SET_ID_FIELD_CMD);
	FILL_INT(ipmi_req.data_len,	byte_num + 2);

	error_code = libdb_attr_get_int(DB_RMM, node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}
	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &ipmi_req, set_id_field_cb, &node_id, JIPMI_NON_SYNC);

	return 0;
}

static int get_id_field_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	memdb_integer * node_id = (memdb_integer*)cb_data;
	char buff[128] = {0};
	char tmp[8] = {0};
	int i = 0;

	if(result == -1 || rsp[0] != IPMI_CC_OK) {
		return -1;
	}

	return 0;
}


static int get_id_field(jrpc_req_pkg_t *req, json_t *resp)
{
	struct jipmi_msg ipmi_req = {};
	int host;
	int i;
	int64 error_code = 0;
	memdb_integer node_id;
	int field_instance;

	jrpc_get_named_param_value(req->json, JRPC_NODE_ID, JSON_INTEGER, &node_id);
	jrpc_get_named_param_value(req->json, JRPC_FIELD_INSTANCE, JSON_INTEGER, &field_instance);

	ipmi_req.data[0] = field_instance;

	FILL_INT(ipmi_req.netfn,	IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		GET_ID_FIELD_CMD);
	FILL_INT(ipmi_req.data_len,	1);

	error_code = libdb_attr_get_int(DB_RMM, node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}
	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &ipmi_req, get_id_field_cb, &node_id, JIPMI_NON_SYNC);

	return 0;
}

static int get_thermal_sensor_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	memdb_integer * node_id = (memdb_integer*)cb_data;
	char buff[128] = {0};
	char tmp[8] = {0};
	int i = 0;

	if(result == -1 || rsp[0] != IPMI_CC_OK) {
		return -1;
	}

	return 0;
}


static int get_thermal_sensor(jrpc_req_pkg_t *req, json_t *resp)
{
	struct jipmi_msg ipmi_req = {};
	int host;
	int i;
	int64 error_code = 0;
	memdb_integer node_id;
	int field_instance;

	jrpc_get_named_param_value(req->json, JRPC_NODE_ID, JSON_INTEGER, &node_id);

	FILL_INT(ipmi_req.netfn,	IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		GET_ID_FIELD_CMD);
	FILL_INT(ipmi_req.data_len,	0);

	error_code = libdb_attr_get_int(DB_RMM, node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}
	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &ipmi_req, get_thermal_sensor_cb, &node_id, JIPMI_NON_SYNC);

	return 0;
}

static int get_health_sensor_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	memdb_integer * node_id = (memdb_integer*)cb_data;
	char buff[128] = {0};
	char tmp[8] = {0};
	int i = 0;

	if(result == -1 || rsp[0] != IPMI_CC_OK) {
		return -1;
	}

	return 0;
}


static int get_health_sensor(jrpc_req_pkg_t *req, json_t *resp)
{
	struct jipmi_msg ipmi_req = {};
	int host;
	int i;
	int64 error_code = 0;
	memdb_integer node_id;
	int field_instance;

	jrpc_get_named_param_value(req->json, JRPC_NODE_ID, JSON_INTEGER, &node_id);

	FILL_INT(ipmi_req.netfn,	IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		GET_ID_FIELD_CMD);
	FILL_INT(ipmi_req.data_len,	0);

	error_code = libdb_attr_get_int(DB_RMM, node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}
	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &ipmi_req, get_health_sensor_cb, &node_id, JIPMI_NON_SYNC);

	return 0;
}


static int get_tray_power_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	memdb_integer * node_id = (memdb_integer*)cb_data;
	char buff[128] = {0};
	char tmp[8] = {0};
	int i = 0;

	if(result == -1 || rsp[0] != IPMI_CC_OK) {
		return -1;
	}

	return 0;
}


static int get_tray_power(jrpc_req_pkg_t *req, json_t *resp)
{
	struct jipmi_msg ipmi_req = {};
	int host;
	int i;
	int64 error_code = 0;
	memdb_integer node_id;
	int field_instance;

	jrpc_get_named_param_value(req->json, JRPC_NODE_ID, JSON_INTEGER, &node_id);

	FILL_INT(ipmi_req.netfn,	IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		GET_ID_FIELD_CMD);
	FILL_INT(ipmi_req.data_len,	0);

	error_code = libdb_attr_get_int(DB_RMM, node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}
	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &ipmi_req, get_tray_power_cb, &node_id, JIPMI_NON_SYNC);

	return 0;
}


static int get_fan_pwd_by_node_id_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	char buff[32] = {0};
	memdb_integer* fan_node_id = (memdb_integer*)cb_data;

	if(result == -1 || rsp[0] != IPMI_CC_OK) {
		return -1;
	}

	snprintf(buff, sizeof(buff), "%d   RPM", rsp[1]);
	libdb_attr_set_string(DB_RMM, *fan_node_id, FAN_DESIRED_SPD_PWM_STR, 0x0, buff, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);

	return 0;

}


static int get_fan_pwm_by_id(jrpc_req_pkg_t *req, json_t *resp)
{
	struct jipmi_msg ipmi_req = {};
	int  host;
	int64 error_code = 0;
	memdb_integer cm_node_id;
	memdb_integer fan_node_id;
	int fan_idx;
	
	jrpc_get_named_param_value(req->json, JRPC_CM_NODE_ID, JSON_INTEGER, &cm_node_id);
	jrpc_get_named_param_value(req->json, JRPC_FAN_NODE_ID, JSON_INTEGER, &fan_node_id);
	jrpc_get_named_param_value(req->json, JRPC_FAN_INDEX, JSON_INTEGER, &fan_idx);

	ipmi_req.data[0] = fan_idx - 1;

	FILL_INT(ipmi_req.netfn,	IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		GET_FAN_PWM_CMD);
	FILL_INT(ipmi_req.data_len,	1);

	error_code = libdb_attr_get_int(DB_RMM, cm_node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}	
	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &ipmi_req, get_fan_pwd_by_node_id_cb, &fan_node_id, JIPMI_NON_SYNC);

	return 0;
}

static int set_fan_pwm_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	if(result == -1 || rsp[0] != IPMI_CC_OK) {
		return -1;
	}
	
	return 0;
}

static int set_fan_pwm(jrpc_req_pkg_t *req, json_t *resp)
{
	struct jipmi_msg ipmi_req = {};
	memdb_integer cm_node_id;
	int  host;
	int64 error_code = 0;
	int tzone_idx;
	int fan_idx;
	int pwm;
	
	jrpc_get_named_param_value(req->json, JRPC_TZ_INDEX, JSON_INTEGER, &tzone_idx);
	jrpc_get_named_param_value(req->json, JRPC_FAN_INDEX, JSON_INTEGER, &fan_idx);
	jrpc_get_named_param_value(req->json, JRPC_PWM, JSON_INTEGER, &pwm);

	ipmi_req.data[0] = fan_idx - 1;
	ipmi_req.data[1] = pwm;

	FILL_INT(ipmi_req.netfn,	IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		SET_DEFAULT_PWM_CMD);
	FILL_INT(ipmi_req.data_len,	2);

	cm_node_id = get_subnode_id_by_idx(tzone_idx, MC_TYPE_RMC, MC_TYPE_CM);
	error_code = libdb_attr_get_int(DB_RMM, cm_node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}

	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &ipmi_req, set_fan_pwm_cb, NULL, JIPMI_NON_SYNC);

	return RESULT_OK;
}


static int serial_inf_close_rsp_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	return 0;
}


static int set_psu_by_node_id_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	int i;
	
	printf("set psu by node id RSP RCV'D: ");
	for (i = 0; i < rsp_len; i++) {
		printf("%02X ", rsp[i]);
	}
	printf("\n");
	return 0;
}

static result_t get_psu_node_id_by_idx(int pzone_idx, int psu_idx, memdb_integer *node_id)
{
	memdb_integer cm_node_id;
	memdb_integer pzone_node_id;
	memdb_integer psu_node_id;
	
	cm_node_id = get_subnode_id_by_idx(pzone_idx, MC_TYPE_RMC,MC_TYPE_CM);
	if(cm_node_id == 0) {
		return RESULT_NO_NODE;
	}

	pzone_node_id = get_subnode_id_by_idx(pzone_idx, cm_node_id, MC_TYPE_PZONE);
	if(pzone_node_id == 0) {
		return RESULT_NO_NODE;
	}

	psu_node_id = get_subnode_id_by_idx(psu_idx,pzone_node_id,MC_TYPE_PSU);
	if(psu_node_id == 0) {
		return RESULT_NO_NODE;
	}

	*node_id = psu_node_id;
	return RESULT_OK;
}


static int session_close_rsp_handler(int result, unsigned char *rsp, int rsp_len,
										  void *cb_data)
{
	struct reset_param *param = (struct reset_param *)cb_data;
	
	if(rsp[0] == IPMI_CC_OK) {
		struct jipmi_serial_msg req = {};
		char cm_dev[64] = {0};
		int64 error_code = 0;

		req.serial_flag = IPMI_SERIAL_CLOSE_DEV;
		FILL_INT(req.msg_base.netfn,	OEM_IPMI_NETFN);
		FILL_INT(req.msg_base.cmd,		OEM_SERIAL_CLOSE_CMD);
		
		snprintf(cm_dev, sizeof(cm_dev), "/dev/ttyCm%dIPMI", param->cm_idx);
		memcpy(req.msg_base.data, cm_dev, strlen(cm_dev));
		FILL_INT(req.msg_base.data_len,	strlen(cm_dev));
		
		libjipmi_serial_cmd(&req, serial_inf_close_rsp_cb, NULL, JIPMI_NON_SYNC);
		free(param);
		return 0;
	}else {
		free(param);
		return -1;
	}
}

static int peripheral_reset_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	int host;
	struct reset_param *param = (struct reset_param *)cb_data;
	int64 error_code = 0;
	
	if(rsp[0] == IPMI_CC_OK) {
		libjipmi_rmcp_close_session(param->host, IPMI_RMCP_PORT, session_close_rsp_handler, param, JIPMI_NON_SYNC);
		return 0;
	}
	else {
		return -1;
	}
}

static int peripheral_hard_reset(jrpc_req_pkg_t *req, json_t *resp)
{
	int64 error_code = 0;
	struct jipmi_msg ipmi_req = {};
	int target;
	int32 cm_idx;
	int32 peripheral_id;
	
	jrpc_get_named_param_value(req->json, JRPC_CM_INDEX, JSON_INTEGER, &cm_idx);
	jrpc_get_named_param_value(req->json, JRPC_PERI_ID, JSON_INTEGER, &peripheral_id);

	struct reset_param *param = (struct reset_param *)malloc(sizeof(struct reset_param));
	if(param == NULL)
		return RESULT_MALLOC_ERR;
	memset(param, 0, sizeof(struct reset_param));

	memdb_integer cm_node_id = get_subnode_id_by_idx(cm_idx, MC_TYPE_RMC, MC_TYPE_CM);
	if(cm_node_id == 0) {
		return RESULT_NO_NODE;
	}

	error_code = libdb_attr_get_int(DB_RMM, cm_node_id, MBP_IP_ADDR_STR, &target, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}
	param->host = target;
	param->cm_idx = cm_idx;
	
	ipmi_req.data[0] = peripheral_id;

	FILL_INT(ipmi_req.netfn,		IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		CHASSIS_RESET_CMD);
	FILL_INT(ipmi_req.data_len,	1);

	libjipmi_rmcp_cmd(target, IPMI_RMCP_PORT, &ipmi_req, peripheral_reset_cb, param, JIPMI_NON_SYNC);

	return RESULT_OK;

}


static int post_psu_by_node_id(jrpc_req_pkg_t *req, json_t *resp)
{
	struct jipmi_msg ipmi_req = {};
	unsigned char opt_code;
	memdb_integer cm_node_id;
	int  host;
	int64 error_code = 0;
	int pzone_idx;
	int psu_idx;
	int request_enabled_state;

	jrpc_get_named_param_value(req->json, JRPC_PZ_INDEX, JSON_INTEGER, &pzone_idx);
	jrpc_get_named_param_value(req->json, JRPC_PSU_INDEX, JSON_INTEGER, &psu_idx);
	jrpc_get_named_param_value(req->json, JRPC_PSU_ENABLE_STATE, JSON_INTEGER, &request_enabled_state);

	if((pzone_idx != LOWER_POWER_ZONE_ID) && (pzone_idx != UPPER_POWER_ZONE_IP)) {
		printf("invalid zone id\n");
		return WRAP_RET_INPUT_ERR;
	}

	if (request_enabled_state == PSU_STATE_ENABLED)
		opt_code = 0x80;
	else if (request_enabled_state == PSU_STATE_DISABLED)
		opt_code = 0x00;
	else
		opt_code = 0x80;

	ipmi_req.data[0] = psu_idx - 1;
	ipmi_req.data[1] = PSU_WRITE_BYTE;
	ipmi_req.data[2] = 0x01;/* operation code for psu pmbus */
	ipmi_req.data[3] = BYTE_LEN;
	ipmi_req.data[4] = opt_code;

	FILL_INT(ipmi_req.netfn,	IPMI_CM_NETFN);
	FILL_INT(ipmi_req.cmd,		SEND_PSU_CMD);
	FILL_INT(ipmi_req.data_len,	5);

	cm_node_id = get_subnode_id_by_idx(pzone_idx, MC_TYPE_RMC, MC_TYPE_CM);
	error_code = libdb_attr_get_int(DB_RMM, cm_node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}

	memdb_integer psu_node_id = 0;
	get_psu_node_id_by_idx(pzone_idx, psu_idx, &psu_node_id);
	libdb_attr_set_int(DB_RMM, psu_node_id, PSU_ENABLE_ST_STR,
						  0x0, request_enabled_state, SNAPSHOT_NEED, LOCK_ID_NULL);

	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &ipmi_req, set_psu_by_node_id_cb, NULL, JIPMI_NON_SYNC);

	return RESULT_OK;
}
#endif


int on_add(jrpc_req_pkg_t *req, json_t *resp)
{
	int64 type;
	int64 pos;
	char *uuid = NULL;
	char *parent_uuid = NULL;

	jrpc_get_named_param_value(req->json, JRPC_EVT_TYPE, JSON_INTEGER, &type);
	jrpc_get_named_param_value(req->json, JRPC_UUID, JSON_STRING, &uuid);
	if(jrpc_get_named_param_value(req->json, JRPC_PARENT_UUID, JSON_STRING, &parent_uuid) == JSONRPC_FAILED)
		 parent_uuid = NULL;

	cmd_func_map_t *cmd_func_entity = find_cmd_func(type);

	if (cmd_func_entity != NULL)
		cmd_func_entity->on_add(uuid, parent_uuid, req->json, resp);

	return 0;
}

int on_remove(jrpc_req_pkg_t *req, json_t *resp)
{
	char *uuid;

	jrpc_get_named_param_value(req->json, JRPC_UUID, JSON_STRING, &uuid);

	if (strlen(uuid) == 0)
		return -1;

	ad_remove_node(uuid);

	return 0;
}

int on_change(jrpc_req_pkg_t *req, json_t *resp)
{
	int64 type = 0;
	char * uuid = NULL;
	int cm_idx = 0;
	cmd_func_map_t *cmd_func_entity = NULL;

	jrpc_get_named_param_value(req->json, JRPC_UUID, JSON_STRING, &uuid);
	/*printf("%s node_id: %lld\n", __func__, node_id);*/
	jrpc_get_named_param_value(req->json, JRPC_EVT_TYPE, JSON_INTEGER, &type);
	/*printf("%s type: %lld\n", __func__, type);*/

	cmd_func_entity = find_cmd_func(type);
	if (cmd_func_entity != NULL)
		cmd_func_entity->on_change(uuid, req->json, resp);

	return 0;
}

int process_req(int func_id, jrpc_req_pkg_t *req, json_t *resp)
{
	return cmd_handles[func_id](req, resp);
}

static void reset_zone(memdb_integer node_id, int cm_lid, reset_zone_fn zone_fn, reset_item_fn item_fn, int sub_type)
{
	int i = 0;
	int lid = 0;
	struct node_info *subnode = NULL;
	int num = 0;
	int sub_idx = 0;

	libdb_attr_get_int(DB_RMM, node_id, WRAP_LOC_ID_STR, &lid, LOCK_ID_NULL);
	zone_fn(&node_id, cm_lid, lid);
	subnode = libdb_list_subnode_by_type(DB_RMM, node_id, sub_type, &num, NULL, LOCK_ID_NULL);
	for (i = 0; i < num; i ++) {
		sub_idx = 0;
		libdb_attr_get_int(DB_RMM, subnode[i].node_id, WRAP_LOC_ID_STR, &sub_idx, LOCK_ID_NULL);
		item_fn(&(subnode[i].node_id), cm_lid, sub_idx, lid);
	}
}

static void reset_zones(memdb_integer node_id, reset_zone_fn zone_fn, int zone_type, reset_item_fn item_fn, int sub_type)
{
	int i = 0;
	int lid = 0;
	struct node_info *subnode = NULL;
	int num = 0;
	struct node_info *zone_node = NULL;

	subnode = libdb_list_subnode_by_type(DB_RMM, node_id, zone_type, &num, NULL,LOCK_ID_NULL);
	if (subnode) {
		zone_node = (struct node_info *)malloc(num * sizeof(struct node_info));
		if (zone_node == NULL)
			return;

		memcpy(zone_node, subnode, num * sizeof(struct node_info));
		libdb_attr_get_int(DB_RMM, node_id, WRAP_LOC_ID_STR, &lid, LOCK_ID_NULL);
		for (i = 0; i < num; i++)
			reset_zone(zone_node[i].node_id, lid, zone_fn, item_fn, sub_type);
		free(zone_node);
		zone_node = NULL;
	}
}

int rmm_factory_reset(jrpc_req_pkg_t *req, json_t *resp)
{
	int i = 0; 
	int j = 0;
	struct node_info *subnode = NULL;

	int mbp_num = 0;
	int mbp_lid = 0;
	struct node_info *mbp_nodes = NULL;

	reset_rack_attr();
	subnode = libdb_list_subnode_by_type(DB_RMM, MC_TYPE_RMC, MC_TYPE_CM, &mbp_num, NULL, LOCK_ID_NULL);
	if(!subnode)
		return 0;

	mbp_nodes = (struct node_info *)malloc(sizeof(struct node_info) * mbp_num);
	if (mbp_nodes == NULL)
		return 0;
	memcpy(mbp_nodes, subnode, sizeof(struct node_info) * mbp_num);

	for (i = 0; i < mbp_num; i++) {
		libdb_attr_get_int(DB_RMM, mbp_nodes[i].node_id, WRAP_LOC_ID_STR, &mbp_lid, LOCK_ID_NULL);;
		reset_mbp_attr(&(mbp_nodes[i].node_id), mbp_lid);

		reset_zones(mbp_nodes[i].node_id, reset_dzone_attr, MC_TYPE_DZONE, reset_drawer_attr, MC_TYPE_DRAWER);
		reset_zones(mbp_nodes[i].node_id, reset_pzone_attr, MC_TYPE_PZONE, reset_psu_attr, MC_TYPE_PSU);
		reset_zones(mbp_nodes[i].node_id, reset_tzone_attr, MC_TYPE_TZONE, reset_fan_attr, MC_TYPE_FAN);
	}
	free(mbp_nodes);
	return 0;
}
