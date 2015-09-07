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
#include "librmmcfg/rmm_cfg.h"

static struct rest_handler mbp_coll_handler;
static struct rest_handler mbp_handler;
static struct rest_handler mbp_coll_evt_handler;
static struct rest_handler mbp_evt_handler;
static struct rest_handler mbp_coll_listener_handler;
static struct rest_handler mbp_listener_handler;


void register_mbp_handler(void)
{
	register_handler("/rack/mbps", &mbp_coll_handler);
	register_handler("/rack/mbps/{mbp_id}", &mbp_handler);

	register_handler("/rack/mbps/rf_event", &mbp_coll_evt_handler);
	register_handler("/rack/mbps/{mbp_id}/rf_event", &mbp_evt_handler);

	register_handler("/rack/mbps/rf_listener", &mbp_coll_listener_handler);
	register_handler("/rack/mbps/{mbp_id}/rf_listener", &mbp_listener_handler);
}

static int32 process_firmware_update(int32 idx, int8 *dev, int8 *data, int32 length)
{
	int32 rc = -1;
	int8 *w_buf = NULL;
	int32 w_fd = -1;
	int32 len = 0;
	int32 offset = 0;
	int32 decode_size = 0;
	int32 buff_len = 0;
	struct stat sb;
	int8 file_name[128] = {0};

	if ((data == NULL) || (length <= 0))
		return -1;

	snprintf(file_name, sizeof(file_name), "%s/%s", FIRMWARE_FILE_PATH, FIRMWARE_FILE_NAME);

	if (stat(FIRMWARE_FILE_PATH, &sb) == -1) {
		if (errno == ENOENT)
			mkdir(FIRMWARE_FILE_PATH, 0777);
	}

	w_fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	if (w_fd < 0) {
		perror("process update file w");
		return -1;
	}

	buff_len = length;

	while (offset < buff_len) {
		/* Decode the file first. */
		decode_size = ((offset + MAX_READ_BASE64_SIZE)
					   < buff_len ? MAX_READ_BASE64_SIZE
					   : (buff_len - offset));

		w_buf = base64_decode(data + offset, decode_size, &len);
		if (NULL == w_buf)
			goto err;

		offset += decode_size;

		rc = write(w_fd, w_buf, len);
		if (rc < 0)
			goto err;

		free(w_buf);
		w_buf = NULL;
	}

	close(w_fd);

	return libwrap_update_mbp_firmware(idx, dev, FIRMWARE_FILE_NAME);

err:
	if (w_buf != NULL) {
		free(w_buf);
		w_buf = NULL;
	}

	close(w_fd);
	remove(file_name);

	return -1;
}

static json_t *mbp_coll_get(struct rest_uri_param *param)
{
	collections_t *mbp_collections = NULL;
	uint32 mbp_num = 0;
	int32 i;
	int32 cm_num = 0;
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *array = NULL;
	json_t *sub_mbp = NULL;

	cm_num = libwrap_get_asset_num(MC_TYPE_CM);
	if (cm_num == 0) {
		HTTPD_ERR("get mbp num fail\n");
		return NULL;
	}

	mbp_collections = (collections_t *)malloc(cm_num * sizeof(collections_t));
	if (mbp_collections == NULL)
		return NULL;

	memset(mbp_collections, 0, cm_num * sizeof(collections_t));

	rs = libwrap_get_mbp_coll(mbp_collections, &mbp_num);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get mbp coll info fail, result is %d\n", rs);
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

	for (i = 0; i < mbp_num; i++) {
		sub_mbp = NULL;
		sub_mbp = json_object();
		if (sub_mbp == NULL) {
			HTTPD_ERR("sub mbp json object request fail\n");
			return NULL;
		}

		add_json_integer(sub_mbp, RMM_JSON_ID, mbp_collections[i].id);
		add_json_string(sub_mbp, RMM_JSON_UUID, mbp_collections[i].uuid);
		add_json_string(sub_mbp, RMM_JSON_NAME, mbp_collections[i].name);
		update_href_host(mbp_collections[i].href, HREF_URL_LEN, param->host);
		add_json_string(sub_mbp, RMM_JSON_HREF, mbp_collections[i].href);
		json_array_add(array, sub_mbp);
	}
	json_object_add(result, RMM_JSON_MBPS, array);

	if (mbp_collections)
		free(mbp_collections);

	return result;
}

static json_t *mbp_coll_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *mbp_coll_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *mbp_coll_evt_get(struct rest_uri_param *param)
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
	get_evt_links(&(service.links), MC_REDFISH_MBP_COLL, reg, RF_EVENT_MBP_COLL_FMT);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *mbp_coll_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *mbp_coll_evt_post(struct rest_uri_param *param)
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

	return process_listener(req, &listener, mask, MC_REDFISH_MBP_COLL);
}

static int32 pack_mbp_basic_info(mbp_member_t *mbp_member, json_t *result)
{
	pack_basic_element_info(&(mbp_member->be), result);
	/*
	add_json_integer(result, RMM_JSON_MBP_ID, mbp_member->be.id);
	add_json_string(result, RMM_JSON_UUID, mbp_member->be.uuid);
	*/
	add_json_string(result, RMM_JSON_MBP_MBPID, mbp_member->MBPID);
	add_json_string(result, RMM_JSON_HW_ADDR, mbp_member->MBPHWAddress);
	add_json_string(result, RMM_JSON_IP_ADDR, mbp_member->MBPIPAddress);
	add_json_string(result, RMM_JSON_FW_STATE, mbp_member->fw_state);

	return 0;
}

static json_t *mbp_get(struct rest_uri_param *param)
{
	int32 idx = 0;
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *fru = NULL;
	json_t *av_action = NULL;
	json_t *rs_json = NULL;
	json_t *cap = NULL;
	mbp_member_t mbp_member = { {0} };
	json_t *loc = NULL;

	idx = get_asset_idx(param, "mbp_id", MC_TYPE_CM);
	if (idx == -1) {
		HTTPD_ERR("get mbp index fail\n");
		return NULL;
	}

	rs = libwrap_get_mbp_by_idx(idx, &mbp_member);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get mbp info fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request fail\n");
		return NULL;
	}

	pack_mbp_basic_info(&mbp_member, result);

	loc = json_object();
	if (loc == NULL) {
		HTTPD_ERR("loc json object request fail\n");
		return NULL;
	}

    add_loc_info(loc, (int32)mbp_member.loc.units, RMM_JSON_RACK_UNITS);
	add_loc_info(loc, mbp_member.loc.u_location, RMM_JSON_ULOC);
	add_loc_info(loc, mbp_member.loc.u_height, RMM_JSON_UHEIGHT);
	add_loc_info(loc, mbp_member.loc.x_location, RMM_JSON_XLOC);
	json_object_add(result, RMM_JSON_RACK_LOC, loc);

	add_json_string(result, RMM_JSON_ASSET_TAG, mbp_member.asset.asset_tag);

	fru = json_object();
	if (fru != NULL) {
		pack_fru_info(&(mbp_member.asset.fru), fru);
		json_object_add(result, RMM_JSON_FRU_INFO, fru);
	}

	av_action = json_array();
	if (av_action == NULL) {
		HTTPD_ERR("available action array request fail\n");
		return NULL;
	}

	/* action reset */
	json_t *action = NULL;

	action = json_object();
	if (0 != prepare_get_action(mbp_member.av_action, action, 0)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	cap = json_array();
	if (cap != NULL) {
		json_t *property = NULL;
		json_t *allow_values = NULL;

		allow_values = json_array();
		/* soft reset */
		property = json_object();
		if ((property != NULL) && (allow_values != NULL)) {
			if (0 != strcmp((int8 *)(mbp_member.av_action[0].cap[0].property), "")) {
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(mbp_member.av_action[0].cap[0].property)));
				json_array_add(allow_values, json_string((int8 *)(mbp_member.av_action[0].cap[0].av)));
			}
			json_object_add(property, RMM_JSON_ALLOWABLE_VALUES, allow_values);
			json_array_add(cap, property);
		}
		json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	}
	json_array_add(av_action, action);


	/* action SetUartTarget */
	action = NULL;
	action = json_object();
	if (0 != prepare_get_action(mbp_member.av_action, action, 1)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	cap = NULL;
	cap = json_array();
	if (cap != NULL) {
		json_t *property = NULL;

		/* targetIndex */
		property = json_object();
		if (property != NULL) {
			if (0 != strcmp((int8 *)(mbp_member.av_action[1].cap[0].property), ""))
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(mbp_member.av_action[1].cap[0].property)));
			json_array_add(cap, property);
		}
		json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	}
	json_array_add(av_action, action);

	/* action Update */
	action = NULL;
	action = json_object();
	if (prepare_get_action(mbp_member.av_action, action, 2)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	cap = NULL;
	cap = json_array();
	if (cap != NULL) {
		json_t *property = NULL;

		property = json_object();
		if (property != NULL) {
			if (0 != strcmp((int8 *)(mbp_member.av_action[2].cap[0].property), ""))
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(mbp_member.av_action[2].cap[0].property)));
			json_array_add(cap, property);
		}
		json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	}
	json_array_add(av_action, action);

	json_object_add(result, RMM_JSON_AVALIBLE_ACTIONS, av_action);

	return result;
}

static json_t *mbp_put(struct rest_uri_param *param)
{
	json_t *req;
	result_t rs = RESULT_OK;
	put_mbp_t put_mbp_info = { {0} };
	int32 idx;

	idx = get_asset_idx(param, "mbp_id", MC_TYPE_CM);
	if (idx == -1) {
		HTTPD_ERR("get mbp index fail\n");
		return NULL;
	}

	rs = libwrap_pre_put_mbp(idx, &put_mbp_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("pre put error, result is %d\n", rs);
		return NULL;
	}

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("parse json data error\n");
		return NULL;
	}

	put_prepare_str(req, put_mbp_info.descr, DESCRIPTION_LEN, RMM_JSON_DESC);
	put_prepare_str(req, put_mbp_info.asset_tag, REST_ASSET_TAG_LEN, RMM_JSON_ASSET_TAG);

	rs = libwrap_put_mbp(idx, put_mbp_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("put mbp error %d\n", rs);
		return NULL;
	}

	json_free(req);
	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", idx);
	rf_log(INFO, MSGMbpUpdate, buff, RMM_JSON_UPDATE);
	return mbp_get(param);
}

static json_t *mbp_process_reset(json_t *req, int32 idx, struct rest_uri_param *param)
{
	int64 reset_type = 0;
	result_t rs = RESULT_OK;
	int32 reset_mode = 0;
	int32 reset_sys_result;

	rs = libwrap_mbp_hard_reset(idx, &reset_sys_result);

	if (rs != RESULT_OK) {
		HTTPD_ERR("reset mbp fail, result is %d\n", rs);
		return NULL;
	}
	update_response_info(param, HTTP_ACCEPTED);

	json_free(req);

	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", idx);
	rf_snmp_evt(INFO, MSGMbpChange, buff, RMM_JSON_RESETED_BIG);
	return NULL;
}

static json_t *mbp_set_uart_target(json_t *req, int32 cm_lid, struct rest_uri_param *param)
{
	int64 reset_type = 0;
	result_t rs = RESULT_OK;
	int32 reset_mode = 0;
	int32 reset_sys_result;
	int32 tgt_idx = 0;
	int32 tray_idx = 0;

	if (is_platform_bdcr()) {
		tray_idx = json_integer_value(json_object_get(req, RMM_JSON_TARGET_TRAY));
		if (tray_idx == -1) {
			HTTPD_ERR("get tray index error\n");
			json_free(req);
			return NULL;
		}
	}

	tgt_idx = json_integer_value(json_object_get(req, RMM_JSON_TARGET_INDEX));
	if (tgt_idx == -1) {
		HTTPD_ERR("get target index error\n");
		json_free(req);
		return NULL;
	}

	rs = libwrap_mbp_uart_switch(cm_lid, tray_idx, tgt_idx);

	if (rs != RESULT_OK) {
		HTTPD_ERR("set uart switch fail, result is %d\n", rs);
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);
	json_free(req);

	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d",cm_lid);
	rf_snmp_evt(INFO, MSGMbpChange, buff, "set uart target");
	return NULL;
}

static json_t *mbp_process_update(json_t *req, int32 idx, struct rest_uri_param *param)
{
	json_t *result = NULL;
	int8 *image_data = NULL;
	json_t *image_obj = NULL;
	int32 rc = -1;
	int32 cm_lid = 0;
	int8 cm_dev[64] = {0};

	/* check update capability */
	if(libwrap_check_update_capability() != RESULT_OK) {
		HTTPD_ERR("firmware update is not supported.\n");
		return NULL;
	}

	if (!libwrap_get_firmware_update_status()) {
		HTTPD_ERR("get firmware update status fail\n");
		return NULL;
	}

	libwrap_set_firmware_update_status(0);

	result = json_object();
	if (result == NULL) {
		update_response_info(param, HTTP_APPLICATION_ERROR);
		libwrap_set_firmware_update_status(1);
		return NULL;
	}
	rc = libwrap_check_tftp_service();
	if (rc == -1) {
		json_free(req);
		json_object_add(result, RMM_JSON_FRU_RESULT, json_string("tftp server not ready"));
		update_response_info(param, HTTP_NOT_ACCEPTABLE);
		libwrap_set_firmware_update_status(1);
		return result;
	}

	image_obj = json_object_get(req, RMM_JSON_IMAGE);
	if (image_obj == NULL) {
		json_free(req);
		json_object_add(result, RMM_JSON_FRU_RESULT, json_string("invalid image"));
		update_response_info(param, HTTP_NOT_ACCEPTABLE);
		libwrap_set_firmware_update_status(1);
		return result;
	}

	image_data = json_string_value(image_obj);
	if (image_data == NULL) {
		json_free(req);
		json_object_add(result, RMM_JSON_FRU_RESULT, json_string("invalid image"));
		update_response_info(param, HTTP_NOT_ACCEPTABLE);
		libwrap_set_firmware_update_status(1);
		return result;
	}

	cm_lid = idx;
	if ((cm_lid == 1) || (cm_lid == 2)) {
		snprintf(cm_dev, sizeof(cm_dev), "/dev/ttyCm%dIPMI", cm_lid);
		rc = process_firmware_update(cm_lid, cm_dev, image_data, strlen(image_data));
	} else {
		HTTPD_ERR("invalid cm loc id\n");
		json_free(req);
		libwrap_set_firmware_update_status(1);
		return NULL;
	}

	if (rc == 0) {
		json_free(req);
		json_object_add(result, RMM_JSON_FRU_RESULT, json_string(RMM_JSON_UPDATING));
		update_response_info(param, HTTP_ACCEPTED);
		return result;
	} else {
		libwrap_set_firmware_update_status(1);
	}

	json_free(req);

	int8 buff[128] = {};
	snprintf(buff, sizeof(buff), "%d", idx);
	rf_snmp_evt(INFO, MSGMbpChange, buff, RMM_JSON_UPDATE_BIG);
	return NULL;
}

static json_t *mbp_post(struct rest_uri_param *param)
{
	json_t *req;
	int8 *action = NULL;
	int32 cm_lid;

	cm_lid = get_asset_idx(param, "mbp_id", MC_TYPE_CM);
	if (cm_lid == -1) {
		HTTPD_ERR("get cm loc lid fail\n");
		return NULL;
	}

	update_response_info(param, HTTP_APPLICATION_ERROR);

	req = json_parse_with_len(param->json_data, (int32)(param->content_length));
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	action = json_string_value(json_object_get(req, RMM_JSON_ACTION));
	if (action == NULL) {
		HTTPD_ERR("get action error\n");
		json_free(req);
		return NULL;
	}

	if (0 == strcmp(action, RMM_JSON_UPDATE)) {
		HTTPD_INFO("processing mbp reset\n");
		return mbp_process_update(req, cm_lid, param);
	} else if (0 == strcmp(action, RMM_JSON_RESET)) {
		HTTPD_INFO("processing mbp reset\n");
		return mbp_process_reset(req, cm_lid, param);
	} else if (0 == strcmp(action, RMM_JSON_SETUARTTARGET)) {
		HTTPD_INFO("processing mbp set uart target\n");
		return mbp_set_uart_target(req, cm_lid, param);
	}

	json_free(req);
	return NULL;
}

static json_t *mbp_evt_get(struct rest_uri_param *param)
{
	rf_evt_svc_t service = { { {0} } };
	json_t *result = NULL;
	int32 idx, reg = 0;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	idx = get_asset_idx(param, "mbp_id", MC_TYPE_CM);
	if (idx == -1) {
		HTTPD_ERR("get mbp index fail\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));
	reg = 1 << (idx - 1);
	get_evt_links(&(service.links), MC_REDFISH_MBP_COLL, reg, RF_EVENT_MBP_FMT, idx);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *mbp_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *mbp_evt_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	int32 mask = 0;
	listener_dest_t listener;
	int32 idx;

	idx = get_asset_idx(param, "mbp_id", MC_TYPE_CM);
	if (idx == -1) {
		HTTPD_ERR("get mbp index fail\n");
		return NULL;
	}

	update_response_info(param, HTTP_APPLICATION_ERROR);

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);

	mask = 1 << (idx - 1);

	return process_listener(req, &listener, mask, MC_REDFISH_MBP_COLL);
}

static json_t *mbp_coll_listener_get(struct rest_uri_param *param)
{
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_MBP_COLL, INVAILD_IDX, INVAILD_IDX, header);
	return	get_json_listener(header);
}

static json_t *mbp_listener_get(struct rest_uri_param *param)
{
	int32 mbp_idx = 0;
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	mbp_idx = get_asset_idx(param, "mbp_id", MC_TYPE_PSU);
	if (mbp_idx == -1) {
		HTTPD_ERR("get mbp index fail\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_MBP_COLL, INVAILD_IDX, mbp_idx, header);

	return get_json_listener(header);
}


static struct rest_handler mbp_coll_handler = {
	.get    = mbp_coll_get,
	.put    = mbp_coll_put,
	.post   = mbp_coll_post,
};

static struct rest_handler mbp_handler = {
	.get    = mbp_get,
	.put    = mbp_put,
	.post   = mbp_post,
};

static struct rest_handler mbp_coll_evt_handler = {
	.get    = mbp_coll_evt_get,
	.put    = mbp_coll_evt_put,
	.post   = mbp_coll_evt_post,
};

static struct rest_handler mbp_evt_handler = {
	.get    = mbp_evt_get,
	.put    = mbp_evt_put,
	.post   = mbp_evt_post,
};

static struct rest_handler mbp_coll_listener_handler = {
	.get    = mbp_coll_listener_get,
	.put    = NULL,
	.post   = NULL,
};

static struct rest_handler mbp_listener_handler = {
	.get    = mbp_listener_get,
	.put    = NULL,
	.post   = NULL,
};

