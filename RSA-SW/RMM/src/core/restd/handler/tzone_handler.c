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

static struct rest_handler tzone_coll_handler;
static struct rest_handler tzone_handler;
static struct rest_handler fan_coll_handler;
static struct rest_handler fan_handler;
static struct rest_handler tzone_coll_evt_handler;
static struct rest_handler tzone_evt_handler;
static struct rest_handler fan_coll_evt_handler;
static struct rest_handler fan_evt_handler;
static struct rest_handler tzone_coll_listener_handler;
static struct rest_handler tzone_listener_handler;
static struct rest_handler fan_coll_listener_handler;
static struct rest_handler fan_listener_handler;

void register_tzone_handler(void)
{
	register_handler("/rack/thermalzones", &tzone_coll_handler);
	register_handler("/rack/thermalzones/{zone_id}", &tzone_handler);

	register_handler("/rack/thermalzones/rf_event", &tzone_coll_evt_handler);
	register_handler("/rack/thermalzones/{zone_id}/rf_event", &tzone_evt_handler);

	register_handler("/rack/thermalzones/rf_listener", &tzone_coll_listener_handler);
	register_handler("/rack/thermalzones/{zone_id}/rf_listener", &tzone_listener_handler);
}

void  register_fan_handler(void)
{
	register_handler("/rack/thermalzones/{zone_id}/fans", &fan_coll_handler);
	register_handler("/rack/thermalzones/{zone_id}/fans/{fan_id}", &fan_handler);

	register_handler("/rack/thermalzones/{zone_id}/fans/rf_event", &fan_coll_evt_handler);
	register_handler("/rack/thermalzones/{zone_id}/fans/{fan_id}/rf_event", &fan_evt_handler);

	register_handler("/rack/thermalzones/{zone_id}/fans/rf_listener", &fan_coll_listener_handler);
	register_handler("/rack/thermalzones/{zone_id}/fans/{fan_id}/rf_listener", &fan_listener_handler);
}

static json_t *tzone_coll_get(struct rest_uri_param *param)
{
	collections_t *tzone_collections = NULL;
	uint32 tzone_num = 0;
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *tzone = NULL;
	json_t *array = NULL;
	int32 i;
	int32 zone_num = 0;

	zone_num = libwrap_get_asset_num(MC_TYPE_TZONE);
	if (zone_num == 0) {
		HTTPD_ERR("get thermal zone num fail\n");
		return NULL;
	}

	tzone_collections = (collections_t *)malloc(zone_num * sizeof(collections_t));
	if (tzone_collections == NULL)
		return NULL;

	memset(tzone_collections, 0, zone_num * sizeof(collections_t));

	rs = libwrap_get_tzone_coll(tzone_collections, &tzone_num);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get cooling zone collection fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	array = json_array();
	if (array == NULL) {
		HTTPD_ERR("json array request error\n");
		return NULL;
	}

	for (i = 0; i < tzone_num; i++) {
		tzone = NULL;
		tzone = json_object();
		if (tzone != NULL) {
			add_json_integer(tzone, RMM_JSON_ID, tzone_collections[i].id);
			add_json_string(tzone, RMM_JSON_UUID, tzone_collections[i].uuid);
			add_json_string(tzone, RMM_JSON_NAME, tzone_collections[i].name);
			update_href_host(tzone_collections[i].href, HREF_URL_LEN, param->host);
			add_json_string(tzone, RMM_JSON_HREF, tzone_collections[i].href);
			json_array_add(array, tzone);
		}
	}
	json_object_add(result, RMM_JSON_THERMAL_ZONES, array);

	if (tzone_collections)
		free(tzone_collections);

	return result;
}

static json_t *tzone_coll_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *tzone_coll_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *tzone_coll_evt_get(struct rest_uri_param *param)
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
	get_evt_links(&(service.links), MC_REDFISH_TZONE, reg, RF_EVENT_CZONE_COLL_FMT);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *tzone_coll_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *tzone_coll_evt_post(struct rest_uri_param *param)
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

	return process_listener(req, &listener, mask, MC_REDFISH_TZONE);
}

#if 0
static int32 pack_tzone_basic_info(const tzone_member_t *tzone_member, json_t *result)
{
	add_json_integer(result, RMM_JSON_TZONE_ID, tzone_member->be.id);
	add_json_string(result, RMM_JSON_UUID, tzone_member->be.uuid);
	add_json_string(result, RMM_JSON_NAME, tzone_member->be.name);
	add_json_string(result, RMM_JSON_DESC, tzone_member->be.desc);
	add_json_string(result, RMM_JSON_CREATED_DATE, tzone_member->be.create_date);
	add_json_string(result, RMM_JSON_UPDATED_DATE, tzone_member->be.update_date);

	return 0;
}
#endif

static json_t *tzone_get(struct rest_uri_param *param)
{
	tzone_member_t tzone_member = { {0} };
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *loc = NULL;
	int32 tzone_idx = 0;
	int32 presence_len = 0;

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	rs = libwrap_get_tzone_by_idx(tzone_idx, &tzone_member);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get cooling zone fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request fail\n");
		return NULL;
	}

	/*pack_tzone_basic_info(&tzone_member, result);*/
	pack_basic_element_info((const base_element_t *)&(tzone_member.be), result);

	loc = json_object();
	if (loc != NULL) {
		add_loc_info(loc, (int32)tzone_member.tzone_loc.units, RMM_JSON_RACK_UNITS);
		add_loc_info(loc, tzone_member.tzone_loc.u_location, RMM_JSON_ULOC);
		add_loc_info(loc, tzone_member.tzone_loc.u_height, RMM_JSON_UHEIGHT);
        add_loc_info(loc, (int32)tzone_member.tzone_loc.x_location, RMM_JSON_XLOC);
	}
	json_object_add(result, RMM_JSON_RACK_LOC, loc);

	add_json_string(result, RMM_JSON_PRESENCE, (uint8 *)(tzone_member.presence));
	add_json_string(result, RMM_JSON_POLICY, tzone_member.policy);

	add_json_integer(result, RMM_JSON_PRESENT_TEMP, tzone_member.pres_temp);
	add_json_integer(result, RMM_JSON_OUT_LET_TEMP, tzone_member.outlet_temp);
	add_json_integer(result, RMM_JSON_AIRFLOW, tzone_member.volumetric_airflow);
	
	//TBD: it can be saved in memdb
	add_json_integer(result, RMM_JSON_MAX_FANS_SUPPORTED, MAX_PWM_NUM);
	presence_len = strlen(tzone_member.presence);
	if ((presence_len > -1) && (presence_len < 9)) {
		int32 i = 0;
		int32 number_fan = 0;
		for(i = 0; i < presence_len; i++) {
			if (tzone_member.presence[i] == 49)//"1"
				number_fan++;
		}
		add_json_integer(result, RMM_JSON_NUM_OF_FANS_PRESENT, number_fan);
	}

	return result;
}

static json_t *tzone_put(struct rest_uri_param *param)
{
	json_t *req = NULL;
	result_t rs = RESULT_OK;
	put_tzone_t put_tzone_info = { {0} };
	int8 *description = NULL;
	json_t *rs_json = NULL;
	int32 tzone_idx = 0;

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	rs = libwrap_pre_put_tzone_by_idx(tzone_idx, &put_tzone_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("cooling zone pre put fail, result is %d\n", rs);
		return NULL;
	}

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	put_prepare_str(req, put_tzone_info.descr, DESCRIPTION_LEN, RMM_JSON_DESC);

	rs = libwrap_put_tzone_by_idx(tzone_idx, put_tzone_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("cooling zone put error, result is %d\n", rs);
		return NULL;
	}

	json_free(req);
	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", tzone_idx);
	rf_log(INFO, MSGTZoneUpdate, buff);
	return tzone_get(param);
}

static json_t *tzone_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *tzone_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	json_t *result = NULL;
	int32 tzone_idx, reg = 0;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));
	reg = 1 << (tzone_idx - 1);
	get_evt_links(&(service.links), MC_REDFISH_TZONE, reg, RF_EVENT_CZONE_FMT, tzone_idx);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *tzone_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *tzone_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	int32 reg = 0;
	listener_dest_t listener;
	int32 tzone_idx;

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	update_response_info(param, HTTP_APPLICATION_ERROR);

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);

	reg = 1 << (tzone_idx - 1);

	return process_listener(req, &listener, reg, MC_REDFISH_TZONE);
}


static json_t *fan_coll_get(struct rest_uri_param *param)
{
	collections_t *tzone_fan_coll = NULL;
	uint32 fan_num = 0;
	int8 *p_tzone_id = NULL;
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *fan = NULL;
	json_t *array = NULL;
	int32 tzone_idx = 0;
	int32 i;

	tzone_fan_coll = (collections_t *)malloc(MAX_PWM_NUM * sizeof(collections_t));
	if (tzone_fan_coll == NULL)
		return NULL;

	memset(tzone_fan_coll, 0, MAX_PWM_NUM * sizeof(collections_t));

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	rs = libwrap_get_tzone_fan_coll(tzone_idx, tzone_fan_coll, &fan_num);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get fan collection fail, result is %d\n", rs);
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

	for (i = 0; i < fan_num; i++) {
		fan = NULL;
		fan = json_object();
		if (NULL != fan) {
			add_json_integer(fan, RMM_JSON_ID, tzone_fan_coll[i].id);
			add_json_string(fan, RMM_JSON_UUID, tzone_fan_coll[i].uuid);
			add_json_string(fan, RMM_JSON_NAME, tzone_fan_coll[i].name);
			update_href_host(tzone_fan_coll[i].href, HREF_URL_LEN, param->host);
			add_json_string(fan, RMM_JSON_HREF, tzone_fan_coll[i].href);
			json_array_add(array, fan);
		}
	}
	json_object_add(result, RMM_JSON_FANS, array);

	if (tzone_fan_coll)
		free(tzone_fan_coll);

	return result;
}

static json_t *fan_coll_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *fan_coll_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *fan_coll_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	json_t *result = NULL;
	int32 tzone_idx, reg = 0;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));
	reg |= RF_EVENT_MASK_ALL;
	get_evt_links(&(service.links), MC_REDFISH_FAN_COLL, reg, RF_EVENT_FAN_COLL_FMT, tzone_idx);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *fan_coll_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *fan_coll_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	int32 reg = 0;
	listener_dest_t listener;
	int32 tzone_idx;

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
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

	return process_listener(req, &listener, reg, MC_REDFISH_FAN_COLL);
}

static int32 pack_fan_basic_info(const fan_member_t *fan_member, json_t *result)
{
	pack_basic_element_info(&(fan_member->be), result);
	/*
	add_json_integer(result, RMM_JSON_FAN_ID, fan_member->be.id);
	add_json_string(result, RMM_JSON_UUID, fan_member->be.uuid);
	*/
	add_json_integer(result, RMM_JSON_ENABLE_STATE, fan_member->enable_state);
	add_json_integer(result, RMM_JSON_HEALTH_STATE, fan_member->health_state);
	add_json_string(result, RMM_JSON_DESIRED_SPEED, fan_member->desired_speed);
	add_json_integer(result, RMM_JSON_TACHOMETER_READING, fan_member->tachometer.reading);
	add_json_string(result, RMM_JSON_METRE_STATE, fan_member->tachometer.current_state);

	return 0;
}

static int32 fan_add_property(json_t *threshold, json_t *property, int32 value, const int8 *name)
{
	property = NULL;
	property = json_object();
	if (property != NULL) {
		add_json_integer(property, name, value);
		json_array_add(threshold, property);
	}

	return 0;
}

static json_t *fan_get(struct rest_uri_param *param)
{
	fan_member_t fan_member = { {0} };
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *threshold = NULL;
	json_t *av_action = NULL;
	json_t *fru = NULL;
	json_t *property = NULL;
	json_t *action = NULL;
	json_t *cap = NULL;
	int32 tzone_idx = 0, fan_idx = 0;
	json_t *loc = NULL;

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	fan_idx = get_asset_idx(param, "fan_id", MC_TYPE_FAN);
	if (fan_idx == -1) {
		HTTPD_ERR("get fan index fail\n");
		return NULL;
	}

	rs = libwrap_get_tzone_fan_by_idx(tzone_idx, fan_idx,  &fan_member);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get fan fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request fail\n");
		return NULL;
	}

	pack_fan_basic_info(&fan_member, result);

	loc = json_object();
	if (loc != NULL) {
		add_loc_info(loc, (int32)fan_member.fan_loc.units, RMM_JSON_RACK_UNITS);
		add_loc_info(loc, fan_member.fan_loc.u_location, RMM_JSON_ULOC);
		add_loc_info(loc, fan_member.fan_loc.u_height, RMM_JSON_UHEIGHT);
        add_loc_info(loc, fan_member.fan_loc.x_location, RMM_JSON_XLOC);
	}
	json_object_add(result, RMM_JSON_RACK_LOC, loc);

	threshold = NULL;
	threshold = json_array();
	if (threshold == NULL) {
		HTTPD_ERR("threshold array request error\n");
		return NULL;
	}

	fan_add_property(threshold, property, fan_member.tachometer.threshold.lower_non_critical, RMM_JSON_LOWER_NON_CRITICAL);
	fan_add_property(threshold, property, fan_member.tachometer.threshold.upper_non_critical, RMM_JSON_UPPER_NON_CRITICAL);
	fan_add_property(threshold, property, fan_member.tachometer.threshold.lower_critical, RMM_JSON_LOWER_CRITICAL);
	fan_add_property(threshold, property, fan_member.tachometer.threshold.upper_critical, RMM_JSON_UPPER_CRITICAL);

	json_object_add(result, RMM_JSON_THRESHOLD, threshold);
	add_json_string(result, RMM_JSON_PRESENCE, fan_member.presence);
	add_json_string(result, RMM_JSON_ASSET_TAG, fan_member.asset.asset_tag);

	add_json_integer(result, RMM_JSON_SPEED_UNIT, fan_member.speed_unit);// 0--PWM, 1---RPM

	fru = json_object();
	if (fru != NULL) {
		pack_fru_info(&(fan_member.asset.fru), fru);
		json_object_add(result, RMM_JSON_FRU_INFO, fru);
	}

	av_action = json_array();
	if (av_action == NULL) {
		HTTPD_ERR("available action array request fail\n");
		return NULL;
	}

	/* requestStateChange */
	action = json_object();
	if (0 != prepare_get_action(fan_member.av_action, action, 0)) {
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
			if (0 != strcmp((int8 *)(fan_member.av_action[0].cap[0].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(fan_member.av_action[0].cap[0].property)));
				json_array_add(allow_values, json_integer(str2int((int8 *)fan_member.av_action[0].cap[0].av)));
			}
		}

		property = NULL;
		property = json_object();
		if ((property != NULL) && (allow_values != NULL)) {
			if (0 != strcmp((int8 *)(fan_member.av_action[0].cap[1].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(fan_member.av_action[0].cap[1].property)));
				json_array_add(allow_values, json_integer(str2int((int8 *)fan_member.av_action[0].cap[1].av)));
			}
		}
		json_object_add(property, RMM_JSON_ALLOWABLE_VALUES, allow_values);
		json_array_add(cap, property);
		json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	}
	json_array_add(av_action, action);

	/* SetDesiredSpeed */
	action = NULL;
	action = json_object();
	if (0 != prepare_get_action(fan_member.av_action, action, 1)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	cap = NULL;
	cap = json_array();
	if (cap != NULL) {
		json_t *property = NULL;
		json_t *allow_values = NULL;

		allow_values = json_array();
		property = json_object();
		if ((property != NULL) && (allow_values != NULL)) {
			if (0 != strcmp((int8 *)(fan_member.av_action[1].cap[0].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(fan_member.av_action[1].cap[0].property)));
				json_array_add(allow_values, json_string((int8 *)(fan_member.av_action[1].cap[0].av)));
			}

			if (0 != strcmp((int8 *)(fan_member.av_action[1].cap[1].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(fan_member.av_action[1].cap[1].property)));
				json_array_add(allow_values, json_string((int8 *)(fan_member.av_action[1].cap[1].av)));
			}
			json_object_add(property, RMM_JSON_ALLOWABLE_VALUES, allow_values);
			json_array_add(cap, property);
		}

		property = NULL;
		property = json_object();
		if (property != NULL) {
			if (0 != strcmp((int8 *)(fan_member.av_action[1].cap[2].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(fan_member.av_action[1].cap[2].property)));
				json_array_add(cap, property);
			}
		}
		json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	}
	json_array_add(av_action, action);
	json_object_add(result, RMM_JSON_AVALIBLE_ACTIONS, av_action);
	return result;
}

static json_t *fan_put(struct rest_uri_param *param)
{
	json_t *req;
	json_t *TachoMeterThreshold;
	result_t rs = RESULT_OK;
	put_fan_t put_fan_info = { {0} };
	uint32 value;
	json_t *elem = NULL;
	json_t *obj = NULL;
	int32 i = 0;
	int32 array_size;
	int32 tzone_idx, fan_idx = 0;

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	fan_idx = get_asset_idx(param, "fan_id", MC_TYPE_FAN);
	if (fan_idx == -1) {
		HTTPD_ERR("get fan index fail\n");
		return NULL;
	}

	rs = libwrap_pre_put_fan(tzone_idx, fan_idx, &put_fan_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("fan pre put fail, result is %d\n", rs);
		return NULL;
	}

	req = json_parse(param->json_data);
	put_prepare_str(req, put_fan_info.descr, DESCRIPTION_LEN, RMM_JSON_DESC);
	put_prepare_str(req, put_fan_info.asset_tag, REST_ASSET_TAG_LEN, RMM_JSON_ASSET_TAG);
	TachoMeterThreshold = json_object_get(req, RMM_JSON_THRESHOLD);
	if (TachoMeterThreshold != NULL) {
		array_size = json_array_size(TachoMeterThreshold);
		for (i = 0; i < array_size; i++) {
			elem = NULL;
			elem = json_array_get(TachoMeterThreshold, i);
			if (elem == NULL) {
				HTTPD_ERR("tacho meter thresh get error\n");
				return NULL;
			}
			put_prepare_int(elem, &(put_fan_info.threshold.lower_non_critical), RMM_JSON_LOWER_NON_CRITICAL);
			put_prepare_int(elem, &(put_fan_info.threshold.upper_non_critical), RMM_JSON_UPPER_NON_CRITICAL);
			put_prepare_int(elem, &(put_fan_info.threshold.lower_critical), RMM_JSON_LOWER_CRITICAL);
			put_prepare_int(elem, &(put_fan_info.threshold.upper_critical), RMM_JSON_UPPER_CRITICAL);
		}
	}
	
	rs = libwrap_put_fan(tzone_idx, fan_idx, put_fan_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("fan put fail, result is %d\n", rs);
		return NULL;
	}
	json_free(req);

	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", ((tzone_idx - 1) * MAX_PWM_NUM + fan_idx));
	rf_log(INFO, MSGFanUpdate, buff);
	return fan_get(param);
}

static json_t *fan_handle_change_state(json_t *req, int32 tzone_idx, int32 fan_idx, struct rest_uri_param *param)
{
	int64 enabled_state = 0;
	result_t rs = RESULT_OK;

	enabled_state = json_integer_value(json_object_get(req, RMM_JSON_ENABLE_STATE));

	if (!((enabled_state == FAN_STATE_ENABLED) || (enabled_state == FAN_STATE_DISABLED))) {
		HTTPD_ERR("unknown fan state\n");
		update_response_info(param, HTTP_APPLICATION_ERROR);
		json_free(req);
		return NULL;
	}

	rs = libwrap_post_fan_change_state(tzone_idx, fan_idx, enabled_state);
	if (rs != RESULT_OK) {
		HTTPD_ERR("fan state post fail, result is %d\n", rs);
		update_response_info(param, HTTP_APPLICATION_ERROR);
	} else {
		update_response_info(param, HTTP_ACCEPTED);
	}
	json_free(req);

	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", (tzone_idx - 1) * MAX_PWM_NUM + fan_idx);
	rf_snmp_evt(INFO, MSGFanChange, buff, "enable state change");

	return NULL;
}

static json_t *fan_handle_set_speed(json_t *req, int32 tzone_idx, int32 fan_idx, struct rest_uri_param *param)
{
	int8 *unit_type = NULL;
	int8 *speed = NULL;
	uint32 u_type = 0;
	result_t rs = RESULT_OK;

	unit_type = json_string_value(json_object_get(req, RMM_JSON_UNIT_TYPE));
	if (unit_type == NULL) {
		HTTPD_ERR("failed to get json <unitType>.\n");
		goto err;
	}

	if (0 == strcmp(unit_type, RMM_JSON_RPM))
		u_type = 0;
	else if ((0 == strcmp(unit_type, RMM_JSON_PWM)))
		u_type = 1;
	else {
		HTTPD_ERR("json string <unitType> is not right.\n");
		goto err;
	}

	speed = json_string_value(json_object_get(req, RMM_JSON_DESIRED_SPEED));
	if (speed == NULL) {
		HTTPD_ERR("failed to get json string <desiredSpeed>.\n");
		goto err;
	}

	rs = libwrap_post_fan_set_speed(tzone_idx, fan_idx, u_type, speed);
	if (rs != RESULT_OK) {
		HTTPD_ERR("fan speed post fail, result is %d\n", rs);
		update_response_info(param, HTTP_APPLICATION_ERROR);
	} else {
		update_response_info(param, HTTP_ACCEPTED);
	}
	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", (tzone_idx - 1) * MAX_PWM_NUM + fan_idx);
	rf_snmp_evt(INFO, MSGFanChange, buff, "speed change");

err:
	json_free(req);
	return NULL;
}

static json_t *fan_post(struct rest_uri_param *param)
{
	json_t *req;
	int8 *action = NULL;
	int32 tzone_idx = 0, fan_idx = 0;

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	fan_idx = get_asset_idx(param, "fan_id", MC_TYPE_FAN);
	if (fan_idx == -1) {
		HTTPD_ERR("get fan index fail\n");
		return NULL;
	}

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	action = json_string_value(json_object_get(req,  RMM_JSON_ACTION));
	if (action == NULL) {
		HTTPD_ERR("failed to get json string <action>\n");
		json_free(req);
		return NULL;
	}

	if (0 == strcmp(action, RMM_JSON_REQ_STATE_CHANGE))
		return fan_handle_change_state(req, tzone_idx, fan_idx, param);
	else if (0 == strcmp(action, RMM_JSON_SET_DESIRED_SPEED))
		return fan_handle_set_speed(req, tzone_idx, fan_idx, param);

	json_free(req);
	return NULL;
}

static json_t *fan_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	json_t *result = NULL;
	int32 tzone_idx, fan_idx, reg = 0;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}
	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	fan_idx = get_asset_idx(param, "fan_id", MC_TYPE_FAN);
	if (fan_idx == -1) {
		HTTPD_ERR("get psu zone index fail\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));
	reg = 1 << ((tzone_idx - 1) * MAX_PWM_NUM + (fan_idx - 1));
	get_evt_links(&(service.links), MC_REDFISH_FAN_COLL, reg, RF_EVENT_FAN_FMT, tzone_idx, fan_idx);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *fan_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *fan_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	listener_dest_t listener;
	int32 tzone_idx, fan_idx;
	int32 mask = 0;

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	fan_idx = get_asset_idx(param, "fan_id", MC_TYPE_FAN);
	if (fan_idx == -1) {
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

	mask = 1 << ((tzone_idx - 1) * MAX_PWM_NUM + (fan_idx - 1));

	return process_listener(req, &listener, mask, MC_REDFISH_FAN_COLL);
}

static json_t *tzone_coll_listener_get(struct rest_uri_param *param)
{
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_TZONE, INVAILD_IDX, INVAILD_IDX, header);
	return get_json_listener(header);
}

static json_t *tzone_listener_get(struct rest_uri_param *param)
{
	int32 tzone_idx = 0;
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_TZONE, INVAILD_IDX, tzone_idx - 1, header);

	return	get_json_listener(header);
}

static json_t *fan_coll_listener_get(struct rest_uri_param *param)
{
	int32 pzone_idx = 0;
	int32 fan_idx = INVAILD_IDX;
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	pzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (pzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_FAN_COLL, pzone_idx, fan_idx, header);

	return	get_json_listener(header);
}

static json_t *fan_listener_get(struct rest_uri_param *param)
{
	int32 tzone_idx = 0;
	int32 fan_idx = 0;
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	tzone_idx = get_asset_idx(param, "zone_id", MC_TYPE_TZONE);
	if (tzone_idx == -1) {
		HTTPD_ERR("get cooling zone index fail\n");
		return NULL;
	}

	fan_idx = get_asset_idx(param, "fan", MC_TYPE_PSU);
	if (fan_idx == -1) {
		HTTPD_ERR("get fan index fail\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_FAN_COLL, tzone_idx, (tzone_idx - 1) * MAX_PWM_NUM + fan_idx, header);
	return get_json_listener(header);

}

static struct rest_handler tzone_coll_handler = {
	.get    = tzone_coll_get,
	.put    = tzone_coll_put,
	.post   = tzone_coll_post,
};

static struct rest_handler tzone_handler = {
	.get    = tzone_get,
	.put    = tzone_put,
	.post   = tzone_post,
};

static struct rest_handler fan_coll_handler = {
	.get    = fan_coll_get,
	.put    = fan_coll_put,
	.post   = fan_coll_post,
};

static struct rest_handler fan_handler = {
	.get    = fan_get,
	.put    = fan_put,
	.post   = fan_post,
};

static struct rest_handler tzone_coll_evt_handler = {
	.get    = tzone_coll_evt_get,
	.put    = tzone_coll_evt_put,
	.post   = tzone_coll_evt_post,
};

static struct rest_handler tzone_evt_handler = {
	.get    = tzone_evt_get,
	.put    = tzone_evt_put,
	.post   = tzone_evt_post,
};

static struct rest_handler fan_coll_evt_handler = {
	.get    = fan_coll_evt_get,
	.put    = fan_coll_evt_put,
	.post   = fan_coll_evt_post,
};

static struct rest_handler fan_evt_handler = {
	.get    = fan_evt_get,
	.put    = fan_evt_put,
	.post   = fan_evt_post,
};

static struct rest_handler tzone_coll_listener_handler = {
	.get    = tzone_coll_listener_get,
	.put    = NULL,
	.post   = NULL,
};

static struct rest_handler tzone_listener_handler = {
	.get    = tzone_listener_get,
	.put    = NULL,
	.post   = NULL,
};

static struct rest_handler fan_coll_listener_handler = {
	.get    = fan_coll_listener_get,
	.put    = NULL,
	.post   = NULL,
};

static struct rest_handler fan_listener_handler = {
	.get    = fan_listener_get,
	.put    = NULL,
	.post   = NULL,
};


