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
#include <pthread.h>
#include "librmmcfg/rmm_cfg.h"
#include "libredfish/msg_reg.h"
#include "libutils/time.h"
#include "libutils/base64.h"

typedef struct rack_update_info {
	uint32 upgrade_seq;
	uint8 deb_name[MAX_DEB_NAME_LEN];
	uint8 deb_path[MAX_DEB_PATH_LEN];
} rack_update_info_t;

static rack_update_info_t rack_update;
static struct rest_handler rack_handler;
static struct rest_handler rack_evt_handler;
static struct rest_handler log_handler;
static struct rest_handler rf_log_handler;
static struct rest_handler log_service_handler;
static struct rest_handler rack_listener_handler;
static struct rest_handler rf_mr_handler;
static struct rest_handler rack_snmp_listener_handler;
static struct rest_handler rack_platform_handler;


void register_rack_handler(void)
{
	register_handler("/rack", &rack_handler);
	register_handler("/rack/log", &log_handler);
	register_handler("/rack/rf_log", &rf_log_handler);
	register_handler("/rack/rf_mr", &rf_mr_handler);
	register_handler("/rack/rf_logservice", &log_service_handler);
	register_handler("/rack/rf_event", &rack_evt_handler);
	register_handler("/rack/rf_listener", &rack_listener_handler);
	register_handler("/rack/snmp_listener", &rack_snmp_listener_handler);
	register_handler("/rack/platform", &rack_platform_handler);
}


static json_t *json_object_result(struct json_result *result)
{
	json_t *obj;

	obj = json_object();
	if (obj == NULL) {
		HTTPD_ERR("request json object error\n");
		return NULL;
	}

	for (; result->name != NULL; result++)
		json_object_add(obj, result->name, result->json);

	return obj;
}

static int32 process_reset_rack(uint8 option)
{
	int32 fd = -1;
	int32 rc = -1;
	struct sockaddr_in addr;
	int32 port = rmm_cfg_get_port(UPGRADED_PORT);
	if (port == 0) {
		HTTPD_ERR("Failed to call rmm_cfg_get_upgrade_monitor_port\n");
		exit(-1);
	}
	
	int32 len = sizeof(struct sockaddr_in);
	struct _upgrade_monitor req = {0};
	int32 try_cnt = 0;

	if (SERVICE_RESET_PREPARE == option)
		req.sync = SERVICE_RESET_PREPARE;
	else if (FACTORY_DEFAULT_PREPARE == option)
		req.sync = FACTORY_DEFAULT_PREPARE;
	else {
		HTTPD_ERR("option not supported\n");
		return -1;
	}
	fd = udp_create();
	if (fd < 0) {
		HTTPD_ERR("upgrade thread create failed\n");
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	while (try_cnt < 3) {
		try_cnt++;

		rc = sendto(fd, (int8 *)&req, sizeof(req), 0,
					(struct sockaddr *)&addr, len);
		if (rc != sizeof(req))
			continue;

		break;
	}

	close(fd);

	if (try_cnt >= 3) {
		HTTPD_ERR("try count over 3\n");
		return -1;
	}
	return 0;
}

static result_t reset_rack(int32 reset_mode, struct rest_uri_param *param)
{
	pid_t pid;

	HTTPD_DEBUG("reset mode:%s\n", (0 == reset_mode) ? RMM_JSON_RESET_MODE_SERVICE:RMM_JSON_RESET_MODE_RMM);

	pid = fork();
	if (0 == pid) {
		close(param->fd);
		sleep(2);
		if (0 == reset_mode)
			process_reset_rack(SERVICE_RESET_PREPARE);
		else if (1 == reset_mode) {
			HTTPD_INFO("RMM Reset\n");
			if (system("reboot &") < 0)
				exit(0);
		}
		exit(0);
	} else {
		sleep(1);
		return RESULT_OK;
	}
}

static void *process_upgrade_thread(void *arg)
{
	int32 fd = -1;
	int32 rc = -1;
	struct sockaddr_in addr;
	struct sockaddr_in remote_addr;
	int32 remote_len = sizeof(struct sockaddr_in);
	int32 port = rmm_cfg_get_port(UPGRADED_PORT);
	if (port == 0) {
		HTTPD_ERR("Failed to call rmm_cfg_get_upgrade_monitor_port\n");
		exit(-1);
	}

	int32 len = sizeof(struct sockaddr_in);
	struct _upgrade_monitor req = {0};
	struct _upgrade_monitor resp = {0};
	fd_set fds;

	fd = udp_create();
	if (fd < 0) {
		HTTPD_ERR("upgrade thread create failed\n");
		return NULL;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	while (1) {
		/* Step 1: notify upgrade monitor to prepare do upgradation */
		req.sync = UPGRADE_PREPARE;
		req.seq = rack_update.upgrade_seq++;
		memcpy(req.deb_name, rack_update.deb_name, MAX_DEB_NAME_LEN);
		memcpy(req.deb_path, rack_update.deb_path, MAX_DEB_PATH_LEN);

		rc = sendto(fd, (int8 *)&req, sizeof(req), 0,
					(struct sockaddr *)&addr, len);
		if (rc != sizeof(req))
			continue;

		/* Step 2: wait for the response from the upgrade monitor,
		 * to make sure the message just be send has been received by upgrade
		 * monitor. */
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		rc = select(fd + 1, &fds, NULL, NULL, NULL);
		if (rc < 0)
			continue;

		rc = recvfrom(fd, (int8 *)&resp, sizeof(resp), 0,
					  (struct sockaddr *)&remote_addr,
					  (socklen_t *)&remote_len);
		if (rc != sizeof(resp))
			continue;

		if ((resp.sync == UPGRADE_PREPARE_ACK) && (resp.seq == rack_update.upgrade_seq))
			++rack_update.upgrade_seq;
		else
			continue;

		/* Step 3: send the confirmation to the upgrade monitor. */
		req.sync = UPGRADE_CONFIRM;
		req.seq = rack_update.upgrade_seq++;
		while (1) {
			rc = sendto(fd, (int8 *)&req, sizeof(req), 0,
						(struct sockaddr *)&addr, len);
			if (rc != sizeof(req))
				continue;

			close(fd);
			return NULL;
		}
	}

	close(fd);
	return NULL;
}

static result_t update_rack(int8 *comp_type, int8 *buff,
							 int64 length, int8 *version, uint32 version_len)
{
	int32 rc = -1;
	int32 status = -1;
	int8 *w_buf = NULL;
	int32 w_fd = -1;
	int32 len = 0;
	int8 cmd[128] = {0};
	int32 offset = 0;
	int32 decode_size = 0;
	int32 buff_len = 0;
	FILE *pp = NULL;
	int8 *sys_path = "export PATH=/sbin/:/bin/:/usr/sbin:/usr/bin";
	int8 output[256] = {0};
	int8 out_name[256] = {0};
	int8 out_ver[256] = {0};
	int32 ver_digital_cur[4] = {0};
	int32 ver_digital_new[4] = {0};
	int32 i = 0;
	struct stat sb;
	int8 *upgrade_path = "/tmp/tmp_rsa_rmm_upgrade_path";

	if ((length <= 0) || (comp_type == NULL) || (buff == NULL)) {
		HTTPD_ERR("input param error\n");
		return -1;
	}

	if (stat(upgrade_path, &sb) == -1) {
		if (errno == ENOENT)
			mkdir(upgrade_path, 0777);
		else {
			HTTPD_ERR("upgrade path error\n");
			return -1;
		}
	}

	w_fd = open(UPGRADE_FILE_TARBALL, O_RDWR | O_CREAT, S_IRWXU);
	if (w_fd < 0) {
		HTTPD_ERR("process update file error\n");
		return -1;
	}

	buff_len = length;

	while (offset < buff_len) {
		/* Decode the file first. */
		decode_size = ((offset + MAX_READ_BASE64_SIZE)
					   < buff_len ? MAX_READ_BASE64_SIZE
					   : (buff_len - offset));

		w_buf = base64_decode(buff + offset, decode_size, &len);
		if (NULL == w_buf)
			goto err;

		offset += decode_size;

		rc = write(w_fd, w_buf, len);
		if (rc < 0)
			goto err;

		free(w_buf);
		w_buf = NULL;
	}

	snprintf(cmd, sizeof(cmd), "rm -rf %s/*", upgrade_path);
	if (system(cmd) < 0)
		goto err;

	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "tar -zxf %s -C %s/", UPGRADE_FILE_TARBALL, upgrade_path);
	status = system(cmd);
	if (status < 0)
		goto err;

	if (WIFEXITED(status)) {
		if (WEXITSTATUS(status) != 0)
			goto err;
	} else
		goto err;

	snprintf(cmd, sizeof(cmd), "%s && dpkg-deb -W %s/%s.deb\n", sys_path, upgrade_path, comp_type);
	printf("cmd %s\n", cmd);
	pp = popen(cmd, "r");
	if (pp == NULL)
		goto err;
	while (fgets(output, 256, pp) != NULL) {
		rc = sscanf(output, "%s%s", out_name, out_ver);
		if (rc == 2) {
			if (strcmp(out_name, comp_type) == 0) {
				/* Compare the version */
				rc = sscanf(out_ver, "%d.%d.%d.%d", &ver_digital_new[0],
							&ver_digital_new[1], &ver_digital_new[2],
							&ver_digital_new[3]);
				if (rc < 3) {
					HTTPD_ERR("version mismatch\n");
					return RESULT_UNMATCHED_VERSION;
				}
				rc = sscanf(RSA_RMM_FIRMWARE_VERSION, "%d.%d.%d.%d",
							&ver_digital_cur[0],
							&ver_digital_cur[1],
							&ver_digital_cur[2],
							&ver_digital_cur[3]);
				if (rc < 3)
					goto err;
				HTTPD_DEBUG("%d.%d.%d.%d::%d.%d.%d.%d\n", ver_digital_cur[0],
						   ver_digital_cur[1], ver_digital_cur[2],
						   ver_digital_cur[3],
						   ver_digital_new[0], ver_digital_new[1],
						   ver_digital_new[2], ver_digital_new[3]);

				for (i = 0; i < 4; i++) {
					if ((ver_digital_cur[i] < ver_digital_new[i])
						|| (ver_digital_cur[i] > ver_digital_new[i]))
						break;

					if (i >= 3)
						return RESULT_SAME_VERSION;
				}

				snprintf(version, version_len, "rev %s", out_ver);
				break;
			}
		}
	}

	pclose(pp);

	memcpy(rack_update.deb_name, comp_type, strlen(comp_type));
	len = (MAX_DEB_PATH_LEN > strlen(upgrade_path) ? strlen(upgrade_path) : MAX_DEB_PATH_LEN);
	memcpy(rack_update.deb_path, upgrade_path, len);

	close(w_fd);
	unlink(UPGRADE_FILE_TARBALL);

	printf("libwrap_update_rack image[%s] len[%lld]\n", comp_type, length);
	return RESULT_OK;

err:
	unlink(UPGRADE_FILE_TARBALL);

	if (w_buf != NULL) {
		free(w_buf);
		w_buf = NULL;
	}

	close(w_fd);

	return -1;
}

static result_t set_factory_rack(void)
{
	int32 rc = -1;
#if 0
	
	int8 cmd[128] = {0};

	if (system("/bin/rm /var/memdb/memdb_upgrade.snapshot"))
		return rc;
	if (system("/bin/rm /var/memdb/memdb_rmm_update.log"))
		return rc;
	snprintf(cmd, sizeof(cmd), "touch %s", FACTORY_DEFAULT_FLAG);
	if (system(cmd))
		return rc;

	rc = process_reset_rack(FACTORY_DEFAULT_PREPARE);
#endif
	libwrap_rmm_factory_reset(&rc);

	return rc;
}

static int32 pack_rack_base_info(rack_info_t *rack_info, json_t *result)
{
	pack_basic_element_info(&(rack_info->be), result);
	/*
	add_json_integer(result, RMM_JSON_RACK_ID, rack_info->be.id);
	add_json_string(result, RMM_JSON_UUID, rack_info->be.uuid);
	*/
	add_json_string(result, RMM_JSON_API_VER, rack_info->api_ver);
	add_json_integer(result, RMM_JSON_RACK_PUID, rack_info->rack_puid);
	add_json_string(result, RMM_JSON_POD_DCUID, rack_info->pod_dcuid);
	add_json_string(result, RMM_JSON_PODM_ADDR, rack_info->podm_addr);
	add_json_string(result, RMM_JSON_GEO_TAG, rack_info->geo_tag);
	add_json_string(result, RMM_JSON_RACK_DCUID, rack_info->rack_dcuid);
	add_json_string(result, RMM_JSON_ASSET_TAG, rack_info->asset.asset_tag);

	return 0;
}

static int32 pack_rack_href_info(rack_info_t *rack_info, json_t *href, int8 *host)
{
	update_href_host(rack_info->href.drawer, HREF_URL_LEN, host);
	update_href_host(rack_info->href.powerzones, HREF_URL_LEN, host);
	update_href_host(rack_info->href.thermalzones, HREF_URL_LEN, host);
	update_href_host(rack_info->href.rf_event, HREF_URL_LEN, host);
	update_href_host(rack_info->href.mbps, HREF_URL_LEN, host);

	add_json_string(href, RMM_JSON_DRAWERS, rack_info->href.drawer);
	add_json_string(href, RMM_JSON_POWER_ZONES, rack_info->href.powerzones);
	add_json_string(href, RMM_JSON_THERMAL_ZONES, rack_info->href.thermalzones);//thermalzones--->thermalzones
	add_json_string(href, RMM_JSON_EVENTS, rack_info->href.rf_event);
	add_json_string(href, RMM_JSON_MBPS, rack_info->href.mbps);

	return 0;
}


static json_t *rack_get(struct rest_uri_param *param)
{
	rack_info_t rack_info = { {0} };
	result_t rs = RESULT_OK;
	json_t *result = NULL;
	json_t *fru = NULL;
	json_t *href = NULL;
	json_t *av_action = NULL;
	json_t *rs_json = NULL;
	json_t *action = NULL;
	json_t *cap = NULL;
	json_t *allow_values = NULL;

	rs = libwrap_get_rack(&rack_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("get rack info fail, result is %d\n", rs);
		return NULL;
	}

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("request json object fail\n");
		return NULL;
	}

	pack_rack_base_info(&rack_info, result);

	fru = json_object();
	if (fru != NULL) {
		pack_fru_info(&(rack_info.asset.fru), fru);
		json_object_add(result, RMM_JSON_FRU_INFO, fru);
	}

	add_json_string(result, RMM_JSON_SYSTEM_PRESENCE, rack_info.drawer_pres);
	add_json_string(result, RMM_JSON_MBP_PRESENCE, rack_info.mbp_pres);

	av_action = json_array();
	if (av_action == NULL) {
		HTTPD_ERR("available action json array request fail\n");
		return NULL;
	}

	/* action reset */
	action = json_object();
	if (0 != prepare_get_action(rack_info.av_action, action, 0)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	cap = json_array();
	if (cap != NULL) {
		json_t *property = NULL;

		property = json_object();
		if (property != NULL) {
			json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string(RMM_JSON_RESET_TYPE));
			allow_values = json_array();
			if (allow_values != NULL) {
				/* service */
				if (0 != strcmp((int8 *)(rack_info.av_action[0].cap[0].property), ""))
					json_array_add(allow_values, json_string((int8 *)(rack_info.av_action[0].cap[0].av)));

				/* rmm */
				if (0 != strcmp((int8 *)(rack_info.av_action[0].cap[1].property), ""))
					json_array_add(allow_values, json_string((int8 *)(rack_info.av_action[0].cap[1].av)));
				json_object_add(property, RMM_JSON_ALLOWABLE_VALUES, allow_values);
			}
			json_array_add(cap, property);
		}
		json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	}
	json_array_add(av_action, action);

	/* action update */
	action = json_object();
	if (0 != prepare_get_action(rack_info.av_action, action, 1)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	cap = json_array();
	if (cap != NULL) {
		json_t *property = NULL;

		/* ComponentType */
		property = json_object();
		if (property != NULL) {
			if (0 != strcmp((int8 *)(rack_info.av_action[1].cap[0].property), ""))
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(rack_info.av_action[1].cap[0].property)));
			json_array_add(cap, property);
		}

		/* ImageURL */
		property = NULL;
		property = json_object();
		if (property != NULL) {
			if (0 != strcmp((int8 *)(rack_info.av_action[1].cap[1].property), ""))
				json_object_add(property, RMM_JSON_PROPERTY_NAME, json_string((int8 *)(rack_info.av_action[1].cap[1].property)));
			json_array_add(cap, property);
		}
		json_object_add(action, RMM_JSON_CAPABILITIES, cap);
	}
	json_array_add(av_action, action);

	/* action FactoryDefault */
	action = json_object();
	if (0 != prepare_get_action(rack_info.av_action, action, 2)) {
		HTTPD_ERR("action prepare fail\n");
		return NULL;
	}

	json_array_add(av_action, action);
	json_object_add(result, RMM_JSON_AVALIBLE_ACTIONS, av_action);

	href = json_object();
	if (href != NULL) {
		pack_rack_href_info(&rack_info, href, param->host);
		json_object_add(result, RMM_JSON_HREF, href);
	}

	add_json_bool(result, RMM_JSON_RMM_PRESENT, (int32)rack_info.rmm_present);//yes or no
	add_json_bool(result, RMM_JSON_DISAGGR, (int32)rack_info.rack_disaggr);//False or True
	add_json_string(result, RMM_JSON_POD, (uint8 *)rack_info.pod);

	return result;
}

static void rack_put_prepare_int(json_t *req, int64 *output, int8 *name)
{
	json_t *rs_json = NULL;
	int8 *input = NULL;

	rs_json = json_object_get(req, name);
	if (rs_json != NULL) {
		int64 var = json_integer_value(rs_json);

		if (TRUE == check_int_range((int32)var, 0, 0x7fffffff))
			*output = var;
	}
}

static json_t *rack_put(struct rest_uri_param *param)
{
	json_t *req;
	result_t rs = RESULT_OK;
	put_rack_info_t put_rack_info = { {0} };
	int8 value[WRAP_DB_MAX_VALUE_LEN] = {0};
	int32 rc = 0;

	rs = libwrap_pre_put_rack(&put_rack_info);
	if (rs != RESULT_OK) {
		HTTPD_ERR("pre put fail, result is %d\n", rs);
		return rack_get(param);
	}

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("parse json data fail");
		return rack_get(param);
	}

	put_prepare_str(req, put_rack_info.descr, DESCRIPTION_LEN, RMM_JSON_DESC);
	rack_put_prepare_int(req, &(put_rack_info.rack_puid), RMM_JSON_RACK_PUID);
	put_prepare_str(req, put_rack_info.pod_dcuid, DCUID_LEN, RMM_JSON_POD_DCUID);
	put_prepare_str(req, put_rack_info.geo_tag, RACK_TAG_LEN, RMM_JSON_GEO_TAG);
	put_prepare_str(req, put_rack_info.podm_addr, REST_RACK_STRING_LEN, RMM_JSON_PODM_ADDR);
	put_prepare_str(req, put_rack_info.asset_tag, REST_RACK_STRING_LEN, RMM_JSON_ASSET_TAG);

	rs = libwrap_put_rack(put_rack_info);
	if (rs != RESULT_OK)
		HTTPD_ERR("rack put fail, result is %d\n", rs);

	libutils_get_time(NULL, value);
	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC,
						  RACK_UPDATE_DATE_STR, 0x0,
						  value, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		rmm_log(ERROR, "memdb set rack update date fail\n");
		return rack_get(param);
	}
	json_free(req);

	rf_log(INFO, MSGRackUpdate, RMM_JSON_UPDATED_BIG);
	return rack_get(param);
}

static json_t *rack_process_reset(int8 *cap, json_t *result, json_t *req, struct rest_uri_param *param)
{
	int32 reset_mode = 0;

	if (0 == strcmp(cap, RMM_JSON_RESET_MODE_SERVICE))
		reset_mode = 0;
	else if (0 == strcmp(cap, RMM_JSON_RESET_MODE_RMM))
		reset_mode = 1;

	json_free(req);
	json_free(result);
	if (RESULT_OK == reset_rack(reset_mode, param)) {
		HTTPD_INFO("rack reset success\n");
		update_response_info(param, HTTP_ACCEPTED);
		rf_snmp_evt(INFO, MSGRackChange, RMM_JSON_RESETED_BIG);
		return NULL;
	} else {
		HTTPD_ERR("rack reset fail\n");
		update_response_info(param, HTTP_NOT_ACCEPTABLE);
		return NULL;
	}
}

static json_t *rack_handle_reset(json_t *req, json_t *result, struct rest_uri_param *param)
{
	json_t *rs_json = NULL;
	int8 *cap = NULL;

	rs_json = json_object_get(req, RMM_JSON_RESET_TYPE);
	if (rs_json == NULL) {
		HTTPD_ERR("json object request error.\n");
		json_free(req);
		json_free(result);
		return NULL;
	}
	cap = json_string_value(rs_json);
	if (cap == NULL) {
		HTTPD_ERR("failed to json string.\n");
		return NULL;
	}
	return rack_process_reset(cap, result, req, param);
}

static json_t *rack_update_err(json_t *req, json_t *result, int32 rc, struct rest_uri_param *param)
{
	int8 buff[64] = {0};

	if (rc == RESULT_UNMATCHED_VERSION)
		snprintf(buff, sizeof(buff), "%s", "unmatched version");
	else if (rc == RESULT_SAME_VERSION)
		snprintf(buff, sizeof(buff), "%s", "same version");
	else {
		HTTPD_ERR("unknown error\n");
		json_free(req);
		json_free(result);
		return NULL;
	}

	update_response_info(param, HTTP_NOT_ACCEPTABLE);
	json_object_add(result, RMM_JSON_RESULT, json_string(buff));
	json_free(req);
	return result;
}

static json_t *rack_handle_update(pthread_t *tid, json_t *req, json_t *result, struct rest_uri_param *param)
{
	json_t *rs_json = NULL;
	int8 *cap1 = NULL;
	int8 *cap2 = NULL;
	int32 rc = -1;
	int64 len = 0;
	int8 image_ver[256] = {0};

	struct json_result response[] = {
		{RMM_JSON_RESULT, NULL},
		{"new image rev", NULL},
		{NULL, NULL}
	};

	rs_json = json_object_get(req, RMM_JSON_COMPONENT_TYPE);
	if (rs_json == NULL) {
		HTTPD_ERR("no ComponentType field!\n");
		goto err;
	} else
		cap1 = json_string_value(rs_json);

	if (cap1 == NULL) {
		HTTPD_ERR("failed to get json string <componentType>!\n");
		goto err;
	}
	rs_json = json_object_get(req, RMM_JSON_IMAGE);
	if (rs_json == NULL) {
		HTTPD_ERR("get image faild!\n");
		goto err;
	}

	cap2 = json_string_value(rs_json);
	if (cap2 == NULL) {
		HTTPD_ERR("failed to get json string <image>!\n");
		goto err;
	}

	len = strlen(cap2);
	/* start to do the update, need to have a solution to receive the image */
	rc = update_rack(cap1, cap2, len, image_ver, 256);
	if (rc == RESULT_OK) {
		json_free(req);
		json_free(result);

		/* Create a new thread to notify upgrade_monitor to do
		* the upgradation. */
		rc = pthread_create(tid, NULL, process_upgrade_thread, NULL);
		if (rc < 0) {
			HTTPD_ERR("upgrade thread create failed\n");
			return NULL;
		}

		update_response_info(param, HTTP_ACCEPTED);

		response[0].json = json_string(RMM_JSON_UPDATING);
		response[1].json = json_string(image_ver);

		rf_snmp_evt(INFO, MSGRackUpdate, "Update firmware");
		return json_object_result(response);
	} else
		return rack_update_err(req, result, rc, param);
err:
	json_free(req);
	json_free(result);
	return NULL;

}

static json_t *rack_handle_default(json_t *req, json_t *result, struct rest_uri_param *param)
{
	json_free(req);
	json_free(result);

	if (set_factory_rack() == RESULT_OK) {
		HTTPD_INFO("factory default success\n");
		update_response_info(param, HTTP_ACCEPTED);
		rf_snmp_evt(INFO, MSGRackUpdate, "factory default");
		return NULL;
	} else {
		HTTPD_ERR("processing factory default error\n");
		return NULL;
	}
}

static json_t *rack_post(struct rest_uri_param *param)
{
	json_t *req = NULL;
	int8 *action = NULL;
	json_t *result = NULL;
	json_t *rs_json = NULL;
	pthread_t tid;

	update_response_info(param, HTTP_APPLICATION_ERROR);

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("json_object fail\n");
		return NULL;
	}

	if ((param->json_data == NULL) || (0 == strlen(param->json_data))) {
		HTTPD_ERR("json_data is NULL or empty\n");
		goto err;
	}

	req = json_parse_with_len(param->json_data, (int32)(param->content_length));
	if (req == NULL) {
		HTTPD_ERR("parse json error\n");
		goto err;
	}

	if (NULL == (rs_json = json_object_get(req, RMM_JSON_ACTION))) {
		HTTPD_ERR("fail to get action field.\n");
		goto err;
	}

	action = json_string_value(rs_json);
	if (action == NULL) {
		HTTPD_ERR("fail to get json string <action> field.\n");
		goto err;
	}

	if (0 == strcmp(action, RMM_JSON_RESET)) {
		rack_handle_reset(req, result, param);
		return NULL;
	} else if (0 == strcmp(action, RMM_JSON_UPDATE))
		return rack_handle_update(&tid, req, result, param);
	else if (0 == strcmp(action, RMM_JSON_LOAD_FACTORY_DEFAULT))
		return rack_handle_default(req, result, param);
err:
	if (req)
		json_free(req);
	if (result)
		json_free(result);
	return NULL;
}

static json_t *rack_evt_get(struct rest_uri_param *param)
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
	get_evt_links(&(service.links), MC_REDFISH_RACK, reg, RF_EVENT_RACK_FMT);
	pack_rf_evt_links(result, &(service.links));

	return result;
}

static json_t *rack_evt_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *rack_evt_post(struct rest_uri_param *param)
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

	return process_listener(req, &listener, mask, MC_REDFISH_RACK);
}

static json_t *log_get(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *log_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *rf_log_get(struct rest_uri_param *param)
{
	json_t *result = NULL;
	json_t *json_log = NULL;
	int8 *buff = NULL;
	int8 *p_count;
	rf_msg_info_t *info = NULL;
	void *offset = NULL;
	int32 count, index = 0;

	p_count = rest_query_value(param, "count");

	if(p_count == NULL)
		goto expt_end;
	
	info = (rf_msg_info_t *)malloc(sizeof(rf_msg_info_t));

	if(!info)
		goto expt_end;

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		goto expt_end;
	}

	count = strtoul(p_count, 0, 10);

	rf_log_get_by_limit(count, &info->dsc, info->data);

	json_object_add(result, RMM_JSON_COUNT, json_integer(info->dsc.count));
	json_object_add(result, RMM_JSON_LEN, json_integer(info->dsc.length));

	json_log = json_array();
	if (NULL != json_log) {
		if(info->dsc.count < 1024) {
			offset = (void *)info->data;
			while(index < info->dsc.count) {
				json_array_add(json_log, json_string((int8*)(offset + index * 256)));
				index++;
			}
			json_object_add(result, RMM_JSON_LOG_INFO, json_log);
			if(info) {
				free(info);
				info = NULL;
			}
		} else {
			json_array_add(json_log, json_string("log request size is too long"));
			json_object_add(result, RMM_JSON_LOG_INFO, json_log);
		}

		return result;
	}

expt_end:
	if(info) {
		free(info);
		info = NULL;
	}
	if(result)
		json_free(result);
	if(json_log)
		json_free(json_log);
	HTTPD_ERR("file %s line %d: %s fail\n", __FILE__, __LINE__, __func__);

	return NULL;
}

static json_t *rf_log_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *rf_log_post(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *log_service_get(struct rest_uri_param *param)
{
	rf_log_svc_t service = { { {0} } };
	json_t *result = NULL;

	get_rf_base_info(&(service.base));

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	pack_rf_base_json(result, &(service.base));

	return result;
}

static json_t *rack_listener_get(struct rest_uri_param *param)
{
	listener_dest_info_t *header = (listener_dest_info_t *)malloc(sizeof(listener_dest_info_t));

	if (header == NULL) {
		HTTPD_ERR("malloc failed\n");
		return NULL;
	}

	memset(header, 0, sizeof(listener_dest_info_t));
	libwrap_get_listeners_by_type(MC_REDFISH_RACK, INVAILD_IDX, INVAILD_IDX, header);

	return get_json_listener(header);
}


static json_t *rack_snmp_listener_get(struct rest_uri_param *param)
{
	return	NULL;
}

static json_t *rack_snmp_listener_put(struct rest_uri_param *param)
{
	return NULL;
}

static json_t *rack_snmp_listener_post(struct rest_uri_param *param)
{
	json_t *req = NULL;

	req = json_parse(param->json_data);
	if (req == NULL) {
		HTTPD_ERR("json parse error\n");
		return NULL;
	}

	update_response_info(param, HTTP_ACCEPTED);

	return process_snmp_listener(req);
}

static json_t *rack_platform_get(struct rest_uri_param *param)
{
	json_t *result = NULL;
	int32 reg = 0;
	int8 *platform;

	if (is_platform_bdcr())
		platform = PLATFORM_NAME_BDC_R;
	else
		platform = PLATFORM_NAME_BDC_A;

	result = json_object();
	if (result == NULL) {
		HTTPD_ERR("result json object request error\n");
		return NULL;
	}

	add_json_string(result, RMM_JSON_PLATFORM, (uint8*)platform);

	return result;
}


static json_t *rf_mr_get(struct rest_uri_param *param)
{
	json_t *result = NULL;
	result = rf_get_standard_msg_reg_json();
	return result;
}

static struct rest_handler rack_handler = {
	.get	= rack_get,
	.put	= rack_put,
	.post	= rack_post,
};

static struct rest_handler rack_evt_handler = {
	.get	= rack_evt_get,
	.put	= rack_evt_put,
	.post	= rack_evt_post,
};

static struct rest_handler log_handler = {
	.get	= log_get,
	.put	= log_post,
	.post	= log_post,
};


static struct rest_handler rf_log_handler = {
	.get	= rf_log_get,
	.put	= rf_log_put,
	.post	= rf_log_post,
};


static struct rest_handler log_service_handler = {
	.get	= log_service_get,
	.put	= NULL,
	.post	= NULL,
};

static struct rest_handler rack_listener_handler = {
	.get	= rack_listener_get,
	.put	= NULL,
	.post	= NULL,
};

static struct rest_handler rack_platform_handler = {
	.get	= rack_platform_get,
	.put	= NULL,
	.post	= NULL,
};


static struct rest_handler rack_snmp_listener_handler = {
	.get	= rack_snmp_listener_get,
	.put	= rack_snmp_listener_put,
	.post	= rack_snmp_listener_post,
};

static struct rest_handler rf_mr_handler = {
	.get	= rf_mr_get,
	.put	= NULL,
	.post	= NULL,
};



