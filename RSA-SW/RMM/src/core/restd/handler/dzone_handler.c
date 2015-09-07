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

static struct rest_handler drawer_coll_handler;
static struct rest_handler drawer_handler;
static struct rest_handler drawer_coll_evt_handler;
static struct rest_handler drawer_evt_handler;
static struct rest_handler drawer_coll_listener_handler;
static struct rest_handler drawer_listener_handler;

void register_drawer_handler(void)
{
	register_handler("/rack/drawers", &drawer_coll_handler);
	register_handler("/rack/drawers/{drawer_uuid}", &drawer_handler);

	register_handler("/rack/drawers/rf_event", &drawer_coll_evt_handler);
	register_handler("/rack/drawers/{drawer_uuid}/rf_event", &drawer_evt_handler);

	register_handler("/rack/drawers/rf_listener", &drawer_coll_listener_handler);
	register_handler("/rack/drawers/{drawer_uuid}/rf_listener", &drawer_listener_handler);
}

static int32 get_drawer_asset_idx(const struct rest_uri_param *param, const int8 *name, int32 *zone_idx, int32 *idx)
{
	int8 *p_id = NULL;
	uint8 uuid[64] = {0};
	memdb_integer node_id;
	struct node_info *drawer_info;
	struct node_info *dzone_info;
	int32 temp = 0;
	int32 cm_lid = 0;
	int32 drawer_lid = 0;
	int8 result[64] = {0};

	p_id = rest_path_value(param, name);
	if (p_id == NULL) {
		HTTPD_ERR("get value fail\n");
		return -1;
	}

	if (TRUE == is_str_uuid((const int8 *)p_id)) {
		snprintf((int8 *)uuid, sizeof(uuid), "%s", p_id);
		if (libwrap_get_node_id_by_uuid(uuid, &node_id) != RESULT_OK)
			return -1;

		if (libdb_attr_get_string(DB_RMM, node_id, DRAWER_LOC_ID_STR, result, 64, LOCK_ID_NULL) != RESULT_OK)
			return -1;
		*idx = (int32)str2int(result);

		drawer_info = libdb_get_node_by_node_id(DB_RMM, node_id, LOCK_ID_NULL);
		if (drawer_info == NULL)
			return -1;

		if (libdb_attr_get_string(DB_RMM, drawer_info->parent, DZONE_LOC_ID_STR, result, 64, LOCK_ID_NULL) != RESULT_OK)
			return -1;
		drawer_lid = (int32)str2int(result);

		dzone_info = libdb_get_node_by_node_id(DB_RMM, drawer_info->parent, LOCK_ID_NULL);
		if (dzone_info == NULL)
			return -1;
		if (libdb_attr_get_string(DB_RMM, dzone_info->parent, MBP_LOC_ID_STR, result, 64, LOCK_ID_NULL) != RESULT_OK)
			return -1;
		cm_lid = (int32)str2int(result);

		*zone_idx = drawer_lid + (cm_lid - 1) * MAX_DZONE_NUM;

	} else {
		temp  = (int32)str2int(p_id);
		if ((temp < 1)||(temp > MAX_DRAWER_NUM * MAX_CM_NUM)) {
			HTTPD_ERR("drawer id exceed!\n");
			return -1;
		}
		*zone_idx = (temp-1) / MAX_DRAWER_NUM + 1;
		*idx = (temp > MAX_DRAWER_NUM)?(temp - MAX_DRAWER_NUM):temp;
	}

	return 0;
}

static json_t *drawer_coll_get(struct rest_uri_param *param)
{
	collections_t *drawer_collections = NULL;
	uint32 drawer_num = 0;
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *array = NULL;
	json_t *sub_drawer = NULL;
	int32 i;
	int32 drawers_num = 0;

	drawers_num = libwrap_get_asset_num(MC_TYPE_DRAWER);
	if (drawers_num == 0) {
		HTTPD_ERR("get drawer num fail\n");
		return NULL;
	}

	drawer_collections = (collections_t *)malloc(drawers_num * sizeof(collections_t));
	if (drawer_collections == NULL)
		return NULL;

	memset(drawer_collections, 0, drawers_num * sizeof(collections_t));

	rs = libwrap_get_drawer_coll(drawer_collections, &drawer_num);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get drawer collection fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request fail\n");
		return NULL;
	}

	array = json_array();
	if (array == NULL) {
		HTTPD_ERR("json array requset fail\n");
		return NULL;
	}

	for (i = 0; i < drawer_num; i++) {
		sub_drawer = NULL;
		sub_drawer = json_object();
		if (sub_drawer != NULL) {
			add_json_integer(sub_drawer, RMM_JSON_ID, drawer_collections[i].id);
			add_json_string(sub_drawer, RMM_JSON_UUID, drawer_collections[i].uuid);
			add_json_string(sub_drawer, RMM_JSON_NAME, drawer_collections[i].name);
			update_href_host(drawer_collections[i].href, HREF_URL_LEN, param->host);
			add_json_string(sub_drawer, RMM_JSON_HREF, drawer_collections[i].href);
			json_array_add(array, sub_drawer);
		}
	}
	json_object_add(result, RMM_JSON_DRAWERS, array);

	if (drawer_collections)
		free(drawer_collections);

	return result;
}

static json_t *drawer_coll_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *drawer_coll_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *drawer_coll_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	json_t *result = NULL;
	int32 reg = 0;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));
	reg |= RF_EVENT_MASK_ALL;
	get_evt_links(&(service.links),MC_REDFISH_DRAWER_COLL, reg, RF_EVENT_DRAWER_COLL_FMT);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *drawer_coll_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *drawer_coll_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	int32 mask = 0;
	listener_dest_t listener;

	update_response_info(param, HTTP_APPLICATION_ERROR);

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);

	mask |= RF_EVENT_MASK_ALL;

	return process_listener(req, &listener, mask, MC_REDFISH_DRAWER_COLL);
}

static int32 pack_drawer_basic_info(const drawer_member_t *drawer_member, json_t *result)
{
	pack_basic_element_info((const base_element_t *)&(drawer_member->be), result);
	/*
	add_json_integer(result, RMM_JSON_DRAWER_ID, drawer_member->be.id);
	add_json_string(result, RMM_JSON_RACkUUID, drawer_member->be.uuid);
	*/
	if (0 == drawer_member->TMC_ip_addr[0]) {
		const uint8 ip[] = "N/A";
		add_json_string(result, RMM_JSON_DRAWER_IPADDRESS, ip);
	} else
		add_json_string(result, RMM_JSON_DRAWER_IPADDRESS, drawer_member->TMC_ip_addr);

	add_json_integer(result, RMM_JSON_DRAWER_RUID, drawer_member->tray_ruid);
	add_json_string(result, RMM_JSON_DRAWER_PUID, drawer_member->tray_puid);

	return 0;
}

static json_t *drawer_get(struct rest_uri_param *param)
{
	drawer_member_t drawer_member = { {0} };
	result_t rs = RESULT_OK;
	json_t *av_action = NULL;
	json_t *result = NULL;
	json_t *loc = NULL;
	json_t *cap = NULL;
	json_t *action = NULL;
	json_t *property = NULL;
	json_t *allow_values = NULL;
	int32 dzone_idx = 0;
	int32 drawer_idx = 0;
	json_t *power_thermal = NULL;

	rs = get_drawer_asset_idx(param, "drawer_uuid", &dzone_idx, &drawer_idx);
	if ((rs != RESULT_OK) || (drawer_idx <= 0)) {
		HTTPD_ERR("drawer zone index get fail\n");
		return NULL;
	}

	rs = libwrap_get_drawer_by_idx(dzone_idx, drawer_idx, &drawer_member);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get drawer by idx fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}
	
	pack_drawer_basic_info(&drawer_member, result);

	loc = json_object();
	if (loc == NULL) {
		HTTPD_ERR("local object request fail\n");
		return NULL;
	}

	add_json_string(loc, RMM_JSON_POD, (uint8 *)drawer_member.pod);
	add_json_string(loc, RMM_JSON_RACK, (uint8 *)drawer_member.rack);
	add_loc_info(loc, drawer_member.loc.u_location, RMM_JSON_ULOC);
	add_loc_info(loc, drawer_member.loc.u_height, RMM_JSON_UHEIGHT);
	add_loc_info(loc, drawer_member.loc.x_location, RMM_JSON_XLOC);
	add_loc_info(loc, drawer_member.loc.units, RMM_JSON_RACK_UNITS);
	json_object_add(result, RMM_JSON_LOC, loc);

	add_json_integer(result, RMM_JSON_DRAWER_STATE, (int32)(drawer_member.thermal_st));

	power_thermal = json_object();
	if (power_thermal == NULL) {
		HTTPD_ERR("local object request fail\n");
		return NULL;
	}

	add_json_integer(power_thermal, RMM_JSON_DRAWER_POWER_CONSUMED_WATTS, (int32)(drawer_member.power_consumption));
	add_json_integer(power_thermal, RMM_JSON_DRAWER_PRESENT_TEMP, (int32)(drawer_member.pres_temp));
	json_object_add(result, RMM_JSON_DRAWER_POWER_AND_THERMAL, power_thermal);

	add_json_bool(result, RMM_JSON_RMM_PRESENT, (int32)drawer_member.rmm_present); ///1---True
	add_json_integer(result, "RackSupportsDisaggregatedPowerCooling", (int32)drawer_member.rack_disaggr); ///0---False

	av_action = json_array();
	if (av_action == NULL) {
		HTTPD_ERR("available action json array request fail\n");
		return NULL;
	}

	action = json_object();
	if (0 != prepare_get_action(&(drawer_member.av_action), action, 0)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	cap = json_array();
	if (cap == NULL) {
		HTTPD_ERR("cap json array request fail\n");
		return NULL;
	}

	allow_values = json_array();
	property = json_object();
	if ((property != NULL) && (allow_values != NULL)) {
		/* soft */
		if (0 != strcmp((int8 *)(drawer_member.av_action.cap[0].property), "")) {
			json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(drawer_member.av_action.cap[0].property)));
			json_array_add(allow_values, json_string((int8 *)(drawer_member.av_action.cap[0].av)));
		}
#if 0
		/* hard */
		if (0 != strcmp((int8 *)(drawer_member.av_action.cap[1].property), "")) {
			json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(drawer_member.av_action.cap[1].property)));
			json_array_add(allow_values, json_string((int8 *)(drawer_member.av_action.cap[1].av)));
		}
#endif
		json_object_add(property, RMM_JSON_ALLOWABLE_VALUES, allow_values);
		json_array_add(cap, property);
	}

	json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	json_array_add(av_action, action);
	json_object_add(result, RMM_JSON_AVALIBLE_ACTIONS, av_action);
	add_json_bool(result, RMM_JSON_DISAGGR, false);
	update_href_host(drawer_member.href, HREF_URL_LEN, param->host);
	add_json_string(result, RMM_JSON_HREF, drawer_member.href);

	return result;
}

static json_t *drawer_put(struct rest_uri_param *param)
{
	json_t *req = NULL;
	result_t rs = RESULT_OK;
	put_drawer_t put_drawer_info = { {0} };
	int8 *description = NULL;
	json_t *rs_json = NULL;
	int32 dzone_idx = 0;
	int32 drawer_idx = 0;

	int32 index = 0;

	rs = get_drawer_asset_idx(param, "drawer_uuid", &dzone_idx, &drawer_idx);
	if (rs != RESULT_OK) {
		HTTPD_ERR("drawer index get fail\n");
		return NULL;
	}

	rs = libwrap_pre_put_drawer(dzone_idx, drawer_idx, &put_drawer_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("drawer pre put fail, result is %d\n", rs);
		return NULL;
	}

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	put_prepare_str(req, put_drawer_info.descr, DESCRIPTION_LEN, RMM_JSON_DESC);

	rs = libwrap_put_drawer(dzone_idx, drawer_idx, put_drawer_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("drawer put error, result is %d\n", rs);
		return NULL;
	}

	json_free(req);
	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", (dzone_idx -1)* MAX_DRAWER_NUM +index);
	rf_log(INFO, MSGDrawerUpdate, buff);

	return drawer_get(param);
}

static json_t *drawer_post(struct rest_uri_param *param)
{
	json_t *req;
	result_t rs = RESULT_OK;
	int32 reset_drawer_result = 0;
	memdb_integer node_id = 0;
	int32 index = 0;
	int32 dzone_idx = 0;
	int32 drawer_idx = 0;

	json_t *action_json = NULL;
	json_t *type_json = NULL;
	int8 *action = NULL;
	int8 *type = NULL;
	rs = get_drawer_asset_idx(param, "drawer_uuid", &dzone_idx, &drawer_idx);
	if (rs == 0) {
		HTTPD_ERR("drawer index get fail\n");
		return NULL;
	}

	update_response_info(param, HTTP_APPLICATION_ERROR);

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	if (NULL == (action_json = json_object_get(req, RMM_JSON_ACTION)) ||
		NULL == (type_json = json_object_get(req, RMM_JSON_RESET_TYPE))) {
		HTTPD_ERR("fail to get json field.\n");
		goto err;
	}

	action = json_string_value(action_json);
	type = json_string_value(type_json);
	if (action == NULL || type == NULL) {
		HTTPD_ERR("fail to get json string.");
		goto err;
	}

	if (0 == strcmp(action, RMM_JSON_RESET_BIG)) {
		if(0 == strcmp(type, RMM_JSON_RESET_SOFT_STR))
			libwrap_drawer_soft_reset(dzone_idx, drawer_idx, &reset_drawer_result);
		else if(0 == strcmp(type, RMM_JSON_RESET_HARD_STR))
			libwrap_drawer_hard_reset(dzone_idx, drawer_idx, &reset_drawer_result);
		else 
			goto err;

		if (reset_drawer_result == RESULT_OK)
			update_response_info(param, HTTP_ACCEPTED);
		else
			update_response_info(param, HTTP_APPLICATION_ERROR);

		int8 buff[128] = {};
		snprintf(buff, sizeof(buff), "%d", (dzone_idx -1)* MAX_DRAWER_NUM + drawer_idx);
		rf_snmp_evt(INFO, MSGDrawerChange, buff, "Reseted"); 
		return NULL;
	}
err:
	json_free(req);
	return NULL;
	
}

static json_t *drawer_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	int32 reg = 0;
	int8 *p_uuid;
	int32 drawer_idx;
	int32 dzone_idx;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	rs = get_drawer_asset_idx(param, "drawer_uuid", &dzone_idx, &drawer_idx);
	if (rs == 0) {
		HTTPD_ERR("drawer index get fail\n");
		return NULL;
	}

	reg = 1 << ((dzone_idx - 1) * MAX_DRAWER_NUM + (drawer_idx - 1));

	pack_rf_base_json(result, &(service.base));
	get_evt_links(&(service.links), MC_REDFISH_DRAWER_COLL, reg, RF_EVENT_DRAWER_FMT, dzone_idx, drawer_idx);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *drawer_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *drawer_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	result_t rs = RESULT_OK;
	int32 reg = 0;
	listener_dest_t listener;
	int32 mask = 0;
	int32 dzone_idx = 0;
	int32 drawer_idx = 0;

	update_response_info(param, HTTP_APPLICATION_ERROR);

	rs = get_drawer_asset_idx(param, "drawer_uuid", &dzone_idx, &drawer_idx);
	if (rs == 0) {
		HTTPD_ERR("drawer index get fail\n");
		return NULL;
	}

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);

	mask = 1 << ((dzone_idx - 1) * MAX_DRAWER_NUM + (drawer_idx - 1));

	return process_listener(req, &listener, mask, MC_REDFISH_SYS);
}

static json_t *drawer_coll_listener_get(struct rest_uri_param *param)
{
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_DRAWER_COLL, INVAILD_IDX, INVAILD_IDX, header);

	return	get_json_listener(header);
}

static json_t *drawer_listener_get(struct rest_uri_param *param)
{
	int32 drawer_idx = get_asset_idx(param, "drawer_uuid", MC_TYPE_DRAWER);

	if(drawer_idx == -1) {
		HTTPD_ERR("get drawer index index fail\n");
		return NULL;
	}

	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_DRAWER_COLL, INVAILD_IDX, drawer_idx, header);

	return get_json_listener(header);
}

static struct rest_handler drawer_coll_handler = {
	.get    = drawer_coll_get,
	.put    = drawer_coll_put,
	.post   = drawer_coll_post,
};

static struct rest_handler drawer_handler = {
	.get    = drawer_get,
	.put    = drawer_put,
	.post   = drawer_post,
};

static struct rest_handler drawer_coll_evt_handler = {
	.get    = drawer_coll_evt_get,
	.put    = drawer_coll_evt_put,
	.post   = drawer_coll_evt_post,
};

static struct rest_handler drawer_evt_handler = {
	.get    = drawer_evt_get,
	.put    = drawer_evt_put,
	.post   = drawer_evt_post,
};


static struct rest_handler drawer_coll_listener_handler = {
	.get    = drawer_coll_listener_get,
	.put    = NULL,
	.post   = NULL,
};

static struct rest_handler drawer_listener_handler = {
	.get    = drawer_listener_get,
	.put    = NULL,
	.post   = NULL,
};


