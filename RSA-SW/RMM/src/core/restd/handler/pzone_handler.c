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

static struct rest_handler pzone_coll_handler;
static struct rest_handler pzone_handler;
static struct rest_handler psu_coll_handler;
static struct rest_handler psu_handler;
static struct rest_handler pzone_coll_evt_handler;
static struct rest_handler pzone_evt_handler;
static struct rest_handler psu_coll_evt_handler;
static struct rest_handler psu_evt_handler;
static struct rest_handler pzone_coll_listener_handler;
static struct rest_handler pzone_listener_handler;
static struct rest_handler psu_coll_listener_handler;
static struct rest_handler psu_listener_handler;


void register_pzone_handler(void)
{
	register_handler("/rack/powerzones", &pzone_coll_handler);
	register_handler("/rack/powerzones/{zone_id}", &pzone_handler);

	register_handler("/rack/powerzones/rf_event", &pzone_coll_evt_handler);
	register_handler("/rack/powerzones/{zone_id}/rf_event", &pzone_evt_handler);

	register_handler("/rack/powerzones/rf_listener", &pzone_coll_listener_handler);
	register_handler("/rack/powerzones/{zone_id}/rf_listener", &pzone_listener_handler);
}

void register_psu_handler(void)
{
	register_handler("/rack/powerzones/{zone_id}/psus", &psu_coll_handler);
	register_handler("/rack/powerzones/{zone_id}/psus/{psu_id}", &psu_handler);

	register_handler("/rack/powerzones/{zone_id}/psus/rf_event", &psu_coll_evt_handler);
	register_handler("/rack/powerzones/{zone_id}/psus/{psu_id}/rf_event", &psu_evt_handler);

	register_handler("/rack/powerzones/{zone_id}/psus/rf_listener", &psu_coll_listener_handler);
	register_handler("/rack/powerzones/{zone_id}/psus/{psu_id}/rf_listener", &psu_listener_handler);
}

static json_t *pzone_coll_get(struct rest_uri_param *param)
{
	collections_t *pzone_collections = NULL;
	uint32 pzone_num = 0;
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *pzone = NULL;
	json_t *array = NULL;
	int32 i;
	int32 zone_num = 0;

	zone_num = libwrap_get_asset_num(MC_TYPE_PZONE);
	if (zone_num == 0) {
		HTTPD_ERR("get powerzone num fail\n");
		return NULL;
	}

	pzone_collections = (collections_t *)malloc(zone_num * sizeof(collections_t));
	if (pzone_collections == NULL)
		return NULL;

	memset(pzone_collections, 0, zone_num * sizeof(collections_t));

	rs = libwrap_get_pzone_coll(pzone_collections, &pzone_num);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get powerzone collection fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request fail\n");
		return NULL;
	}

	array = json_array();
	if (array == NULL) {
		HTTPD_ERR("json array request fail\n");
		return NULL;
	}

	for (i = 0; i < pzone_num; i++) {
		pzone = NULL;
		pzone = json_object();
		if (pzone != NULL) {
			add_json_integer(pzone, RMM_JSON_ID, pzone_collections[i].id);
			add_json_string(pzone, RMM_JSON_UUID, pzone_collections[i].uuid);
			add_json_string(pzone, RMM_JSON_NAME, pzone_collections[i].name);
			update_href_host(pzone_collections[i].href, HREF_URL_LEN, param->host);
			add_json_string(pzone, RMM_JSON_HREF, pzone_collections[i].href);
		}
		json_array_add(array, pzone);
	}
	json_object_add(result, RMM_JSON_POWER_ZONES, array);

	if (pzone_collections)
		free(pzone_collections);

	return result;
}

static json_t *pzone_coll_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *pzone_coll_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *pzone_coll_listener_get(struct rest_uri_param *param)
{
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_PZONE, INVAILD_IDX, INVAILD_IDX, header);

	return get_json_listener(header);
}

static json_t *pzone_coll_evt_get(struct rest_uri_param *param)
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
	get_evt_links(&(service.links), MC_REDFISH_PZONE, reg, RF_EVENT_PZONE_COLL_FMT);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *pzone_coll_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *pzone_coll_evt_post(struct rest_uri_param *param)
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

	return process_listener(req, &listener, mask, MC_REDFISH_PZONE);
}

static int32 pack_pzone_basic_info(const pzone_member_t *pzone_member, json_t *result)
{
	pack_basic_element_info(&(pzone_member->be), result);
	/*
	add_json_integer(result, RMM_JSON_PZONE_ID, pzone_member->be.id);
	add_json_string(result, RMM_JSON_UUID, pzone_member->be.uuid);
	*/
	return 0;
}

static json_t *pzone_get(struct rest_uri_param *param)
{
	pzone_member_t pzone_member = { {0} };
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *loc = NULL;
	int32 pzone_idx = 0;
	int32 presence_len = 0;

	pzone_idx = get_asset_idx(param, RMM_JSON_ZONE_ID, MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	rs = libwrap_get_pzone_by_idx(pzone_idx, &pzone_member);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get power zone fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request fail\n");
		return NULL;
	}

	pack_pzone_basic_info(&pzone_member, result);

	loc = json_object();
	if (loc != NULL) {
		add_loc_info(loc, pzone_member.power_zone_loc.x_location, RMM_JSON_XLOC);
		add_loc_info(loc, (int32)pzone_member.power_zone_loc.units, RMM_JSON_RACK_UNITS);
		add_loc_info(loc, pzone_member.power_zone_loc.u_location, RMM_JSON_ULOC);
		add_loc_info(loc, pzone_member.power_zone_loc.u_height, RMM_JSON_UHEIGHT);
	}
	json_object_add(result, RMM_JSON_RACK_LOC, loc);

	add_json_integer(result, RMM_JSON_POWER_INPUT, pzone_member.tot_power_consumption);
	add_json_integer(result, RMM_JSON_POWER_MAX_RATED_CAPACITY, pzone_member.tot_power_cap);
	add_json_integer(result, RMM_JSON_POWER_OUTPUT, pzone_member.tot_power_production);
	add_json_string(result, RMM_JSON_PRESENCE, (uint8 *)(pzone_member.presence));

	//TBD: it can be saved in memdb
	add_json_integer(result, RMM_JSON_MAX_PSUS_SUPPORT, MAX_PSU_NUM);
	presence_len = strlen(pzone_member.presence);
	if ((presence_len > -1) && (presence_len < 9)) {
		int32 i = 0;
		int32 number_psu = 0;
		for(i = 0; i < presence_len; i++) {
			if (pzone_member.presence[i] == 49)//"1"
				number_psu++;
		}
		add_json_integer(result, RMM_JSON_PSU_NUMBER_OF_PRESENT, number_psu);
	}

	return result;
}

static json_t *pzone_put(struct rest_uri_param *param)
{
	json_t *req = NULL;
	result_t rs = RESULT_OK;
	put_pzone_t put_pzone_info = { {0} };
	int8 *description = NULL;
	int32 pzone_idx = 0;

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	rs = libwrap_pre_put_pzone_by_idx(pzone_idx, &put_pzone_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("power zone pre put fail, result is %d\n", rs);
		return NULL;
	}

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	put_prepare_str(req, put_pzone_info.descr, DESCRIPTION_LEN, RMM_JSON_DESC);

	rs = libwrap_put_pzone_by_idx(pzone_idx, put_pzone_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("put power zone fail, result is %d\n", rs);
		return NULL;
	}

	json_free(req);

	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", pzone_idx);
	rf_log(INFO, MSGPZoneUpdate, buff);
	return pzone_get(param);
}

static json_t *pzone_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *pzone_listener_get(struct rest_uri_param *param)
{
	int32 pzone_idx = 0;
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_PZONE, INVAILD_IDX, pzone_idx - 1 , header);
	return	get_json_listener(header);
}

static json_t *pzone_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	json_t *result = NULL;
	int32 pzone_idx, reg = 0;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));
	reg = 1 << (pzone_idx - 1);
	get_evt_links(&(service.links), MC_REDFISH_PZONE, reg, RF_EVENT_PZONE_FMT, pzone_idx);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *pzone_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *pzone_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	int32 mask = 0;
	listener_dest_t listener;
	int32 pzone_idx;

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	update_response_info(param, HTTP_APPLICATION_ERROR);

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);

	mask = 1 << (pzone_idx - 1);

	return process_listener(req, &listener, mask, MC_REDFISH_PZONE);
}

static json_t *psu_coll_get(struct rest_uri_param *param)
{
	collections_t *pzone_psu_coll = NULL;
	uint32 psu_num = 0;
	uint32 pzone_idx = 0;
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *psu = NULL;
	json_t *array = NULL;
	int32 i;

	pzone_psu_coll = (collections_t *)malloc(MAX_PSU_NUM * sizeof(collections_t));
	if (pzone_psu_coll == NULL)
		return NULL;

	memset(pzone_psu_coll, 0, MAX_PSU_NUM * sizeof(collections_t));

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	rs = libwrap_get_pzone_psu_coll(pzone_idx, pzone_psu_coll, &psu_num);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get psu collection fail %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	array = json_array();
	if (array == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	for (i = 0; i < psu_num; i++) {
		psu = NULL;
		psu = json_object();
		if (psu != NULL) {
			add_json_integer(psu, RMM_JSON_ID, pzone_psu_coll[i].id);
			add_json_string(psu, RMM_JSON_UUID, pzone_psu_coll[i].uuid);
			add_json_string(psu, RMM_JSON_NAME, pzone_psu_coll[i].name);
			update_href_host(pzone_psu_coll[i].href, HREF_URL_LEN, param->host);
			add_json_string(psu, RMM_JSON_HREF, pzone_psu_coll[i].href);
		}
		json_array_add(array, psu);
	}
	json_object_add(result, RMM_JSON_PSUS, array);

	if (pzone_psu_coll)
		free(pzone_psu_coll);

	return result;
}

static json_t *psu_coll_put(struct rest_uri_param *param)
{
	return NULL;
}


static json_t *psu_coll_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *psu_coll_listener_get(struct rest_uri_param *param)
{
	int32 pzone_idx = 0;
	int32 psu_idx   = INVAILD_IDX;
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_PSU_COLL, pzone_idx, INVAILD_IDX, header);

	return get_json_listener(header);
}

static json_t *psu_coll_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	json_t *result = NULL;
	int32 pzone_idx, reg = 0;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));
	reg |= RF_EVENT_MASK_ALL;
	get_evt_links(&(service.links), MC_REDFISH_PSU_COLL, reg, RF_EVENT_PSU_COLL_FMT, pzone_idx);
	pack_rf_evt_links(result, &(service.links));

	return result;

}

static json_t *psu_coll_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *psu_coll_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	int32 reg = 0;
	listener_dest_t listener;
	int32 pzone_idx;

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	update_response_info(param, HTTP_APPLICATION_ERROR);

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);

	reg |= RF_EVENT_MASK_ALL;

	return process_listener(req, &listener, reg, MC_REDFISH_PSU_COLL);
}

static int32 pack_psu_basic_info(psu_member_t *psu_member, json_t *result)
{
	pack_basic_element_info(&(psu_member->be), result);
	/*
	add_json_integer(result, RMM_JSON_PSU_ID, psu_member->be.id);
	add_json_string(result, RMM_JSON_UUID, psu_member->be.uuid);
	*/
	add_json_integer(result, RMM_JSON_ENABLE_STATE, psu_member->enable_state);
	add_json_string(result, RMM_JSON_REDUNDANCY_SET, (uint8 *)(psu_member->redundancy));
	add_json_integer(result, RMM_JSON_HEALTH_CODE, psu_member->health_state);
	add_json_integer(result, RMM_JSON_POWER_INPUT, psu_member->power_in);
	add_json_integer(result, RMM_JSON_POWER_MAX_RATED_CAPACITY, psu_member->power_cap);
	add_json_integer(result, RMM_JSON_POWER_OUTPUT, psu_member->power_out);
	add_json_string(result, RMM_JSON_ASSET_TAG, psu_member->asset.asset_tag);

	return 0;
}

static json_t *psu_get(struct rest_uri_param *param)
{
	psu_member_t psu_member = { {0} };
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *fru = NULL;
	json_t *av_action = NULL;
	json_t *action = NULL;
	json_t *cap = NULL;
	int32 pzone_idx, psu_idx;
	json_t *loc = NULL;

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	psu_idx = get_asset_idx(param, "psu_id", MC_TYPE_PSU);
	if (psu_idx == -1) {
		HTTPD_ERR("get psu index fail\n");
		return NULL;
	}

	rs = libwrap_get_pzone_psu_by_idx(pzone_idx, psu_idx,  &psu_member);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get psu fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	pack_psu_basic_info(&psu_member, result);

	loc = json_object();
	if (loc != NULL) {
		add_loc_info(loc, (int32)psu_member.psu_loc.units, RMM_JSON_RACK_UNITS);
		add_loc_info(loc, psu_member.psu_loc.u_location, RMM_JSON_ULOC);
		add_loc_info(loc, psu_member.psu_loc.u_height, RMM_JSON_UHEIGHT);
		add_loc_info(loc, psu_member.psu_loc.x_location, RMM_JSON_XLOC);
	}
	json_object_add(result, RMM_JSON_RACK_LOC, loc);

	fru = json_object();
	if (fru != NULL) {
		pack_fru_info(&(psu_member.asset.fru), fru);
		json_object_add(result, RMM_JSON_FRU_INFO, fru);
	}

	av_action = json_array();
	if (av_action == NULL) {
		HTTPD_ERR("available action json array request error\n");
		return NULL;
	}

	/* action reset */
	action = json_object();
	if (0 != prepare_get_action(&(psu_member.av_action), action, 0)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	cap = json_array();
	if (cap != NULL) {
		json_t *property = NULL;
		json_t *allow_values = NULL;

		allow_values = json_array();

		property = json_object();
		if ((property != NULL) && (allow_values != NULL)) {
			if (0 != strcmp((int8 *)(psu_member.av_action.cap[0].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(psu_member.av_action.cap[0].property)));
				json_array_add(allow_values, json_integer(str2int((int8 *)(psu_member.av_action.cap[0].av))));
			}

			if (0 != strcmp((int8 *)(psu_member.av_action.cap[1].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(psu_member.av_action.cap[1].property)));
				json_array_add(allow_values, json_integer(str2int((int8 *)(psu_member.av_action.cap[1].av))));
			}

			if (0 != strcmp((int8 *)(psu_member.av_action.cap[2].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(psu_member.av_action.cap[2].property)));
				json_array_add(allow_values, json_integer(str2int((int8 *)(psu_member.av_action.cap[2].av))));
			}
			json_object_add(property, RMM_JSON_ALLOWABLE_VALUES, allow_values);
			json_array_add(cap, property);
		}
		json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	}
	json_array_add(av_action, action);
	json_object_add(result, RMM_JSON_AVALIBLE_ACTIONS, av_action);

	return result;
}

static json_t *psu_put(struct rest_uri_param *param)
{
	json_t *req;
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	int8 descr_str[DESCRIPTION_LEN] = {0};
	int8 asset_tag_str[REST_ASSET_TAG_LEN] = {0};
	json_t *rs_json = NULL;
	int32 pzone_idx, psu_idx;

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	psu_idx = get_asset_idx(param, "psu_id", MC_TYPE_PSU);
	if (psu_idx == -1) {
		HTTPD_ERR("get psu zone index fail\n");
		return NULL;
	}

	rs = libwrap_pre_put_psu(pzone_idx, psu_idx, descr_str, asset_tag_str);
	if (rs != RESULT_OK) {
		HTTPD_ERR("psu pre put fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	req = json_parse(param->json_data);
	put_prepare_str(req, (uint8 *)descr_str, DESCRIPTION_LEN, RMM_JSON_DESC);
	put_prepare_str(req, (uint8 *)asset_tag_str, REST_ASSET_TAG_LEN, RMM_JSON_ASSET_TAG);

	rs = libwrap_put_psu(pzone_idx, psu_idx, descr_str, asset_tag_str);
	if (rs != RESULT_OK) {
		HTTPD_ERR("psu put fail, result is %d\n", rs);
		return NULL;
	}

	json_free(req);
	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", ((pzone_idx-1) * MAX_PSU_NUM + psu_idx));
	rf_log(INFO, MSGPsuUpdate, buff);
	return psu_get(param);
}

static json_t *psu_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	json_t *result = NULL;
	result_t rs = RESULT_OK;
	int8 *action = NULL;
	int32 en_st = 0;
	int32 pzone_idx, psu_idx;

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	psu_idx = get_asset_idx(param, "psu_id", MC_TYPE_PSU);
	if (psu_idx == -1) {
		HTTPD_ERR("get psu zone index fail\n");
		return NULL;
	}

	update_response_info(param, HTTP_APPLICATION_ERROR);

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		json_free(result);
		return NULL;
	}

	action = json_string_value(json_object_get(req, RMM_JSON_ACTION));
	if (action == NULL) {
		HTTPD_ERR("get json string error.\n");
		json_free(result);	
		return NULL;
	}

	en_st = json_integer_value(json_object_get(req, RMM_JSON_ENABLE_STATE));
	if (!((0 == strcmp(action, RMM_JSON_REQ_STATE_CHANGE))&&(
		(en_st == PSU_STATE_ENABLED) ||
		(en_st == PSU_STATE_DISABLED) ||
		(en_st == PSU_STATE_EN_OFFLINE)||
		(en_st == PSU_STATE_NOT_APP)))) {
		HTTPD_INFO("bad enable state, returning\n");
		json_free(req);
		json_free(result);
		return NULL;
	}

	rs = libwrap_post_pzone_psu_by_idx(pzone_idx, psu_idx, en_st);
	if (rs != RESULT_OK) {
		HTTPD_ERR("psu post fail, result is %d\n", rs);
		json_free(req);
		json_free(result);
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);
	json_free(req);

	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d",((pzone_idx -1) * MAX_PSU_NUM + psu_idx));
	rf_snmp_evt(INFO, MSGPsuChange, buff, "enable status");
	return result;
}

static json_t *psu_listener_get(struct rest_uri_param *param)
{
	int32 pzone_idx = 0;
	int32 psu_idx   = 0;
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	psu_idx = get_asset_idx(param, "psu_id", MC_TYPE_PSU);
	if (psu_idx == -1) {
		HTTPD_ERR("get psu index fail\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_PSU_COLL, pzone_idx, (pzone_idx - 1) * MAX_PSU_NUM + psu_idx, header);

	return get_json_listener(header);
}

static json_t *psu_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	json_t *result = NULL;
	int32 pzone_idx, psu_idx, reg = 0;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}
	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	psu_idx = get_asset_idx(param, "psu_id", MC_TYPE_PSU);
	if (psu_idx == -1) {
		HTTPD_ERR("get psu zone index fail\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));
	reg = 1 << ((pzone_idx - 1) * MAX_PSU_NUM + (psu_idx - 1));
	get_evt_links(&(service.links), MC_REDFISH_PSU_COLL, reg, RF_EVENT_PSU_FMT, pzone_idx, psu_idx);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *psu_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *psu_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	listener_dest_t listener;
	int32 pzone_idx, psu_idx;
	int32 mask = 0;

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_PZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get power zone index fail\n");
		return NULL;
	}

	psu_idx = get_asset_idx(param, "psu_id", MC_TYPE_PSU);
	if (psu_idx == -1) {
		HTTPD_ERR("get psu zone index fail\n");
		return NULL;
	}

	update_response_info(param, HTTP_APPLICATION_ERROR);

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);

	mask = 1 << ((pzone_idx - 1) * MAX_PSU_NUM + (psu_idx - 1));

	return process_listener(req, &listener, mask, MC_REDFISH_PSU_COLL);
}

static struct rest_handler pzone_coll_handler = {
	.get    = pzone_coll_get,
	.put    = pzone_coll_put,
	.post   = pzone_coll_post,
};

static struct rest_handler pzone_handler = {
	.get    = pzone_get,
	.put    = pzone_put,
	.post   = pzone_post,
};


static struct rest_handler psu_coll_handler = {
	.get    = psu_coll_get,
	.put    = psu_coll_put,
	.post   = psu_coll_post,
};

static struct rest_handler psu_handler = {
	.get    = psu_get,
	.put    = psu_put,
	.post   = psu_post,
};

static struct rest_handler pzone_coll_evt_handler = {
	.get    = pzone_coll_evt_get,
	.put    = pzone_coll_evt_put,
	.post   = pzone_coll_evt_post,
};

static struct rest_handler pzone_evt_handler = {
	.get    = pzone_evt_get,
	.put    = pzone_evt_put,
	.post   = pzone_evt_post,
};

static struct rest_handler psu_coll_evt_handler = {
	.get    = psu_coll_evt_get,
	.put    = psu_coll_evt_put,
	.post   = psu_coll_evt_post,
};

static struct rest_handler psu_evt_handler = {
	.get    = psu_evt_get,
	.put    = psu_evt_put,
	.post   = psu_evt_post,
};

static struct rest_handler pzone_coll_listener_handler = {
	.get    = pzone_coll_listener_get,
	.put    = NULL,
	.post   = NULL,
};

static struct rest_handler pzone_listener_handler = {
	.get    = pzone_listener_get,
	.put    = NULL,
	.post   = NULL,
};

static struct rest_handler psu_coll_listener_handler = {
	.get    = psu_coll_listener_get,
	.put    = NULL,
	.post   = NULL,
};

static struct rest_handler psu_listener_handler = {
	.get    = psu_listener_get,
	.put    = NULL,
	.post   = NULL,
};


