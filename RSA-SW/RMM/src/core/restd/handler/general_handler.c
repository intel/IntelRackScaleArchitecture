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


#include "handler.h"
#include "libredfish/rf_event.h"
#include "librmmcfg/rmm_cfg.h"
#include "libutils/time.h"
#include "libutils/ip.h"
#include "libutils/string.h"
#include "libredfish/snmp_event.h"


typedef struct type_to_key {
	int32 type;
	int8 id_key[RMM_NAME_LEN];
	int8 uuid_key[RMM_NAME_LEN];
} type_to_key_t;

const type_to_key_t type_key[] = {
	{MC_TYPE_RMC, RACK_LOC_ID_STR, RACK_UUID_STR},
	{MC_TYPE_CM, MBP_LOC_ID_STR, MBP_UUID_STR},
	{MC_TYPE_DZONE, "", ""},
	{MC_TYPE_DRAWER, DRAWER_LOC_ID_STR, DRAWER_UUID_STR},
	{MC_TYPE_BMC, "", ""},
	{MC_TYPE_PZONE, PZONE_LOC_ID_STR, PZONE_UUID_STR},
	{MC_TYPE_PSU, PSU_LOC_ID_STR, PSU_UUID_STR},
	{MC_TYPE_TZONE, TZONE_LOC_ID_STR, TZONE_UUID_STR},
	{MC_TYPE_FAN, FAN_LOC_ID_STR, FAN_UUID_STR},
	{MC_TYPE_SENSOR, "", ""},
};

const id_evt_type_t nodeid_keys[] = {
	{MC_REDFISH_CHANGE, RF_EVENT_TYPE_CHANGE_STR},
	{MC_REDFISH_UPDATE, RF_EVENT_TYPE_UPDATE_STR},
	{MC_REDFISH_ADD,    RF_EVENT_TYPE_ADD_STR},
	{MC_REDFISH_REMOVE, RF_EVENT_TYPE_DELETE_STR},
	{MC_REDFISH_ALERT,  RF_EVENT_TYPE_ALERT_STR},
};

static int32 gen_asset_index(const int32 type, int32 loc_id,  memdb_integer node_id)
{
	int8 *name;
	struct node_info *pnode = NULL;
	int64 error_code = 0;
	int32 zone_num = 0;
	int32 cm_lid = 0;
	int32 index = 0;
	int8 rs[128] = {0};

	switch (type) {
	case MC_TYPE_PZONE:
		zone_num = MAX_PZONE_NUM;
		break;
	case MC_TYPE_TZONE:
		zone_num = MAX_TZONE_NUM;
		break;
	case MC_TYPE_DZONE:
		zone_num = MAX_DZONE_NUM;
		break;
	default:
		return loc_id;
	}

	pnode = libdb_get_node_by_node_id(DB_RMM, node_id, LOCK_ID_NULL);
	if (pnode == NULL) {
		 return -1;
	}

	memset(rs, 0, 128);
	error_code = 0;
	error_code = libdb_attr_get_string(DB_RMM, pnode->parent, MBP_LOC_ID_STR, rs, 128, LOCK_ID_NULL);
	if (error_code != 0) {
		rmm_log(ERROR, "%s:%d: error code:%d\n", __func__, __LINE__, error_code);
		return -1;
	}
	cm_lid = str2int(rs);

	index = (cm_lid - 1) * zone_num + loc_id;

	return index;
}

static bool get_index_by_uuid(const int8 *uuid, const int32 type, int32 *index)
{
	struct node_info *subnode = NULL;
	int32 subnode_num = 0;
	int32 i;
	int8 rs[128] = {0};
	int64 error_code = 0;
	int32 loc_id = 0;

	subnode = libdb_list_node_by_type(DB_RMM, (uint32)type, (uint32)type, &subnode_num, NULL, LOCK_ID_NULL);

	for (i = 0; i < subnode_num; i++) {
		memset(rs, 0, 128);
		error_code = 0;
		error_code = libdb_attr_get_string(DB_RMM, subnode[i].node_id, (int8 *)type_key[type].uuid_key, rs, 128, LOCK_ID_NULL);
		if (error_code != 0) {
			rmm_log(ERROR, "%s:%d: error code:%d\n", __func__, __LINE__, error_code);
			libdb_free_node(subnode);
			return FALSE;
		} else {
			if (0 == strcmp(uuid, rs)) {
				memset(rs, 0, 128);
				error_code = 0;
				error_code = libdb_attr_get_string(DB_RMM, subnode[i].node_id, (int8 *)type_key[type].id_key, rs, 128, LOCK_ID_NULL);
				if (error_code != 0) {
					rmm_log(ERROR, "%s:%d: error code:%d\n", __func__, __LINE__, error_code);
					libdb_free_node(subnode);
					return FALSE;
				} else if (index != NULL) {
					loc_id = str2int(rs);
					*index = gen_asset_index(type, loc_id, subnode[i].node_id);
					libdb_free_node(subnode);
					return TRUE;
				}
			}
		}
	}

	libdb_free_node(subnode);
	return FALSE;
}

int32 get_asset_idx(const struct rest_uri_param *param, const int8 *name, int32 type)
{
	int8 *p_id = NULL;
	int32 index;

	p_id = rest_path_value(param, name);
	if (p_id == NULL) {
		HTTPD_ERR("get value fail\n");
		return -1;
	}

	if (TRUE == is_str_uuid((const int8 *)p_id)) {
		if (FALSE == get_index_by_uuid((const int8 *)p_id, type, &index)) {
			HTTPD_ERR("uuid %s error!\n", p_id);
			return -1;
		}
	} else
		index = (uint32)str2int(p_id);

	return index;
}

memdb_integer get_asset_node_id(const struct rest_uri_param *param, const int8 *name)
{
	int8 *p_uuid = NULL;
	uint8 uuid[64] = {0};
	memdb_integer node_id;

	p_uuid = rest_path_value(param, name);
	if (p_uuid == NULL) {
		HTTPD_ERR("get value from path fail\n");
		return 0;
	}

	snprintf((int8 *)uuid, sizeof(uuid), "%s", p_uuid);
	libwrap_get_node_id_by_uuid(uuid, &node_id);

	return node_id;
}


int32 prepare_get_action(const avail_action_t *av_action, json_t *action, int32 index)
{
	if (action == NULL) {
		HTTPD_ERR("action json array request fail\n");
		return -1;
	}
	if (0 != strcmp((int8 *)(av_action[index].action), ""))
		add_json_string(action, RMM_JSON_ACTION, av_action[index].action);

	return 0;
}


void put_prepare_str(const json_t *req, uint8 *output, int32 len, const int8 *name)
{
	json_t *rs_json = NULL;
	int8 *input = NULL;

	rs_json = json_object_get(req, name);
	if (rs_json != NULL) {
		input = json_string_value(rs_json);
		if(input == NULL) {
			HTTPD_ERR("json string get failed.\n");
			return;
		}
		if (TRUE == check_str_len(input, len))
			strncpy_safe((int8 *)output, input, len, len - 1);
	}
}

void put_prepare_int(const json_t *elem, uint32 *output, const int8 *name)
{
	uint32 value;
	json_t *obj = NULL;

	obj = json_object_get(elem, name);
	if (obj != NULL) {
		value = json_integer_value(obj);
		if (TRUE == check_int_range(value, 0, FAN_THRESH_MAX))
			*output = value;
	}
}

void add_loc_info(json_t *loc, int32 input, const int8 *name)
{
	json_t *rs_json = NULL;

	rs_json = json_integer((int64)input);
	if (rs_json)
		json_object_add(loc, name, rs_json);
}

int32 pack_basic_element_info(const base_element_t *be, json_t *result)
{
	add_json_integer(result, RMM_JSON_ID, be->id);
	add_json_string(result, RMM_JSON_UUID, be->uuid);
	add_json_string(result, RMM_JSON_NAME, be->name);
	add_json_string(result, RMM_JSON_DESC, be->desc);
	add_json_string(result, RMM_JSON_CREATED_DATE, be->create_date);
	add_json_string(result, RMM_JSON_UPDATED_DATE, be->update_date);

	return 0;
}

int32 pack_fru_info(const fru_info_t *fru_info, json_t *fru)
{
	add_json_string(fru, RMM_JSON_SERIAL_NUM, fru_info->serial_num);
	add_json_string(fru, RMM_JSON_MANUFACTURE, fru_info->manufacture);
	add_json_string(fru, RMM_JSON_MODEL_NUM, fru_info->model);
	add_json_string(fru, RMM_JSON_PART_NUM, fru_info->part_num);
	add_json_string(fru, RMM_JSON_FIRMWARE_VER, fru_info->fw_ver);

	return 0;
}

int32 get_rf_base_info(rf_base_t *base)
{
	libutils_get_time("modified", base->modify_date);

	snprintf(base->oem, sizeof(base->oem), "Intel.1.0.1");
	snprintf(base->name, sizeof(base->name), "RSA_RMM");
	snprintf(base->type, sizeof(base->type), "RMM1.0.0");
	snprintf(base->description, sizeof(base->description), "For internal use only");

	return 0;
}

int32 pack_rf_base_json(json_t *result, const rf_base_t *base)
{
	add_json_string(result, RMM_JSON_OEM, (uint8 *)base->oem);
	add_json_string(result, RMM_JSON_DESC, (uint8 *)base->name);
	add_json_string(result, RMM_JSON_MODIFY, (uint8 *)base->modify_date);
	add_json_string(result, RMM_JSON_TYPE, (uint8 *)base->type);
	add_json_string(result, RMM_JSON_DESC, (uint8 *)base->description);

	return 0;
}


int32 subscribe_event(listener_dest_t *listener, int8 *evt_str, int32 type)
{
	memdb_integer evt_nid = 0;
	memdb_integer listener_nid = 0;
	int32 mask = 0;

	listener_nid = libwrap_get_listener(listener->dest, evt_str, &evt_nid, type);
	if (listener_nid == -1) {
		HTTPD_ERR("get redfish event listener error, type is %d\n", type);
		return -1;
	}

	return libwrap_subscribe_listener(listener, listener_nid, evt_nid);
}

int32 unsubscribe_event(listener_dest_t *listener, int8 *evt_str, int32 type)
{
	memdb_integer evt_nid;
	memdb_integer listener_nid = 0;
	int32 mask = 0;

	listener_nid = libwrap_get_listener(listener->dest, evt_str, &evt_nid, type);

	if (listener_nid == 0)
		return 0;

	return libwrap_unsubscribe_listener(listener, listener_nid);
}


json_t *process_listener(json_t *req, listener_dest_t *listener, int32 mask, int32 node_type)
{
	int8 *action = NULL;
	int8 *event_type = NULL;
	int8 *dest = NULL;
	int8 *type = NULL;
	int8 *protocol = NULL;
	json_t *elem = NULL;
	json_t *event_types = NULL;
	json_t *obj = NULL;
	int32 array_size, i = 0;

	action = json_string_value(json_object_get(req, RMM_JSON_ACTION));
	type = json_string_value(json_object_get(req, RMM_JSON_TYPE));
	dest = json_string_value(json_object_get(req, RMM_JSON_DEST));
	protocol = json_string_value(json_object_get(req, RMM_JSON_PROTOCOL));
	event_types = json_object_get(req, RMM_JSON_RF_EVT_TYPES);


	if (!(action && type && dest && protocol && event_types)) {
		HTTPD_ERR("get json string error\n");
		return NULL;
	}

	snprintf(listener->type, sizeof(listener->type), "%s", type);
	snprintf(listener->dest, sizeof(listener->dest), "%s", dest);
	snprintf(listener->protocol, sizeof(listener->protocol), "%s", protocol);
	listener->idx_mask = mask;

	array_size = json_array_size(event_types);
	for (i = 0; i < array_size; i++) {
		elem = NULL;
		elem = json_array_get(event_types, i);
		if (elem == NULL) {
			HTTPD_ERR("get json array element error\n");
			return NULL;
		}

		if ((obj = json_object_get(elem, RMM_JSON_RF_EVT)) != NULL) {
			event_type = json_string_value(obj);
			if(event_type == NULL) {
				HTTPD_ERR("get json string  event error\n");
				return NULL;
			}
			if (0 == strcmp(action, "subscribe"))
				subscribe_event(listener, event_type, node_type);
			else if (0 == strcmp(action, "unsubscribe"))
				unsubscribe_event(listener, event_type, node_type);
		}
	}

	return NULL;
}


json_t *process_snmp_listener(json_t *req)
{
	int8 *action = NULL;
	int8 *dest = NULL;
	int32 port = 0;

	action = json_string_value(json_object_get(req, RMM_JSON_ACTION));
	dest = json_string_value(json_object_get(req, RMM_JSON_DEST));
	port = json_integer_value(json_object_get(req, RMM_JSON_PORT));

	if (!(action && dest)) {
		HTTPD_ERR("get json string error\n");
		return NULL;
	}

	if (0 == strcmp(action, "subscribe"))
		snmp_subagentd_set(0, dest, port);
	else if (0 == strcmp(action, "unsubscribe"))
		snmp_subagentd_set(1, dest, port);

	return NULL;
}

void pack_rf_evt_links(json_t *result, rf_link_t *link)
{
	int32 i = 0;
	json_t *array = NULL;
	json_t *listener = NULL;

	array = json_array();
	if (array == NULL) {
		HTTPD_ERR("json array request fail\n");
		return;
	}

	for (i = 0; i < RF_EVENT_MAX_LISTENER_NUM; i++) {
		listener = NULL;
		listener = json_object();
		if (listener != NULL) {
			if (strlen(link->subscription[i]) != 0) {
				add_json_string(listener, RMM_JSON_RF_LISTENER, (uint8 *)(link->subscription[i]));
				json_array_add(array, listener);
			}
		}
	}
	json_object_add(result, RMM_JSON_RF_SUBSCRIPTION, array);

	add_json_string(result, RMM_JSON_RF_SELF, (uint8 *)link->self);
}

void update_href_host(uint8 *href, int32 len, int8 *host)
{
	int32 ip[4] = {};
	int32 port = 0;
	char url[96] = {};

	sscanf((int8 *)href, "http://%d.%d.%d.%d:%d%s", &ip[0],&ip[1],&ip[2],&ip[3], &port, url);
	snprintf((int8 *)href, len, "http://%s%s", host, url);
}


void get_event_type_by_nodeid(int32 nodeid, int8 *data, int32 data_len)
{
	int32 i  = 0;

	for (i = 0; i < sizeof(nodeid_keys) / sizeof(id_evt_type_t); i++) {
		if (nodeid_keys[i].node_id == nodeid) {
			strncpy_safe(data, nodeid_keys[i].type_name, data_len, data_len - 1);
			return;
		}
	}
	HTTPD_ERR("nodeid input error: %d\n", nodeid);
}


json_t *get_json_listener(listener_dest_info_t *header)
{
	json_t *result = NULL;
	json_t *jitem = NULL;
	json_t *evt_type_item = NULL;
	int32 i = 0;
	int8 evt_type[128];
	listener_dest_info_t *tmp;

	result = json_array();
	if (result == NULL) {
		HTTPD_ERR("result json object request fail\n");
		return NULL;
	}

	while (header) {
		jitem = json_object();
		json_object_add(jitem, RMM_JSON_RF_LINKS, json_string(header->dest));

		evt_type_item = json_array();
		for (i = 0; i < header->evt_index; i++) {
			memset(evt_type, 0, 128);
			get_event_type_by_nodeid(header->event_types[i], evt_type, sizeof(evt_type));
			json_array_add(evt_type_item, json_string(evt_type));
		}

		json_object_add(jitem, RMM_JSON_RF_EVT_TYPES, evt_type_item);
		json_array_add(result, jitem);
		tmp = header->pnext;
		free(header);
		header = tmp;
	}

	return result;
}

void get_evt_links(rf_link_t *links, int32 type, int32 reg, const int8 *fmt, ...)
{
	va_list args;
	int32 port;
	int8 ip_addr[WRAP_DB_MAX_VALUE_LEN] = {0};
	memdb_integer nid = 0;
	int32 mask, zone_reg = 0;
	int8 fmt_hdr[256] = {0};
	int8 format[2048] = {0};
	int8 prefix[MAX_URL] = {0};

	rmm_cfg_get_rest_prefix(prefix, MAX_URL);

	if (libutils_get_ip((int8 *)ip_addr) < 0) {
		memset(ip_addr, 0, WRAP_DB_MAX_VALUE_LEN);
		strncpy_safe(ip_addr, "x", sizeof(ip_addr), 1);
	}

	port = rmm_cfg_get_port(RESTD_PORT);
	if (port == 0) {
		HTTPD_ERR("Failed to call rmm_cfg_get_restd_port\n");
		exit(-1);
	}

	snprintf(fmt_hdr, sizeof(fmt_hdr), RF_EVENT_URL_FMT_HDR, ip_addr, port);
	snprintf(format, sizeof(format), "%s%s%s", fmt_hdr, fmt, prefix);

	va_start(args, fmt);
	vsprintf(links->self, format, args);
	va_end(args);

	nid = libwrap_get_uniq_node_by_type(type);

	libwrap_get_listener_links(reg, nid, links);
}
