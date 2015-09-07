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


#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <termios.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>

#include "libmemdb/memdb.h"
#include "libredfish/msg_reg.h"
#include "libredfish/rf_event.h"

#include "libwrap/wrap.h"
#include "librmmcfg/cfg_def.h"
#include "librmmcfg/rmm_cfg.h"
#include "libjsonrpcapi/assetd_api.h"

#include "libjsonrpcapi/assetd_socket.h"
#include "libjsonrpcapi/asset_module_socket.h"
#include "libjsonrpcapi/assetd_api.h"
#include "libjsonrpc/jsonrpc.h"
#include "libassetd/assetd_type.h"
#include "libassetd/assetd_jrpc_def.h"
#include "libutils/ip.h"
#include "libutils/rmm_json.h"
#include "libutils/string.h"
#include "libutils/types.h"
#include "libassetmodule/am_action.h"

#include "am_api.h"

wrap_msg_t wrap_ipmi_msg;
static pthread_mutex_t wrap_mutex[MAX_REQ_PROCESS];
static pthread_cond_t  wrap_cond[MAX_REQ_PROCESS];


#if 0
static void* wrap_ipmi_thread(void *argv);
#endif
static result_t check_module_capability(char *module, char *cap_name);

#define DEFAULT_MASK 		0xFFFF
static int get_db_info_num(unsigned char db_name, memdb_integer node_id, char* name)
{
	char result[128] = {0};
	char* offset;
	int64 error_code = 0;

	error_code = libdb_attr_get_string(db_name, node_id, name, result, 128, LOCK_ID_NULL);
	if(error_code != 0) {
		printf("getting %s from memdb error, code is %llu\n", name, error_code);
	}

	if(strstr((char*)result, HARD_CODE_FLAG)) {
		offset = result + (sizeof(HARD_CODE_FLAG)-1);
		return atoi(offset);
	}
	
	return atoi(result);
}

int memdb_filter(memdb_integer node_id)
{
	char result[128] = {0};
	int64 rc = 0;

	rc = libdb_attr_get_string(DB_RMM, node_id, WRAP_UUID_STR, result, 128, LOCK_ID_NULL);
	if(rc != 0)
		return 1;

	if(strlen(result) == 0)
		return 1;
	else
		return 0;
}


/*
static int get_process_idx(int pid)
{
	int i = 0;

	for(i = 0; i < MAX_REQ_PROCESS; i++) {
		if(id_tbl[i].pid == pid) {
			return id_tbl[i].idx;
		}
	}

	for(i = 0; i < MAX_REQ_PROCESS; i++) {
		if((id_tbl[i].pid == 0)&&(id_tbl[i].idx == 0)) {
			id_tbl[i].pid = pid;
			id_tbl[i].idx = i;
		}
	}
	return id_tbl[i].idx;
}
*/

int libwrap_get_asset_num(int zone_type)
{
	int subnode_num = 0;
	struct node_info *node = NULL;

	node = libdb_list_node_by_type(DB_RMM, zone_type, zone_type, &subnode_num, memdb_filter, LOCK_ID_NULL);
	libdb_free_node(node);

	return subnode_num;
}

static int get_zone_num(int zone_type)
{
	int zone_num = 0;

	switch (zone_type) {
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
		return 0;
	}

	return zone_num;
}

int get_cm_lid(int zone_type, int zone_idx)
{
	int lid = 0;
	int zone_num = get_zone_num(zone_type);

	if (zone_num == 1)
		lid = (zone_idx / zone_num);
	else
		lid = (zone_idx / zone_num) + 1;

	return lid;
}

int get_zone_lid(int zone_type, int zone_idx)
{
	int lid = 0;
	int zone_num = get_zone_num(zone_type);

	if (zone_num == 1)
		lid = 1;
	else
		lid = zone_idx % zone_num;

	return lid;
}

result_t get_psu_node_id_by_idx(int pzone_idx, int psu_idx, memdb_integer *node_id)
{
	memdb_integer cm_node_id;
	memdb_integer pzone_node_id;
	memdb_integer psu_node_id;
	int cm_lid = 0;
	int pzone_lid = 0;
	int psu_lid = psu_idx;

	cm_lid = get_cm_lid(MC_TYPE_PZONE, pzone_idx);
	pzone_lid = get_zone_lid(MC_TYPE_PZONE, pzone_idx);
	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	if(cm_node_id == 0) {
		return RESULT_NO_NODE;
	}

	pzone_node_id = get_subnode_id_by_lid(pzone_lid, cm_node_id, MC_TYPE_PZONE);
	if(pzone_node_id == 0) {
		return RESULT_NO_NODE;
	}

	psu_node_id = get_subnode_id_by_lid(psu_lid, pzone_node_id, MC_TYPE_PSU);
	if(psu_node_id == 0) {
		return RESULT_NO_NODE;
	}

	*node_id = psu_node_id;
	return RESULT_OK;
}

result_t get_fan_node_id_by_idx(int tzone_idx, int fan_idx, memdb_integer *node_id)
{
	memdb_integer cm_node_id;
	memdb_integer tzone_node_id;
	memdb_integer fan_node_id;
	int cm_lid = 0;
	int tzone_lid = 0;
	int fan_lid = fan_idx;

	cm_lid = get_cm_lid(MC_TYPE_TZONE, tzone_idx);
	tzone_lid = get_zone_lid(MC_TYPE_TZONE, tzone_idx);
	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	if(cm_node_id == 0) {
		return RESULT_NO_NODE;
	}

	tzone_node_id = get_subnode_id_by_lid(tzone_lid, cm_node_id, MC_TYPE_TZONE);
	if(tzone_node_id == 0) {
		return RESULT_NO_NODE;
	}

	fan_node_id = get_subnode_id_by_lid(fan_lid, tzone_node_id, MC_TYPE_FAN);
	if(fan_node_id == 0) {
		return RESULT_NO_NODE;
	}

	*node_id = fan_node_id;
	return RESULT_OK;
}


result_t get_drawer_id_by_idx(int dzone_idx, int drawer_idx, memdb_integer *node_id)
{
	memdb_integer cm_node_id;
	memdb_integer dzone_node_id;
	memdb_integer drawer_node_id;
	int cm_lid = 0;
	int dzone_lid = 0;
	int drawer_lid = drawer_idx;

	cm_lid = get_cm_lid(MC_TYPE_TZONE, dzone_idx);
	dzone_lid = get_zone_lid(MC_TYPE_TZONE, dzone_idx);
	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	if(cm_node_id == 0) {
		return RESULT_NO_NODE;
	}

	dzone_node_id = get_subnode_id_by_lid(dzone_lid, cm_node_id, MC_TYPE_DZONE);
	if(dzone_node_id == 0) {
		return RESULT_NO_NODE;
	}

	drawer_node_id = get_subnode_id_by_lid(drawer_lid, dzone_node_id, MC_TYPE_DRAWER);
	if(drawer_node_id == 0) {
		return RESULT_NO_NODE;
	}

	*node_id = drawer_node_id;
	return RESULT_OK;
}


#if 0
void wrap_init(void)
{
	pthread_t pth_id;
	bzero(&wrap_ipmi_msg, sizeof(struct wrap_msg));
	INIT_LIST_HEAD(&wrap_ipmi_msg.wrap_head);

	if (pthread_create(&pth_id, NULL, wrap_ipmi_thread, NULL) != 0) {
		printf("Failed to create wrap ipmi thread!\n");
		exit(0);
	}
	return;
}
#endif

static int get_present(memdb_integer zone_node_id, unsigned int *present)
{
	int i;
	int64 error_code = 0;
	struct node_info *sub_node_infos = 0;

	int  sub_nodenum = 0;
	int index = 0;

	sub_node_infos = libdb_list_subnode(DB_RMM, zone_node_id, &sub_nodenum, memdb_filter, LOCK_ID_NULL);
	if(sub_node_infos == NULL) {
		return -1;
	}

	for (i = 0; i < sub_nodenum; i++) {
		index = 0;
		libdb_attr_get_int(DB_RMM, sub_node_infos[i].node_id, WRAP_LOC_ID_STR, &index, LOCK_ID_NULL);
		if (index > 0)
			*present = *present | 1 << (index -1);
	}
	return 0;
}

static int get_present_string(memdb_integer node_id, char* output, int max_len)
{
	unsigned int present; 
	int i = 0;
	char tmp[8] = {0};
	char buff[REST_PRESENCE_LEN] = {0};
	char c;

	if (-1 ==  get_present(node_id, &present))
		return -1;
	printf("%s node_id is %lld, present is %d\n", __func__, node_id, present);

	for(i = 0; i < 8; i++) {
		memset(tmp, 0, sizeof(tmp));
		if(((present>>i)&1) == 1) {
			snprintf(tmp, sizeof(tmp), "%d", 1);
		} else {
			snprintf(tmp, sizeof(tmp), "%d", 0);
		}
		strncat(buff, tmp, strlen(tmp));
	}

	//Revert string.
	for(i = 0; i < 4; i++) {
		c = buff[i];
		buff[i] = buff[7-i];
		buff[7-i] = c;
	}

	strncpy_safe(output, buff, sizeof(buff), sizeof(buff) - 1);

	return 0;
}

static int get_db_info_string(unsigned char db_name, memdb_integer node_id, char* name, uint8* output, unsigned int len)
{
	char result[128] = {0};
	char* offset;
	int64 error_code = 0;

	error_code = libdb_attr_get_string(db_name, node_id, name, result, 128, LOCK_ID_NULL);

	if(error_code != 0) {
		return (int)error_code;
	}

	if(strstr((char*)result, HARD_CODE_FLAG)) {
		offset = result + (sizeof(HARD_CODE_FLAG)-1);
	}
	else {
		offset = result;
	}
	memcpy((char*)output, offset, (strlen(offset)>len)?len:strlen(offset));

	return 0;
}


static int get_base_element(base_element_t* be, unsigned char db_name, memdb_integer node_id)
{
	be->id = get_db_info_num(db_name, node_id, RACK_LOC_ID_STR);

	get_db_info_string(db_name, node_id, RACK_UUID_STR, be->uuid, UUID_LEN);
	get_db_info_string(db_name, node_id, RACK_NAME_STR, be->name, RMM_NAME_LEN);
	get_db_info_string(db_name, node_id, RACK_DESCRIPT_STR, be->desc, DESCRIPTION_LEN);
	get_db_info_string(db_name, node_id, RACK_CREATE_DATE_STR, be->create_date, DATE_LEN);
	get_db_info_string(db_name, node_id, RACK_UPDATE_DATE_STR, be->update_date, DATE_LEN);

	return 0;
}


static int get_asset_info(unsigned char db_name, asset_info_t* asset, memdb_integer node_id)
{
	get_db_info_string(db_name, node_id, RACK_SER_NUM_STR, asset->fru.serial_num, REST_RACK_STRING_LEN);
	get_db_info_string(db_name, node_id, RACK_MANUFACT_STR, asset->fru.manufacture, REST_RACK_STRING_LEN);
	get_db_info_string(db_name, node_id, RACK_MODEL_STR, asset->fru.model, REST_RACK_STRING_LEN);
	get_db_info_string(db_name, node_id, RACK_PART_NUM_STR, asset->fru.part_num, REST_RACK_STRING_LEN);
	get_db_info_string(db_name, node_id, RACK_FW_VER_STR, asset->fru.fw_ver, REST_RACK_STRING_LEN);
	get_db_info_string(db_name, node_id, RACK_ASSET_TAG_STR, asset->asset_tag, REST_ASSET_TAG_LEN);

	return  0;
}

static int get_rack_availible_action(unsigned char db_name, avail_action_t* _action, memdb_integer node_id)
{
	char buff[RMM_NAME_LEN] = {0};
	avail_action_t *action = _action;

	strncpy_safe((char*)action->action, RMM_JSON_RESET, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[0].property, RMM_JSON_RESET_TYPE, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(db_name,node_id, RACK_AV_RST_1_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[0].av, buff, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	strncpy_safe((char*)action->cap[1].property, RMM_JSON_RESET_TYPE, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(db_name,node_id, RACK_AV_RST_2_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[1].av, buff, REST_RACK_STRING_LEN, RMM_NAME_LEN - 1);

	action++;
	strncpy_safe((char*)action->action, RMM_JSON_UPDATE, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[0].property, RMM_JSON_COMPONENT_TYPE, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[1].property, RMM_JSON_IMAGE, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	action++;
	strncpy_safe((char*)action->action, RMM_JSON_LOAD_FACTORY_DEFAULT, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	return 0;
}

static int get_rack_href(unsigned char db_name, href_t* href, memdb_integer node_id)
{
	get_db_info_string(db_name,node_id, RACK_HREF_DRAWER_STR, (uint8*)(href->drawer), REST_RACK_STRING_LEN);
	get_db_info_string(db_name,node_id, RACK_HREF_PZONE_STR, (uint8*)(href->powerzones), REST_RACK_STRING_LEN);
	get_db_info_string(db_name,node_id, RACK_HREF_TZONE_STR, (uint8*)(href->thermalzones), REST_RACK_STRING_LEN);
	get_db_info_string(db_name,node_id, RACK_HREF_EVENT_STR, (uint8*)(href->rf_event), REST_RACK_STRING_LEN);
	get_db_info_string(db_name,node_id, RACK_HREF_MBP_STR, (uint8*)(href->mbps), REST_RACK_STRING_LEN);
	return 0;
}

#if 0
/**
 * @Ex: "2013-01-10T01:09:57+08:00"
 */
result_t libwrap_get_time(char* name, char* value)
{
	return get_iso8601_time(value);
}
#endif


static void get_mbp_pres(unsigned char *pres)
{
	memdb_integer cm_node_id;
	int i = 0;

	for(i = 1; i <= MAX_MBP_PRESENCE; i++) {
		cm_node_id = get_subnode_id_by_lid(i, MC_TYPE_RMC, MC_TYPE_CM);
		if(cm_node_id != 0) {
			pres[MAX_MBP_PRESENCE-i] = '1';
		}
		else {
			pres[MAX_MBP_PRESENCE-i] = '0';
		}
	}
	pres[MAX_MBP_PRESENCE] = '\0';
}

static int get_drawer_pres_from_cm(char *pres)
{
    struct node_info *subnode = NULL;
	int subnode_num = 0;
	int i,j;
	int cm_lid = 0;
	int drawer_pres = 0;;
	int tmp = 0;
	int dr_number = 0;
	struct node_info *dr_subnode = NULL;
	int dr_lid = 0;
	memdb_integer dz_node_id = 0;
	memdb_integer cm_node_id = 0;

	subnode = libdb_list_node_by_type(MC_TYPE_RMC, MC_TYPE_DZONE, MC_TYPE_DZONE, &subnode_num, memdb_filter, LOCK_ID_NULL);

	if (subnode_num == 0) {
		libdb_free_node(subnode);
		return RESULT_NO_NODE;
	}

	for (i = 0; i < subnode_num; i++) {
		dz_node_id = subnode[i].node_id;
		cm_node_id = subnode[i].parent;
		dr_subnode = libdb_list_subnode_by_type(DB_RMM, dz_node_id, MC_TYPE_DRAWER, &dr_number, &memdb_filter, LOCK_ID_NULL);
		libdb_attr_get_int(DB_RMM, cm_node_id, MBP_LOC_ID_STR, &cm_lid, LOCK_ID_NULL);
		for (j = 0; j < dr_number; j++) {
			dr_lid = get_db_info_num(DB_RMM, dr_subnode[j].node_id, WRAP_LOC_ID_STR);
			drawer_pres = (1 << (dr_lid - 1 + ((cm_lid - 1) * 4)))|drawer_pres;
			printf("cm[%d] dr_number[%d] dr_idx[%d] drawer_pres[%d]\n", cm_lid, dr_number, dr_lid, drawer_pres);
		}
	}

	for (i = 0; i < 8; i++) {
		strncat(pres, ((0x80 == (0x80 & (drawer_pres << i)))? "1" : "0"), 1);		
	}

	libdb_free_node(subnode);

	return subnode_num;
}

result_t libwrap_get_rack(rack_info_t *rack_info)
{
	get_base_element(&rack_info->be, DB_RMM, MC_TYPE_RMC);

	rack_info->rack_puid = get_db_info_num(DB_RMM, MC_TYPE_RMC,
										   RACK_PUID_STR);

	get_db_info_string(DB_RMM, MC_TYPE_RMC, RACK_GEOTAG_STR,
					   rack_info->geo_tag, RACK_TAG_LEN);
	get_db_info_string(DB_RMM, MC_TYPE_RMC, RACK_DCUID_STR,
					   rack_info->rack_dcuid, DCUID_LEN);
	get_db_info_string(DB_RMM, MC_TYPE_RMC, POD_DCUID_STR,
					   rack_info->pod_dcuid, DCUID_LEN);
	get_db_info_string(DB_RMM, MC_TYPE_RMC, RACK_API_VER_STR,
					   rack_info->api_ver, VERSION_LEN);
	get_db_info_string(DB_RMM, MC_TYPE_RMC, RACK_PODM_ADDR_STR,
					   rack_info->podm_addr, REST_RACK_STRING_LEN);

	get_asset_info(DB_RMM, &rack_info->asset, MC_TYPE_RMC);

	memset((char *)rack_info->drawer_pres, 0, MAX_DRAWER_PRESENCE + 1);
	get_drawer_pres_from_cm((char *)rack_info->drawer_pres);
	memset((char *)rack_info->mbp_pres, 0, MAX_MBP_PRESENCE + 1);
	get_mbp_pres(rack_info->mbp_pres);

	get_rack_availible_action(DB_RMM, rack_info->av_action, MC_TYPE_RMC);
	get_rack_href(DB_RMM, &(rack_info->href), MC_TYPE_RMC);

    #ifdef ODATA_ADD_ATTR
	//get_db_info_string(DB_RMM, MC_TYPE_RMC, RACK_RMM_PRES, (uint8*)(rack_info->rmm_present), REST_RACK_STRING_LEN);
    rack_info->rmm_present = 1; ///always true
	//get_db_info_string(DB_RMM, MC_TYPE_RMC, RACK_DISAGGR, (uint8*)(rack_info->rack_disaggr), REST_RACK_STRING_LEN);
    rack_info->rack_disaggr = 0; ///false
	//get_db_info_string(DB_RMM, MC_TYPE_RMC, RACK_POD, (uint8*)(rack_info->pod), REST_RACK_STRING_LEN);
	memset(rack_info->pod, 0, REST_RACK_STRING_LEN);
    memcpy(rack_info->pod, "N/A", strlen("N/A"));
	#endif
	return RESULT_OK;
}

result_t libwrap_pre_put_rack(put_rack_info_t *put_rack_info)
{
	char buff[32] = {0};
	char output[32] = {0};

	libdb_attr_get_string(DB_RMM, MC_TYPE_RMC, RACK_DESCRIPT_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe((char*)put_rack_info->descr, output, DESCRIPTION_LEN, DESCRIPTION_LEN - 1);
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, MC_TYPE_RMC, RACK_GEOTAG_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe((char*)put_rack_info->geo_tag, output, RACK_TAG_LEN, RACK_TAG_LEN - 1);
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, MC_TYPE_RMC, RACK_PODM_ADDR_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe((char*)put_rack_info->podm_addr, output, REST_RACK_STRING_LEN, REST_RACK_STRING_LEN - 1);
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, MC_TYPE_RMC, POD_DCUID_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe((char*)put_rack_info->pod_dcuid, output, DCUID_LEN, DCUID_LEN - 1);
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, MC_TYPE_RMC, RACK_ASSET_TAG_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe((char*)put_rack_info->asset_tag, output, REST_RACK_STRING_LEN, REST_RACK_STRING_LEN - 1);
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, MC_TYPE_RMC, RACK_PUID_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe(buff, output, sizeof(buff), sizeof(buff) - 1);
	put_rack_info->rack_puid = (int32)atoi(buff);

	return RESULT_OK;
}

result_t libwrap_put_rack(const put_rack_info_t put_rack_info)
{
	char buff[32] = {0};
	int rc = 0;

	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC, RACK_DESCRIPT_STR,
						  0x0, (char*)put_rack_info.descr, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC, RACK_GEOTAG_STR,
						  0x0, (char*)put_rack_info.geo_tag, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC, RACK_PODM_ADDR_STR,
						  0x0, (char*)put_rack_info.podm_addr, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC, POD_DCUID_STR,
						  0x0, (char*)put_rack_info.pod_dcuid, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}
	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC, RACK_ASSET_TAG_STR,
						  0x0, (char*)put_rack_info.asset_tag, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC, RACK_ASSET_TAG_STR,
						  0x0, (char*)put_rack_info.asset_tag, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	snprintf(buff, sizeof(buff), "%d", (int32)put_rack_info.rack_puid);
	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC, RACK_PUID_STR,
						  0x0, buff, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, MC_TYPE_RMC, RACK_LOC_ID_STR,
						  0x0, buff, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	return RESULT_OK;
}


static void get_fan_present_by_node_id(memdb_integer tzone_node_id,int fan_idx, struct fan_member *fan_member)
{
	unsigned int present = 0;
	int64 error_code = 0;
	int rc = -1;

	rc = get_present(tzone_node_id, &present);
	
	if((error_code != 0)||(rc == -1)) {
		snprintf((char *)(fan_member->presence), REST_PRESENCE_LEN, "NOT PRESENT");
		return;
	}

	if((present>>(fan_idx - 1))&1) {
		snprintf((char *)fan_member->presence, REST_PRESENCE_LEN, "PRESENT");
	} else {
		snprintf((char *)fan_member->presence, REST_PRESENCE_LEN, "NOT PRESENT");
	}

	return;
}

static int get_fan_availible_action(unsigned char db_name, avail_action_t* _action, memdb_integer node_id)
{
	char buff[RMM_NAME_LEN] = {0};
	avail_action_t *action = _action;

	strncpy_safe((char*)action->action, RMM_JSON_REQ_STATE_CHANGE, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[0].property, RMM_JSON_ENABLE_STATE, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(db_name,node_id, FAN_AV_RSC_EN_VAL_1_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[0].av, buff, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[1].property, RMM_JSON_ENABLE_STATE, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(db_name,node_id, FAN_AV_RSC_EN_VAL_2_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[1].av, buff, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	action++;

	strncpy_safe((char*)action->action, RMM_JSON_SET_DESIRED_SPEED, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[0].property, RMM_JSON_UNIT_TYPE, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(db_name,node_id, FAN_AV_SDS_UNIT_1_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[0].av, buff, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[1].property, RMM_JSON_UNIT_TYPE, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(db_name,node_id, FAN_AV_SDS_UNIT_2_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[1].av, buff, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	strncpy_safe((char*)action->cap[2].property, RMM_JSON_DESIRED_SPEED, RMM_NAME_LEN, RMM_NAME_LEN - 1);

	return 0;
}

result_t libwrap_pre_put_tzone_by_idx(int tzone_idx, put_tzone_t *put_tzone_info)
{
	memdb_integer cm_node_id = 0;
	memdb_integer tzone_node_id = 0;
	int64 error_code = 0;
	char output[128] = {0};
	int cm_lid = 0;
	int tzone_lid = 0;

	cm_lid = get_cm_lid(MC_TYPE_TZONE, tzone_idx);
	tzone_lid = get_zone_lid(MC_TYPE_TZONE, tzone_idx);
	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	if (cm_node_id == 0)
		return RESULT_NO_NODE;

	tzone_node_id = get_subnode_id_by_lid(tzone_lid, cm_node_id, MC_TYPE_TZONE);
	if (tzone_node_id == 0)
		return RESULT_NO_NODE;

	error_code = libdb_attr_get_string(DB_RMM, tzone_node_id, TZONE_DESCRIPT_STR, output, 128, LOCK_ID_NULL);
	if(error_code != 0) {
		return RESULT_ATTR_ERR;
	}
	strncpy_safe((char*)put_tzone_info->descr, output, DESCRIPTION_LEN, DESCRIPTION_LEN - 1);
	return RESULT_OK;
}

result_t libwrap_put_tzone_by_idx(int tzone_idx, const put_tzone_t put_tzone_info)
{
	memdb_integer cm_node_id = 0;
	memdb_integer tzone_node_id = 0;
	int cm_lid = 0;
	int tzone_lid = 0;
	int rc = 0;

	cm_lid = get_cm_lid(MC_TYPE_TZONE, tzone_idx);
	tzone_lid = get_zone_lid(MC_TYPE_TZONE, tzone_idx);
	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	if (cm_node_id == 0)
		return RESULT_NO_NODE;

	tzone_node_id = get_subnode_id_by_lid(tzone_lid, cm_node_id, MC_TYPE_TZONE);
	if (tzone_node_id == 0)
		return RESULT_NO_NODE;

	rc = libdb_attr_set_string(DB_RMM, tzone_node_id, TZONE_DESCRIPT_STR,
						  0x0, (char*)put_tzone_info.descr, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}
	return RESULT_OK;
}

static int get_fan_by_idx(uint32 tzone_idx, uint32 fan_idx, struct fan_member *fan_member)
{
	int cm_lid = get_cm_lid(MC_TYPE_TZONE, tzone_idx);
	int cm_tzone_idx = get_zone_lid(MC_TYPE_TZONE, tzone_idx);
	int fan_lid = fan_idx;
	result_t rc;
	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	memdb_integer tzone_node_id = get_subnode_id_by_lid(cm_tzone_idx, cm_node_id, MC_TYPE_TZONE);
	memdb_integer fan_node_id = get_subnode_id_by_lid(fan_idx, tzone_node_id, MC_TYPE_FAN);
	#ifdef ODATA_ADD_ATTR
	char buff[RMM_NAME_LEN] = {0};
	#endif

	rc = am_get_fan_pwm_by_id(&cm_node_id, &fan_node_id, fan_idx);
	if (rc != RESULT_OK)
		return rc;

	fan_member->be.id = get_db_info_num(DB_RMM, fan_node_id, FAN_LOC_ID_STR);

	get_db_info_string(DB_RMM, fan_node_id, FAN_UUID_STR,fan_member->be.uuid,UUID_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_NAME_STR,fan_member->be.name,RMM_NAME_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_DESCRIPT_STR,fan_member->be.desc,DESCRIPTION_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_CREATE_DATE_STR,fan_member->be.create_date,DATE_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_UPDATE_DATE_STR,fan_member->be.update_date,DATE_LEN);

	fan_member->enable_state  = get_db_info_num(DB_RMM, fan_node_id, FAN_ENABLE_STATE_STR);
	fan_member->health_state  = get_db_info_num(DB_RMM, fan_node_id, FAN_HEALTH_STATE_STR);

	fan_member->tachometer.reading = (uint32)get_db_info_num(DB_RMM, fan_node_id, FAN_TACH_READ_STR);
	get_db_info_string(DB_RMM, fan_node_id, FAN_TACH_STATE_STR,fan_member->tachometer.current_state,RMM_NAME_LEN);
	fan_member->tachometer.threshold.lower_non_critical = get_db_info_num(DB_RMM, fan_node_id, FAN_TACH_THRESH_LNC_STR);
	fan_member->tachometer.threshold.lower_critical     = get_db_info_num(DB_RMM, fan_node_id, FAN_TACH_THRESH_LC_STR);  
	fan_member->tachometer.threshold.upper_non_critical = get_db_info_num(DB_RMM, fan_node_id, FAN_TACH_THRESH_UNC_STR);
	fan_member->tachometer.threshold.upper_critical     = get_db_info_num(DB_RMM, fan_node_id, FAN_TACH_THRESH_UC_STR);

	get_db_info_string(DB_RMM, fan_node_id, FAN_SER_NUM_STR, fan_member->asset.fru.serial_num, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_MANUFACT_STR, fan_member->asset.fru.manufacture, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_MODEL_STR, fan_member->asset.fru.model, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_PART_NUM_STR, fan_member->asset.fru.part_num, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_FW_VER_STR, fan_member->asset.fru.fw_ver, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, fan_node_id, FAN_ASSET_TAG_STR,fan_member->asset.asset_tag, REST_ASSET_TAG_LEN);

	get_fan_present_by_node_id(tzone_node_id, fan_idx, fan_member);
	get_fan_availible_action(DB_RMM, fan_member->av_action, fan_node_id);
#if 0
	get_db_info_string(DB_RMM, fan_node_id, FAN_DESIRED_SPD_PWM_STR,fan_member->desired_speed,RMM_NAME_LEN);
#else
	snprintf((char *)fan_member->desired_speed, RMM_NAME_LEN, "%s", "N/A");
#endif

#ifdef ODATA_ADD_ATTR
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, fan_node_id, UNITS, (uint8 *)buff, RMM_NAME_LEN);
	fan_member->fan_loc.units = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, fan_node_id, X_LOC, (uint8 *)buff, RMM_NAME_LEN);
	fan_member->fan_loc.x_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, fan_node_id, U_LOC, (uint8 *)buff, RMM_NAME_LEN);
	fan_member->fan_loc.u_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, fan_node_id, U_HEIGHT, (uint8 *)buff, RMM_NAME_LEN);
	fan_member->fan_loc.u_height = (uint32)atoi((const char *)buff);
#endif

	return 0;
}

result_t libwrap_get_tzone_fan_by_idx(uint32 tzone_idx, uint32 fan_idx, struct fan_member *fan_member)
{
	return get_fan_by_idx(tzone_idx, fan_idx, fan_member);
}

result_t libwrap_pre_put_fan(uint32 tzone_idx, uint32 fan_idx, put_fan_t *put_fan_info)
{
	memdb_integer fan_node_id;
	char value[32] = {0};
	char output[32] = {0};

	get_fan_node_id_by_idx(tzone_idx, fan_idx, &fan_node_id);
	if(0 == fan_node_id)
		return RESULT_NO_NODE;

	libdb_attr_get_string(DB_RMM, fan_node_id, FAN_DESCRIPT_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe((char *)put_fan_info->descr, output, DESCRIPTION_LEN, DESCRIPTION_LEN - 1);
	libdb_attr_get_string(DB_RMM, fan_node_id, FAN_ASSET_TAG_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe((char *)put_fan_info->asset_tag, output, REST_ASSET_TAG_LEN, REST_ASSET_TAG_LEN - 1);

	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, fan_node_id, FAN_TACH_THRESH_LNC_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe(value, output, sizeof(value), sizeof(value) - 1);
	put_fan_info->threshold.lower_non_critical = (uint32)atoi(value);

	memset(value, 0, sizeof(value));
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, fan_node_id, FAN_TACH_THRESH_UNC_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe(value, output, sizeof(value), sizeof(value) - 1);
	if (strlen(value) > 10)
		return RESULT_LEN_ERR;
	put_fan_info->threshold.upper_non_critical = (uint32)atoi(value);

	memset(value, 0, sizeof(value));
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, fan_node_id, FAN_TACH_THRESH_LC_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe(value, output, sizeof(value), sizeof(value) - 1);
	if (strlen(value) > 10)
		return RESULT_LEN_ERR;
	put_fan_info->threshold.lower_critical = (uint32)atoi(value);

	memset(value, 0, sizeof(value));
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, fan_node_id, FAN_TACH_THRESH_UC_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe(value, output, sizeof(value), sizeof(value) - 1);
	if (strlen(value) > 10)
		return RESULT_LEN_ERR;
	put_fan_info->threshold.upper_critical = (uint32)atoi(value);

	return RESULT_OK;
}

result_t libwrap_put_fan(uint32 tzone_idx, uint32 fan_idx, const put_fan_t put_fan_info)
{
	memdb_integer fan_node_id = 0;
	char value[32] = {0};
	int rc = 0;

	get_fan_node_id_by_idx(tzone_idx, fan_idx, &fan_node_id);
	if(0 == fan_node_id)
		return RESULT_NO_NODE;

	rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_DESCRIPT_STR,
						  0x0, (char *)put_fan_info.descr, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_ASSET_TAG_STR,
							  0x0, (char *)put_fan_info.asset_tag, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	snprintf(value, sizeof(value), "%d", put_fan_info.threshold.lower_non_critical);
	rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_TACH_THRESH_LNC_STR,
												0, value, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	snprintf(value, sizeof(value), "%d", put_fan_info.threshold.upper_non_critical);
	rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_TACH_THRESH_UNC_STR,
												0, value, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	snprintf(value, sizeof(value), "%d", put_fan_info.threshold.lower_critical);
	rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_TACH_THRESH_LC_STR,
												0, value, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	snprintf(value, sizeof(value), "%d", put_fan_info.threshold.upper_critical);
	rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_TACH_THRESH_UC_STR,
												0, value, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	return RESULT_OK;
}


result_t libwrap_post_fan_set_speed(uint32 tzone_idx, uint32 fan_idx, uint32 type, char *speed)
{
	memdb_integer fan_node_id = 0;
	char buff[64] = {0};
	int spd = atoi(speed);
	int rc;

	if ((rc = check_module_capability(JRPC_GAMI_REG_CAP_FAN, RMM_JSON_SET_DESIRED_SPEED)) != RESULT_OK)
		return rc;

	get_fan_node_id_by_idx(tzone_idx, fan_idx, &fan_node_id);
	if(0 == fan_node_id)
		return RESULT_NO_NODE;

	/* unit is rpm */
	if(type == 0) {
		snprintf(buff, sizeof(buff), "%s%s ", speed, "  RPM");
		rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_DESIRED_SPD_RPM_STR,
								  0x0, buff, SNAPSHOT_NEED, LOCK_ID_NULL);
		if (rc == -1) {
			return RESULT_ATTR_ERR;
		}
	} else {
		/* unit is pwm */
		am_set_fan_pwm(tzone_idx, fan_idx, spd);
		snprintf(buff, sizeof(buff), "%s%s ", speed, "  PWM");
		rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_DESIRED_SPD_PWM_STR,
								  0x0, buff, SNAPSHOT_NEED, LOCK_ID_NULL);
		if (rc == -1) {
			return RESULT_ATTR_ERR;
		}

		if(spd == 0) {
			rc = libdb_attr_set_int(DB_RMM, fan_node_id, FAN_ENABLE_STATE_STR,
					  0x0, FAN_STATE_DISABLED, SNAPSHOT_NEED, LOCK_ID_NULL);
			if (rc == -1) {
				return RESULT_ATTR_ERR;
			}
		} else {
			rc = libdb_attr_set_int(DB_RMM, fan_node_id, FAN_ENABLE_STATE_STR,
					  0x0, FAN_STATE_ENABLED, SNAPSHOT_NEED, LOCK_ID_NULL);
			if (rc == -1) {
				return RESULT_ATTR_ERR;
			}

			rc = libdb_attr_set_string(DB_RMM, fan_node_id, FAN_SPEED_PWM_RESTORE_STR,
								  0x0, speed, SNAPSHOT_NEED_NOT, LOCK_ID_NULL);
			if (rc == -1) {
				return RESULT_ATTR_ERR;
			}
		}
	}

	return RESULT_OK;
}


static result_t fan_change_state(uint32 tzone_idx, int fan_idx, 
								memdb_integer fan_node_id,
								int state)
{
	int pwm = 0;
	char ppwm[128] = {0};
	int rc = 0;

	rc = libdb_attr_set_int(DB_RMM, fan_node_id, FAN_ENABLE_STATE_STR,
						  0x0, state, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	if(state == FAN_STATE_ENABLED) {
		libdb_attr_get_string(DB_RMM, fan_node_id, FAN_SPEED_PWM_RESTORE_STR, ppwm, 128, LOCK_ID_NULL);
		pwm = atoi(ppwm);
	}
	else if(state == FAN_STATE_DISABLED){
		pwm = 0;
	}

	return am_set_fan_pwm(tzone_idx, fan_idx, pwm);
}

result_t libwrap_post_fan_change_state(uint32 tzone_idx, uint32 fan_idx, int64 state)
{
	memdb_integer fan_node_id = 0;
	result_t rc;

	if ((rc = check_module_capability(JRPC_GAMI_REG_CAP_FAN, RMM_JSON_REQ_STATE_CHANGE)) != RESULT_OK)
		return rc;

	get_fan_node_id_by_idx(tzone_idx, fan_idx, &fan_node_id);
	if(0 == fan_node_id)
		return RESULT_NO_NODE;

	return fan_change_state(tzone_idx, fan_idx, fan_node_id, state);
}

static result_t get_fan_collection(uint32 tzone_idx, collections_t* fan/*[PSU_PER_POWERZONE]*/, uint32 *number)
{
	memdb_integer cm_node_id,tzone_node_id,fan_node_id;
	int i;
	int fan_num = 0;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 48] = {0};
	int cm_lid = get_cm_lid(MC_TYPE_TZONE, tzone_idx);
	int tzone_lid = get_zone_lid(MC_TYPE_TZONE, tzone_idx);

	if (libutils_get_ip((char *)value) < 0) {
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);
		memcpy(value, "x", 1);
	}

	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	tzone_node_id = get_subnode_id_by_lid(tzone_lid, cm_node_id, MC_TYPE_TZONE);

	for(i = 0; i < MAX_PWM_NUM; i++) {
		fan_node_id = get_subnode_id_by_lid(i + 1, tzone_node_id, MC_TYPE_FAN);
		if(fan_node_id != INVALID_NODE_ID) {    
			fan->id = (i + 1);
			get_db_info_string(DB_RMM, fan_node_id, FAN_UUID_STR, fan->uuid, UUID_LEN);
			get_db_info_string(DB_RMM, fan_node_id, FAN_NAME_STR, fan->name, RMM_NAME_LEN);
			memset(prefix, 0, MAX_URL);
			memset(new_link, 0, MAX_URL + 48);
			rmm_cfg_get_rest_prefix(prefix, MAX_URL);
			insert_str(new_link, "http://%s:%d/rack/thermalzones/%d/fans/%d", 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/

			snprintf((char *)(fan->href), DESCRIPTION_LEN, new_link,
					(char *)value, rmm_cfg_get_port(RESTD_PORT), tzone_idx, fan->id);
			fan++;
			fan_num++;
		}
	}
	*number = fan_num;
	return RESULT_OK;
}

result_t libwrap_get_tzone_fan_coll(uint32 tzone_idx, collections_t* fan, uint32 *number)
{
	return get_fan_collection(tzone_idx, fan, number);
}

static int get_tzone_by_id(uint32 tzone_idx, struct tzone_member *cz_member)
{
	int cm_lid = get_cm_lid(MC_TYPE_TZONE, tzone_idx);
	int tzone_lid = get_zone_lid(MC_TYPE_TZONE, tzone_idx);
	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	memdb_integer tzone_node_id = get_subnode_id_by_lid(tzone_lid, cm_node_id, MC_TYPE_TZONE);
	char buff[RMM_NAME_LEN] = {0};

	cz_member->be.id = tzone_idx;
//		get_db_info_num(DB_RMM, tzone_node_id, TZONE_INDEX_STR);
	get_db_info_string(DB_RMM, tzone_node_id, 
		TZONE_UUID_STR, cz_member->be.uuid, UUID_LEN);
	get_db_info_string(DB_RMM, tzone_node_id, 
		TZONE_NAME_STR, cz_member->be.name, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, tzone_node_id, 
		TZONE_DESCRIPT_STR, cz_member->be.desc, DESCRIPTION_LEN);
	get_db_info_string(DB_RMM, tzone_node_id, 
		TZONE_CREATE_DATE_STR, cz_member->be.create_date, DATE_LEN);
	get_db_info_string(DB_RMM, tzone_node_id, 
		TZONE_UPDATE_DATE_STR, cz_member->be.update_date, DATE_LEN);
	get_db_info_string(DB_RMM, tzone_node_id,
		TZONE_POLICY_STR, cz_member->policy, DATE_LEN);

    #ifdef ODATA_ADD_ATTR
    cz_member->pres_temp = 
		get_db_info_num(DB_RMM, tzone_node_id, TZ_PRESENT_TEMP);//"cz_present_temperature"
    cz_member->outlet_temp = 
		get_db_info_num(DB_RMM, tzone_node_id, TZ_OUTLET_TEMP);//"cz_outlet_temperature"
    cz_member->volumetric_airflow = 
		get_db_info_num(DB_RMM, tzone_node_id, TZ_VOLUM_AIRFLOW);//"cz_volumetric_airflow"
    cz_member->max_fan_sup = 
		get_db_info_num(DB_RMM, tzone_node_id, TZ_MAX_FAN_SUP);//"cz_max_fan_support"
		
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, tzone_node_id, UNITS, (uint8 *)buff, RMM_NAME_LEN);
	cz_member->tzone_loc.units = (uint32)atoi((const char *)buff);
    #endif	
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, tzone_node_id, X_LOC, (uint8 *)buff, RMM_NAME_LEN);
	cz_member->tzone_loc.x_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, tzone_node_id, U_LOC, (uint8 *)buff, RMM_NAME_LEN);
	cz_member->tzone_loc.u_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, tzone_node_id, U_HEIGHT, (uint8 *)buff, RMM_NAME_LEN);
	cz_member->tzone_loc.u_height = (uint32)atoi((const char *)buff);

	memset(cz_member->presence, 0, REST_PRESENCE_LEN);
	get_present_string(tzone_node_id, cz_member->presence, REST_PRESENCE_LEN);

	return 0;
}

result_t libwrap_get_tzone_by_idx(uint32 tzone_idx, struct tzone_member *tzone_member)
{
	return get_tzone_by_id(tzone_idx, tzone_member);
}

static result_t get_thermalzone_collection(collections_t* tzone, uint32 *number)
{
    struct node_info *subnode = NULL;
	int subnode_num = 0;
	int i;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 48] = {0};
	int cm_lid = 0;
	int max_tz_num = 0;
	int tz_lid = 0;

	if (libutils_get_ip((char *)value) < 0) {
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);
		memcpy(value, "x", 1);
	}

	subnode = libdb_list_node_by_type(MC_TYPE_RMC, MC_TYPE_TZONE, MC_TYPE_TZONE, &subnode_num, memdb_filter, LOCK_ID_NULL);
	if (subnode_num == 0) {
		libdb_free_node(subnode);
		return RESULT_NO_NODE;
	}

	for (i = 0; i < subnode_num; i++) {
		cm_lid = get_db_info_num(DB_RMM, subnode[i].parent, MBP_LOC_ID_STR);
		max_tz_num = MAX_TZONE_NUM;
		tz_lid = get_db_info_num(DB_RMM, subnode[i].node_id, TZONE_LOC_ID_STR);
		tzone->id = tz_lid + (cm_lid - 1) * max_tz_num;
		if (tzone->id < 1) {
			tzone->id = 0;
			continue;
		}

		get_db_info_string(DB_RMM, subnode[i].node_id, TZONE_UUID_STR, tzone->uuid, UUID_LEN);
		get_db_info_string(DB_RMM, subnode[i].node_id, TZONE_NAME_STR, tzone->name, RMM_NAME_LEN);
		memset(prefix, 0, MAX_URL);
		memset(new_link, 0, MAX_URL + 48);
		rmm_cfg_get_rest_prefix(prefix, MAX_URL);
		insert_str(new_link, "http://%s:%d/rack/thermalzones/%d/", 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/
		snprintf((char *)(tzone->href), DESCRIPTION_LEN, new_link,
					(char *)value, rmm_cfg_get_port(RESTD_PORT), tzone->id);
		tzone++;
	}

	*number = subnode_num;
	libdb_free_node(subnode);

	return RESULT_OK;
}

result_t libwrap_get_node_id_by_uuid(unsigned char *uuid,
									 memdb_integer *node_id)
{
	struct node_info *n = NULL;
	int node_num = 0;
	int i = 0;
	void *attr = NULL;
	struct attr_info *attr_info = NULL;
	int attr_info_offset = 0;
	int size = 0;
	char attr_name[256] = {0};
	char attr_data[256] = {0};

	n = libdb_list_node_by_type(DB_RMM, MC_TYPE_DRAWER, MC_TYPE_DRAWER, &node_num, NULL, LOCK_ID_NULL);
	for (i = 0; i < node_num; i++) {
		attr = libdb_list_attrs_by_node(DB_RMM, n[i].node_id, &size, LOCK_ID_NULL);

		foreach_attr_info(attr_info, attr_info_offset, attr, size) {
			memset(attr_name, 0, 256);
			memset(attr_data, 0, 256);

			memcpy(attr_name, (char *)attr_info->elems, attr_info->data_offset);
			memcpy(attr_data, (char *)attr_info->elems + attr_info->data_offset,
				   attr_info->data_len);

			if (strcmp((char *)uuid, attr_data) == 0) {
				*node_id = n[i].node_id;
				libdb_free_node(n);
				return 0;
			}
		}
	}

	libdb_free_node(n);
	return -1;
}

static int get_dzone_by_id(uint32 rack_dzone_idx, struct dzone_member *dz_number)
{
	int cm_lid = get_cm_lid(MC_TYPE_DZONE, rack_dzone_idx);
	int dzone_lid = get_zone_lid(MC_TYPE_DZONE, rack_dzone_idx);
	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	memdb_integer dzone_node_id = get_subnode_id_by_lid(dzone_lid, cm_node_id, MC_TYPE_DZONE);
	char buff[RMM_NAME_LEN] = {0};

	dz_number->be.id = 
		get_db_info_num(DB_RMM, dzone_node_id, DZONE_LOC_ID_STR);
	get_db_info_string(DB_RMM, dzone_node_id, 
		TZONE_UUID_STR, dz_number->be.uuid, UUID_LEN);
	get_db_info_string(DB_RMM, dzone_node_id, 
		TZONE_NAME_STR, dz_number->be.name, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, dzone_node_id, 
		TZONE_DESCRIPT_STR, dz_number->be.desc, DESCRIPTION_LEN);
	get_db_info_string(DB_RMM, dzone_node_id, 
		TZONE_CREATE_DATE_STR, dz_number->be.create_date, DATE_LEN);
	get_db_info_string(DB_RMM, dzone_node_id, 
		TZONE_UPDATE_DATE_STR, dz_number->be.update_date, DATE_LEN);

	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, dzone_node_id, X_LOC, (uint8 *)buff, RMM_NAME_LEN);
	dz_number->dzone_loc.x_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, dzone_node_id, U_LOC, (uint8 *)buff, RMM_NAME_LEN);
	dz_number->dzone_loc.u_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, dzone_node_id, U_HEIGHT, (uint8 *)buff, RMM_NAME_LEN);
	dz_number->dzone_loc.u_height = (uint32)atoi((const char *)buff);

	memset(dz_number->presence, 0, REST_PRESENCE_LEN);
	get_present_string(dzone_node_id, dz_number->presence, REST_PRESENCE_LEN);

	return 0;
}

result_t libwrap_get_tzone_coll(collections_t* tzone, uint32 *number)
{
	return get_thermalzone_collection(tzone, number);
}

static result_t get_drawerzone_collection(collections_t* dzone, uint32 *number)
{
    struct node_info *subnode = NULL;
	int subnode_num = 0;
	int i;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 48] = {0};
	int cm_lid = 0;
	int max_dz_num = 0;
	int dz_lid = 0;

	if (libutils_get_ip((char *)value) < 0) {
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);
		memcpy(value, "x", 1);
	}

	subnode = libdb_list_node_by_type(MC_TYPE_RMC, MC_TYPE_DZONE, MC_TYPE_DZONE, &subnode_num, memdb_filter,LOCK_ID_NULL);

	for (i = 0; i < subnode_num; i++) {
		cm_lid = get_db_info_num(DB_RMM, subnode[i].parent, MBP_LOC_ID_STR);
		max_dz_num = MAX_TZONE_NUM;
		dz_lid = get_db_info_num(DB_RMM, subnode[i].node_id, TZONE_LOC_ID_STR);
		dzone->id = dz_lid + (cm_lid -1) * max_dz_num;
		if (dzone->id < 1) {
			dzone->id = 0;
			continue;
		}

		get_db_info_string(DB_RMM, subnode[i].node_id, DZONE_UUID_STR, dzone->uuid, UUID_LEN);
		get_db_info_string(DB_RMM, subnode[i].node_id, DZONE_NAME_STR, dzone->name, RMM_NAME_LEN);
		memset(prefix, 0, MAX_URL);
		memset(new_link, 0, MAX_URL + 48);
		rmm_cfg_get_rest_prefix(prefix, MAX_URL);
		insert_str(new_link, "http://%s:%d/rack/drawerzone/%d/", 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/
		snprintf((char *)(dzone->href), DESCRIPTION_LEN, new_link,
					(char *)value, rmm_cfg_get_port(RESTD_PORT), dzone->id);
		dzone++;
	}

	*number = subnode_num;
	libdb_free_node(subnode);

	return RESULT_OK;
}

result_t libwrap_get_dzone_by_idx(uint32 tzone_idx, dzone_member_t *tzone_member)
{
	return get_dzone_by_id(tzone_idx, tzone_member);
}


result_t libwrap_get_dzone_coll(collections_t *dzone, uint32 *number)
{
	return get_drawerzone_collection(dzone, number);
}

result_t libwrap_pre_put_drawer(uint32 dzone_idx, uint32 drawer_idx, put_drawer_t *put_drawer_info)
{
	memdb_integer drawer_node_id;

	char output[128] = {0};
	int64 error_code = 0;
	get_drawer_id_by_idx(dzone_idx, drawer_idx, &drawer_node_id);
	libdb_attr_get_string(DB_RMM, drawer_node_id, DRAWER_DESCRIPT_STR, output, 128, LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}	
	strncpy_safe((char*)put_drawer_info->descr, output, DESCRIPTION_LEN, DESCRIPTION_LEN - 1);

	return RESULT_OK;
}

result_t libwrap_put_drawer(int32 dzone_idx, int32 drawer_idx, const put_drawer_t put_drawer_info)
{

	memdb_integer drawer_id = 0;
	get_drawer_id_by_idx(dzone_idx, drawer_idx, &drawer_id);
	if (libdb_attr_set_string(DB_RMM, drawer_id, DRAWER_DESCRIPT_STR,
						  0x0, (char*)put_drawer_info.descr, SNAPSHOT_NEED, LOCK_ID_NULL) == -1) {
		return RESULT_ATTR_ERR;
	}

	return RESULT_OK;
}

static int get_drawer_by_idx(int32 dzone_idx, int32 drawer_idx, drawer_member_t *drawer_member)
{
	int cm_lid = get_cm_lid(MC_TYPE_DZONE, dzone_idx);
	int dzone_lid = get_zone_lid(MC_TYPE_DZONE, dzone_idx);
	int drawer_lid = drawer_idx;

	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	memdb_integer dzone_node_id = get_subnode_id_by_lid(dzone_lid, cm_node_id, MC_TYPE_DZONE);
	memdb_integer drawer_node_id = get_subnode_id_by_lid(drawer_lid, dzone_node_id, MC_TYPE_DRAWER);

	char buff[RMM_NAME_LEN] = {0};
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 48] = {0};

	if (0 == drawer_node_id )
		return RESULT_NO_NODE;

	if (libutils_get_ip((char *)value) < 0) {
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);
		strncpy_safe(value, "x", WRAP_DB_MAX_VALUE_LEN, 1);
	}

	drawer_member->be.id = 
		get_db_info_num(DB_RMM, drawer_node_id, DRAWER_LOC_ID_STR) + (cm_lid - 1) * dzone_lid * MAX_DRAWER_NUM;
	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_UUID_STR, drawer_member->be.uuid, UUID_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_NAME_STR, drawer_member->be.name, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_DESCRIPT_STR, drawer_member->be.desc, DESCRIPTION_LEN);

	rmm_cfg_get_rest_prefix(prefix, MAX_URL);
	insert_str(new_link, "http://%s:%d/rack/drawers/%s", 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/

	snprintf((char *)(drawer_member->href), DESCRIPTION_LEN, new_link,
			(char *)value, rmm_cfg_get_port(RESTD_PORT), drawer_member->be.uuid);

	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_CREATE_DATE_STR, drawer_member->be.create_date, DATE_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_UPDATE_DATE_STR, drawer_member->be.update_date, DATE_LEN);
    memset(drawer_member->TMC_ip_addr, 0, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_TMC_IP_STR, drawer_member->TMC_ip_addr, REST_RACK_STRING_LEN);	

	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_PUID_STR, drawer_member->tray_puid, REST_RACK_STRING_LEN);
	drawer_member->tray_ruid = 
		(uint32)get_db_info_num(DB_RMM, drawer_node_id, DRAWER_RUID_STR);
	drawer_member->power_consumption = 
		(uint32)get_db_info_num(DB_RMM, drawer_node_id, DRAWER_PWR_CONSUM_STR);
	drawer_member->thermal_st = 
		(uint32)get_db_info_num(DB_RMM, drawer_node_id, DRAWER_THERMAL_ST_STR);

	snprintf((char *)(drawer_member->av_action.action), RMM_NAME_LEN, "%s", RMM_JSON_RESET);
	snprintf((char *)(drawer_member->av_action.cap[0].property), RMM_NAME_LEN, "%s", RMM_JSON_RESET_TYPE);
	snprintf((char *)(drawer_member->av_action.cap[1].property), RMM_NAME_LEN, "%s", RMM_JSON_RESET_TYPE);
	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_AV_RST_TYPE_1_STR, drawer_member->av_action.cap[0].av, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, 
		DRAWER_AV_RST_TYPE_2_STR, drawer_member->av_action.cap[1].av, RMM_NAME_LEN);	

    #ifdef ODATA_ADD_ATTR
	drawer_member->pres_temp = 
		(uint32)get_db_info_num(DB_RMM, drawer_node_id, DRAWER_PRES_TEMP);	
	//get_db_info_string(DB_RMM, drawer_node_id, DRAWER_POD, drawer_member->pod, REST_RACK_STRING_LEN);
	memset(drawer_member->pod, 0, REST_RACK_STRING_LEN);
    memcpy(drawer_member->pod, "N/A", strlen("N/A"));
	//get_db_info_string(DB_RMM, drawer_node_id, DRAWER_RACK, drawer_member->rack, REST_RACK_STRING_LEN);
	memset(drawer_member->rack, 0, REST_RACK_STRING_LEN);
    get_db_info_string(DB_RMM, MC_TYPE_RMC, RACK_UUID_STR, drawer_member->rack, UUID_LEN);
    
	//get_db_info_string(DB_RMM, drawer_node_id, DRAWER_RMM_PRES, (uint8*)(drawer_member->rmm_present), REST_RACK_STRING_LEN);
	drawer_member->rmm_present = 1; ///always true
	//get_db_info_string(DB_RMM, drawer_node_id, DRAWER_DISAGGR, (uint8*)(drawer_member->rack_disaggr), REST_RACK_STRING_LEN);
	drawer_member->rack_disaggr = 0; ///false
	
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, UNITS, (uint8 *)buff, RMM_NAME_LEN);
	drawer_member->loc.units = (uint32)atoi((const char *)buff);
    #endif
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, X_LOC, (uint8 *)buff, RMM_NAME_LEN);
	drawer_member->loc.x_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, U_LOC, (uint8 *)buff, RMM_NAME_LEN);
	drawer_member->loc.u_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, drawer_node_id, U_HEIGHT, (uint8 *)buff, RMM_NAME_LEN);
	drawer_member->loc.u_height = (uint32)atoi((const char *)buff);
	
	return 0;
}

static result_t check_module_capability(char *module, char *cap_name)
{
	int i = 0;
	char *cap= NULL;
	int cap_len = 0;
	int nodenum = 0;
	char query[MAX_NAME_LEN] = {0};
	char buff[256] = {0};
	char ip_str[64] = {0};
	memdb_integer module_node_id = 0;
	memdb_integer reg_node_id = 0;
	struct node_info *subnode = NULL;

	if ((NULL == module) || (NULL == cap_name))
		return RESULT_LEN_ERR;

	cap_len = strlen(cap_name);

	subnode = libdb_list_subnode_by_type(DB_RMM, MC_TYPE_RMC, MC_TYPE_REG, &nodenum, NULL, LOCK_ID_NULL);
	if (nodenum != 1) {
		/*printf("reg node number is not right\n");*/
		return RESULT_NO_NODE;
	}

	reg_node_id = subnode[0].node_id;

	subnode = libdb_list_subnode_by_type(DB_RMM, reg_node_id, MC_TYPE_REG_MODULE, &nodenum, NULL, LOCK_ID_NULL);
	for (i = 0; i < nodenum; i++) {
		libdb_attr_get_string(DB_RMM, subnode[i].node_id, JRPC_GAMI_REG_NAME, query, MAX_NAME_LEN, LOCK_ID_NULL);
		if (strcmp(query, JRPC_GAM_REG_ASSETMODULE) == 0)
			break;
	}

	if (nodenum == i) {
		/*printf("asset module is not registed.\n");*/
		return RESULT_NO_NODE;
	} else
		module_node_id = subnode[i].node_id;

	libdb_attr_get_string(DB_RMM, module_node_id, module, buff, 256, LOCK_ID_NULL);

	for (i = 0; i < GAMI_MAX_CAP_NUM; i++) {
		if (i == 0)
			cap = buff;
		else
			cap = strstr(cap, GAMI_CAP_SPLIT_STR) + 1;

		if (cap == NULL)
			return RESULT_ATTR_ERR;

		if (*cap == '\0')
			return RESULT_ATTR_ERR;

		if (strncmp(cap, cap_name, cap_len) == 0 && strncmp(cap+cap_len, GAMI_CAP_SPLIT_STR, 1) == 0)
			return RESULT_OK;
	}

	return RESULT_OTHER_ERR;
}

result_t check_asset_module_capability(char *cap_name)
{
	int i = 0;
	char *cap= NULL;
	int cap_len = strlen(cap_name);
	int nodenum = 0;
	char query[MAX_NAME_LEN] = {0};
	char buff[256] = {0};
	char ip_str[64] = {0};
	memdb_integer module_node_id = 0;
	memdb_integer reg_node_id = 0;
	struct node_info *subnode = NULL;

	subnode = libdb_list_subnode_by_type(DB_RMM, MC_TYPE_RMC, MC_TYPE_REG, &nodenum, NULL, LOCK_ID_NULL);
	if (nodenum != 1) {
		/*printf("reg node number is not right\n");*/
		return RESULT_NO_NODE;
	}

	reg_node_id = subnode[0].node_id;

	subnode = libdb_list_subnode_by_type(DB_RMM, reg_node_id, MC_TYPE_REG_MODULE, &nodenum, NULL, LOCK_ID_NULL);
	for (i = 0; i < nodenum; i++) {
		libdb_attr_get_string(DB_RMM, subnode[i].node_id, JRPC_GAMI_REG_NAME, query, MAX_NAME_LEN, LOCK_ID_NULL);
		if (strcmp(query, JRPC_GAM_REG_ASSETMODULE) == 0)
			break;
	}

	if (nodenum == i) {
		/*printf("asset module is not registed.\n");*/
		return RESULT_NO_NODE;
	} else
		module_node_id = subnode[i].node_id;

	libdb_attr_get_string(DB_RMM, module_node_id, JRPC_GAMI_REG_CAP_MBP, buff, 256, LOCK_ID_NULL);

	for (i = 0; i < GAMI_MAX_CAP_NUM; i++) {
		if (i == 0)
			cap = buff;
		else
			cap = strstr(cap, GAMI_CAP_SPLIT_STR) + 1;

		if (cap == NULL)
			return RESULT_ATTR_ERR;

		if (*cap == '\0')
			return RESULT_ATTR_ERR;

		if (strncmp(cap, cap_name, cap_len) == 0 && strncmp(cap+cap_len, GAMI_CAP_SPLIT_STR, 1) == 0)
			return RESULT_OK;
	}

	return RESULT_OTHER_ERR;
}

result_t libwrap_get_drawer_by_idx(int32 dzone_idx, int32 drawer_idx, drawer_member_t *drawer_member)
{
	return get_drawer_by_idx(dzone_idx, drawer_idx, drawer_member);
}

result_t cm_hard_reset(int cm_idx, int *result)
{
	return am_peripheral_hard_reset(cm_idx, 0, result);
}

result_t cm_uart_switch(int cm_idx, int tray_idx, int tgt_idx)
{
	return am_uart_switch(cm_idx, tray_idx, tgt_idx);
}

result_t drawer_hard_reset(int32 dzone_idx, int32 drawer_idx, int* result)
{
	drawer_idx =  (dzone_idx - 1) * MAX_DRAWER_NUM + drawer_idx;
	int peripheral_id = drawer_idx * 2 - 1;
	return am_peripheral_hard_reset(dzone_idx, peripheral_id, result);
}

result_t rmm_factory_reset(int* result)
{
	return assetd_rmm_factory_rest(result);
}

result_t libwrap_set_id_field(memdb_integer *node_id, int32 field_type, int32 field_instance, int32 byte_num, int32 *data)
{
	return am_set_id_field(node_id, field_type, field_instance, byte_num, data);
}

result_t libwrap_get_id_field(memdb_integer *node_id, int32 field_instance)
{
	return am_get_id_field(node_id, field_instance);
}

result_t libwrap_get_aggregate_thermal_sensor(memdb_integer *node_id)
{
	return am_get_aggregate_thermal_sensor(node_id);
}

result_t libwrap_get_aggregate_health_sensor(memdb_integer *node_id)
{
	return am_get_aggregate_health_sensor(node_id);
}

result_t libwrap_get_tray_power(memdb_integer *node_id)
{
	return am_get_tray_power(node_id);
}

result_t libwrap_mbp_hard_reset(int idx, int *result)
{
	result_t rc;
	
	if ((rc = check_module_capability(JRPC_GAMI_REG_CAP_MBP, RMM_JSON_RESET)) != RESULT_OK)
		return rc;
	return cm_hard_reset(idx, result);
}

result_t libwrap_mbp_soft_reset(int idx, int *result)
{
	result_t rc;
	
	if ((rc = check_module_capability(JRPC_GAMI_REG_CAP_MBP, RMM_JSON_RESET)) != RESULT_OK)
		return rc;
	
	return -1;
}

result_t libwrap_mbp_uart_switch(int cm_idx, int tray_idx, int tgt_idx)
{
	result_t rc;

	if ((rc = check_module_capability(JRPC_GAMI_REG_CAP_MBP, RMM_JSON_SETUARTTARGET)) != RESULT_OK)
		return rc;

	return cm_uart_switch(cm_idx, tray_idx, tgt_idx);
}

result_t libwrap_drawer_hard_reset(int cm_idx, int drawer_idex, int *result)
{
	result_t rc;

	if ((rc = check_module_capability(JRPC_GAMI_REG_CAP_DRAWER, RMM_JSON_RESET)) != RESULT_OK)
		return rc;

	return drawer_hard_reset(cm_idx, drawer_idex, result);
}

result_t libwrap_rmm_factory_reset(int *result)
{
	return rmm_factory_reset(result);
}

result_t libwrap_drawer_soft_reset(int cm_idx, int drawer_idex, int *result)
{
	result_t rc;

	if ((rc = check_module_capability(JRPC_GAMI_REG_CAP_DRAWER, RMM_JSON_RESET)) != RESULT_OK)
		return rc;

	return -1;
}

result_t libwrap_get_log(int begin, int count, void* info)
{
    return -1;
}
static result_t get_drawer_coll(collections_t *drawer_coll, uint32 *number)
{
    struct node_info *subnode = NULL;
	struct node_info *parent = NULL;
	int cm_node_id = 0;
	int dzone_node_id = 0;
	int drawer_node_id = 0;
	int subnode_num  = 0;
	int i;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	int cm_lid = 0;
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 48] = {0};

	if (libutils_get_ip((char *)value) < 0) {
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);
		memcpy(value, "x", 1);
	}

	subnode = libdb_list_node_by_type(MC_TYPE_RMC, MC_TYPE_DRAWER, MC_TYPE_DRAWER, &subnode_num, memdb_filter, LOCK_ID_NULL);
	if (subnode_num == 0) {
		libdb_free_node(subnode);
		return RESULT_NO_NODE;
	}

	for (i = 0; i < subnode_num; i++) {
		parent = libdb_get_node_by_node_id(DB_RMM, subnode[i].parent, LOCK_ID_NULL);
		if (parent == NULL)
			return RESULT_NO_NODE;

		cm_lid = (uint32)get_db_info_num(DB_RMM, parent[0].parent, MBP_LOC_ID_STR);
		drawer_coll->id = (cm_lid - 1) * MAX_DRAWER_NUM +
			(uint32)get_db_info_num(DB_RMM, subnode[i].node_id, DRAWER_LOC_ID_STR);
		if (drawer_coll->id < 1) {
			drawer_coll->id = 0;
			continue;
		}

		get_db_info_string(DB_RMM, subnode[i].node_id, DRAWER_UUID_STR, drawer_coll->uuid, UUID_LEN);
		get_db_info_string(DB_RMM, subnode[i].node_id, DRAWER_NAME_STR, drawer_coll->name, RMM_NAME_LEN);
		get_db_info_string(DB_RMM, subnode[i].node_id, DRAWER_TMC_IP_STR, (uint8*)value, 20);
		memset(prefix, 0, MAX_URL);
		memset(new_link, 0, MAX_URL + 48);
		rmm_cfg_get_rest_prefix(prefix, MAX_URL);
		insert_str(new_link, "http://%s:%d/rack/drawers/%s/", 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/
		snprintf((char *)(drawer_coll->href), DESCRIPTION_LEN, new_link,
					(char *)value, rmm_cfg_get_port(RESTD_PORT), (char *)&(drawer_coll->uuid));
		drawer_coll++;
	}

	*number = subnode_num;
	libdb_free_node(subnode);

	return RESULT_OK;
}

result_t libwrap_get_drawer_coll(collections_t *drawer, uint32 *number)
{
	return get_drawer_coll(drawer, number);
}

result_t libwrap_pre_put_mbp(int idx, put_mbp_t *put_mbp_info)
{
	char output[128] = {0};
	int lid = idx;
	memdb_integer node_id = get_subnode_id_by_lid(lid, MC_TYPE_RMC, MC_TYPE_CM);
	int64 error_code = 0;
	
	if(node_id == 0) {
		return RESULT_ATTR_ERR;
	}

	error_code = libdb_attr_get_string(DB_RMM, node_id, MBP_DESCRIPT_STR, output, sizeof(output), LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}

	strncpy_safe((char*)put_mbp_info->descr, output, DESCRIPTION_LEN, DESCRIPTION_LEN - 1);
	error_code = 0;
	error_code = libdb_attr_get_string(DB_RMM, node_id, MBP_ASSET_TAG_STR, output, sizeof(output), LOCK_ID_NULL);
	if(error_code != 0) {
		return (int)error_code;
	}

	strncpy_safe((char*)put_mbp_info->asset_tag, output, REST_ASSET_TAG_LEN, REST_ASSET_TAG_LEN - 1);

	return RESULT_OK;
}

result_t libwrap_put_mbp(int idx, const put_mbp_t put_mbp_info)
{
	int lid = idx;
	int rc = 0;
	memdb_integer node_id = get_subnode_id_by_lid(lid, MC_TYPE_RMC, MC_TYPE_CM);

	if(node_id == 0) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, node_id, MBP_DESCRIPT_STR,
						  0x0, (char*)put_mbp_info.descr, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, node_id, MBP_ASSET_TAG_STR,
						  0x0, (char*)put_mbp_info.asset_tag, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	return RESULT_OK;
}

static int get_mbp_by_idx(int idx, mbp_member_t *mbp_member)
{
	memdb_integer node_id = 0;
	int ip_addr;
	int lid = idx;
	char buff[RMM_NAME_LEN] = {0};

	node_id = get_subnode_id_by_lid(lid, MC_TYPE_RMC, MC_TYPE_CM);
	if(node_id == 0) {
		return RESULT_ATTR_ERR;
	}

	mbp_member->be.id = 
		get_db_info_num(DB_RMM, node_id, MBP_LOC_ID_STR);
	get_db_info_string(DB_RMM, node_id, 
		MBP_UUID_STR, mbp_member->be.uuid, UUID_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_NAME_STR, mbp_member->be.name, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_DESCRIPT_STR, mbp_member->be.desc, DESCRIPTION_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_CREATE_DATE_STR, mbp_member->be.create_date, DATE_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_UPDATE_DATE_STR, mbp_member->be.update_date, DATE_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_MBPID_STR, mbp_member->MBPID, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_HW_ADDR_STR, mbp_member->MBPHWAddress, REST_RACK_STRING_LEN);
	libdb_attr_get_int(DB_RMM, node_id, MBP_IP_ADDR_STR, &ip_addr, LOCK_ID_NULL);
	snprintf((char *)mbp_member->MBPIPAddress, REST_RACK_STRING_LEN, "%d.%d.%d.%d", ip_addr&0xff,
			(ip_addr>>8)&0xff,
			(ip_addr>>16)&0xff,
			(ip_addr>>24)&0xff);
	get_db_info_string(DB_RMM, node_id,
		MBP_FW_STATE, mbp_member->fw_state, DATE_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_ASSET_TAG_STR, mbp_member->asset.asset_tag, REST_ASSET_TAG_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_SER_NUM_STR, mbp_member->asset.fru.serial_num, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_MANUFACT_STR, mbp_member->asset.fru.manufacture, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_MODEL_STR, mbp_member->asset.fru.model, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_PART_NUM_STR, mbp_member->asset.fru.part_num, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, 
		MBP_FW_VER_STR, mbp_member->asset.fru.fw_ver, REST_RACK_STRING_LEN);

	snprintf((char *)(mbp_member->av_action[0].action), RMM_NAME_LEN, "%s", RMM_JSON_RESET);
	snprintf((char *)(mbp_member->av_action[0].cap[0].property), RMM_NAME_LEN, "%s", RMM_JSON_RESET_TYPE);
	get_db_info_string(DB_RMM, node_id, 
		MBP_AV_RST_TYPE_1_STR, mbp_member->av_action[0].cap[0].av, RMM_NAME_LEN);
	snprintf((char *)(mbp_member->av_action[1].action), RMM_NAME_LEN, "%s", RMM_JSON_SETUARTTARGET);
	snprintf((char *)(mbp_member->av_action[1].cap[0].property), RMM_NAME_LEN, "%s", RMM_JSON_TARGET_INDEX);
	snprintf((char *)(mbp_member->av_action[2].action), RMM_NAME_LEN, "%s", RMM_JSON_UPDATE);
	snprintf((char *)(mbp_member->av_action[2].cap[0].property), RMM_NAME_LEN, "%s", RMM_JSON_IMAGE);

#ifdef ODATA_ADD_ATTR
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, UNITS, (uint8 *)buff, RMM_NAME_LEN);
	mbp_member->loc.units = (uint32)atoi((const char *)buff);
#endif		
    memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, MBP_X_LOC_STR, (uint8 *)buff, RMM_NAME_LEN);
	mbp_member->loc.x_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, MBP_U_LOC_STR, (uint8 *)buff, RMM_NAME_LEN);
	mbp_member->loc.u_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, MBP_U_HEIGHT_STR, (uint8 *)buff, RMM_NAME_LEN);
	mbp_member->loc.u_height = (uint32)atoi((const char *)buff);
	return 0;
}

result_t libwrap_get_mbp_by_idx(int idx, mbp_member_t *mbp_member)
{
	return get_mbp_by_idx(idx, mbp_member);
}

static result_t get_mbp_coll(collections_t *mbp, uint32 *number)
{
    struct node_info *subnode = NULL;
	int subnode_num = 0;
	int i;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 48] = {0};

	if (libutils_get_ip((char *)value) < 0) {
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);
		memcpy(value, "x", 1);
	}

	subnode = libdb_list_node_by_type(MC_TYPE_RMC, MC_TYPE_CM, MC_TYPE_CM, &subnode_num, memdb_filter, LOCK_ID_NULL);
	if (subnode_num == 0) {
		libdb_free_node(subnode);
		return RESULT_NO_NODE;
	}

	for (i = 0; i < subnode_num; i++) {
		mbp->id = 
			(uint32)get_db_info_num(DB_RMM, subnode[i].node_id, MBP_LOC_ID_STR);

		if (mbp->id < 1) {
			mbp->id = 0;
			continue;
		}

		get_db_info_string(DB_RMM, subnode[i].node_id, MBP_UUID_STR, mbp->uuid, UUID_LEN);
		get_db_info_string(DB_RMM, subnode[i].node_id, MBP_NAME_STR, mbp->name, RMM_NAME_LEN);
		memset(prefix, 0, MAX_URL);
		memset(new_link, 0, MAX_URL + 48);
		rmm_cfg_get_rest_prefix(prefix, MAX_URL);
		insert_str(new_link, "http://%s:%d/rack/mbps/%d/", 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/
		snprintf((char *)(mbp->href), DESCRIPTION_LEN, new_link,
					(char *)value,  rmm_cfg_get_port(RESTD_PORT), mbp->id);
		mbp++;
	}

	*number = subnode_num;
	libdb_free_node(subnode);

	return RESULT_OK;
}

result_t libwrap_get_mbp_coll(collections_t* mbp, uint32 *number)
{
	return get_mbp_coll(mbp, number);
}

static result_t get_psu_collection(uint32 pzone_idx, collections_t* psu, uint32 *number)
{
    memdb_integer cm_node_id,pzone_node_id,psu_node_id;
	int i, psu_num = 0;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 48] = {0};
	int cm_lid = get_cm_lid(MC_TYPE_PZONE, pzone_idx);
	int pzone_lid = get_zone_lid(MC_TYPE_PZONE, pzone_idx);

	if (libutils_get_ip((char *)value) < 0) {
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);
		memcpy(value, "x", 1);
	}

	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	pzone_node_id = get_subnode_id_by_lid(pzone_lid, cm_node_id, MC_TYPE_PZONE);

	for(i = 0; i < MAX_PSU_NUM; i++) {
		psu_node_id = get_subnode_id_by_lid(i + 1, pzone_node_id, MC_TYPE_PSU);
		if(psu_node_id != INVALID_NODE_ID) {
			psu->id = (i + 1);
			get_db_info_string(DB_RMM, psu_node_id, PSU_UUID_STR, psu->uuid, UUID_LEN);
			get_db_info_string(DB_RMM, psu_node_id, PSU_NAME_STR, psu->name, RMM_NAME_LEN);
			memset(prefix, 0, MAX_URL);
			memset(new_link, 0, MAX_URL + 48);
			rmm_cfg_get_rest_prefix(prefix, MAX_URL);
			insert_str(new_link, "http://%s:%d/rack/powerzones/%d/psus/%d", 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/

			snprintf((char *)(psu->href), DESCRIPTION_LEN, new_link,
					(char *)value, rmm_cfg_get_port(RESTD_PORT), pzone_idx, psu->id);
			psu++;
			psu_num++;
		}
	}
	*number = psu_num;
	return RESULT_OK;
}

result_t libwrap_get_pzone_psu_coll(uint32 pzone_idx, collections_t* psu/*[PSU_PER_POWERZONE]*/, uint32 *number)

{
	return get_psu_collection(pzone_idx, psu, number);
}

static int get_pzone_by_idx(uint32 pzone_idx, struct pzone_member *pzone_member)
{
	memdb_integer cm_node_id = 0;
	memdb_integer pzone_node_id = 0;
	char buff[RMM_NAME_LEN] = {0};
	int cm_lid = get_cm_lid(MC_TYPE_PZONE, pzone_idx);
	int pzone_lid = get_zone_lid(MC_TYPE_TZONE, pzone_idx);

	cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	pzone_node_id = get_subnode_id_by_lid(pzone_lid,cm_node_id,MC_TYPE_PZONE);

	pzone_member->be.id = 
		pzone_idx;//get_db_info_num(DB_RMM, pzone_node_id, PZONE_INDEX_STR);
	get_db_info_string(DB_RMM, pzone_node_id, 
		PZONE_UUID_STR, pzone_member->be.uuid, UUID_LEN);
	get_db_info_string(DB_RMM, pzone_node_id, 
		PZONE_NAME_STR, pzone_member->be.name, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, pzone_node_id, 
		PZONE_DESCRIPT_STR, pzone_member->be.desc, DESCRIPTION_LEN);
	get_db_info_string(DB_RMM, pzone_node_id, 
		PZONE_CREATE_DATE_STR, pzone_member->be.create_date, DATE_LEN);
	get_db_info_string(DB_RMM, pzone_node_id, 
		PZONE_UPDATE_DATE_STR, pzone_member->be.update_date, DATE_LEN);
	pzone_member->tot_power_consumption = 
		(uint32)get_db_info_num(DB_RMM, pzone_node_id, PZONE_TT_PWR_CONSUM_STR);
	pzone_member->tot_power_cap =
		(uint32)get_db_info_num(DB_RMM, pzone_node_id, PZONE_TT_PWR_CAP_STR);
	pzone_member->tot_power_production = 
		(uint32)get_db_info_num(DB_RMM, pzone_node_id, PZONE_TT_PWR_PROD_STR);

	#ifdef ODATA_ADD_ATTR
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, pzone_node_id, UNITS, (uint8 *)buff, RMM_NAME_LEN);
	pzone_member->power_zone_loc.units = (uint32)atoi((const char *)buff);
	#endif
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, pzone_node_id, X_LOC, (uint8 *)buff, RMM_NAME_LEN);
	pzone_member->power_zone_loc.x_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, pzone_node_id, U_LOC, (uint8 *)buff, RMM_NAME_LEN);
	pzone_member->power_zone_loc.u_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, pzone_node_id, U_HEIGHT, (uint8 *)buff, RMM_NAME_LEN);
	pzone_member->power_zone_loc.u_height = (uint32)atoi((const char *)buff);

	memset(pzone_member->presence, 0, REST_PRESENCE_LEN);
	get_present_string(pzone_node_id, pzone_member->presence, REST_PRESENCE_LEN);

	return 0;
}

result_t libwrap_get_pzone_by_idx(uint32 pzone_idx, struct pzone_member *pzone_member)
{
	return get_pzone_by_idx(pzone_idx, pzone_member);
}

result_t libwrap_pre_put_pzone_by_idx(int pzone_idx, put_pzone_t *put_pzone_info)
{
	int cm_lid = get_cm_lid(MC_TYPE_PZONE, pzone_idx);
	int pzone_lid = get_zone_lid(MC_TYPE_PZONE, pzone_idx);
	int64 error_code = 0;
	char output[128] = {0};
	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	memdb_integer pzone_node_id = get_subnode_id_by_lid(pzone_lid, cm_node_id, MC_TYPE_PZONE);

	error_code = libdb_attr_get_string(DB_RMM, pzone_node_id, PZONE_DESCRIPT_STR, output, sizeof(output), LOCK_ID_NULL);
	if(error_code != 0) {
		return RESULT_ATTR_ERR;
	}
	strncpy_safe((char*)put_pzone_info->descr, output, DESCRIPTION_LEN, DESCRIPTION_LEN - 1);

	return RESULT_OK;
}

result_t libwrap_put_pzone_by_idx(int pzone_idx, const put_pzone_t put_pzone_info)
{
	int cm_lid = get_cm_lid(MC_TYPE_PZONE, pzone_idx);
	int pzone_lid = get_zone_lid(MC_TYPE_PZONE, pzone_idx);
	memdb_integer cm_node_id = get_subnode_id_by_lid(cm_lid, 0, MC_TYPE_CM);
	memdb_integer pzone_node_id = get_subnode_id_by_lid(pzone_lid, cm_node_id, MC_TYPE_PZONE);
	int rc = 0;

	rc = libdb_attr_set_string(DB_RMM, pzone_node_id, PZONE_DESCRIPT_STR,
						  0x0, (char*)put_pzone_info.descr, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	return RESULT_OK;
}

static result_t get_pzone_collection(collections_t *pzone/*[POWERZONE_PER_RACK]*/, uint32 *number)
{
    struct node_info *subnode = NULL;
	int subnode_num = 0;
	int i;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 48] = {0};
	int cm_lid = 0;
	int max_pz_num = 0;
	int pz_lid = 0;

	if (libutils_get_ip((char *)value) < 0) {
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);
		memcpy(value, "x", 1);
	}

	subnode = libdb_list_node_by_type(MC_TYPE_RMC, MC_TYPE_PZONE, MC_TYPE_PZONE, &subnode_num, memdb_filter, LOCK_ID_NULL);
	if (subnode_num == 0) {
		libdb_free_node(subnode);
		return RESULT_NO_NODE;
	}

	for (i = 0; i < subnode_num; i++) {
		cm_lid = get_db_info_num(DB_RMM, subnode[i].parent, MBP_LOC_ID_STR);
		max_pz_num = MAX_PZONE_NUM;
		pz_lid = get_db_info_num(DB_RMM, subnode[i].node_id, PZONE_LOC_ID_STR);
		pzone->id = pz_lid + (cm_lid - 1) * max_pz_num;
		if (pzone->id < 1) {
			pzone->id = 0;
			continue;
		}

		get_db_info_string(DB_RMM, subnode[i].node_id, PZONE_UUID_STR, pzone->uuid, UUID_LEN);
		get_db_info_string(DB_RMM, subnode[i].node_id, PZONE_NAME_STR, pzone->name, RMM_NAME_LEN);
		memset(prefix, 0, MAX_URL);
		memset(new_link, 0, MAX_URL + 48);
		rmm_cfg_get_rest_prefix(prefix, MAX_URL);
		insert_str(new_link, "http://%s:%d/rack/powerzones/%d/", 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/

		snprintf((char *)(pzone->href), DESCRIPTION_LEN, new_link,
					(char *)value, rmm_cfg_get_port(RESTD_PORT), pzone->id);
		pzone++;
	}

	*number = subnode_num;
	libdb_free_node(subnode);

	return RESULT_OK;
}

result_t libwrap_get_pzone_coll(collections_t* pzone/*[POWERZONE_PER_RACK]*/, uint32 *number)
{
	return get_pzone_collection(pzone, number);
}

static result_t add_dest_to_info(listener_dest_info_t* header, char *tmp_dest, int type_id)
{
	int ret = 0;
	listener_dest_info_t *plast = NULL;
	//Header is not set.
	if(header->evt_index == 0) {
		goto end;
	}

	while (header) {
		ret = strcmp(header->dest, tmp_dest);
		if (ret == 0)
			break;
		plast = header;
		header = plast->pnext;
        plast->pnext = NULL;
	}

	if (header == NULL && plast != NULL) {
		plast->pnext = (struct listener_dest_info*)malloc(sizeof(struct listener_dest_info));
		if (plast->pnext == NULL)
			return RESULT_MALLOC_ERR;
		memset(plast->pnext, 0, sizeof(struct listener_dest_info));
		header =  plast->pnext;
	}
	if (header == NULL)
		return RESULT_OTHER_ERR;
	
end:
	strncpy_safe(header->dest, tmp_dest, sizeof(header->dest), sizeof(header->dest) - 1);
	header->event_types[header->evt_index] = type_id;
	header->evt_index = header->evt_index + 1;
	return RESULT_OK;
		
}

static result_t get_listeners_by_nodeid(int type, int zone_id,  int index, listener_dest_info_t* header)
{
	struct node_info *subnode = NULL;
	int subnode_num = 0;
	int i = 0;
	struct node_info *listener_subnode = NULL;
	int listener_subnode_num = 0;
	int j = 0;
	int is_listen_validate_flag = 1;
	int64 error_code = 0;
	int mask = 0;
	char tmp_dest[256] = {0};
	int cmp_mask;

	subnode = libdb_list_node_by_type(DB_RMM, MC_REDFISH_EVENT, MC_REDFISH_EVENT, &subnode_num, NULL, LOCK_ID_NULL);
	if (subnode_num != 1) {
		libdb_free_node(subnode);
		return RESULT_OK;
	}

	subnode = libdb_list_subnode_by_type(DB_RMM, subnode[0].node_id, type, &subnode_num, NULL, LOCK_ID_NULL);
	if (subnode_num != 1)
		return RESULT_OK;
	subnode = libdb_list_subnode(DB_RMM, subnode[0].node_id, &subnode_num, NULL, LOCK_ID_NULL);
	for(i = 0; i < subnode_num; i++) {
		listener_subnode = libdb_list_subnode_by_type(DB_RMM, subnode[i].node_id, MC_REDFISH_LISTENER, &listener_subnode_num, NULL, LOCK_ID_NULL);
		for(j = 0; j < listener_subnode_num; j++) {
			cmp_mask = DEFAULT_MASK;
			if (index != INVAILD_IDX) {
				cmp_mask = 1<< index;
			} else if(zone_id == 1 || zone_id == 2) {
				if (zone_id ==1) 
					cmp_mask= 0x3F;
				else 
					cmp_mask= 0xFC0;
			} 

			if (cmp_mask != DEFAULT_MASK) {
				error_code = 0;
				error_code = libdb_attr_get_int(DB_RMM, listener_subnode[j].node_id, RF_EVENT_LISTENER_INDEXES_STR, &mask, LOCK_ID_NULL);
				if(error_code != 0)
					continue;
				if((mask & cmp_mask) == 0)
					is_listen_validate_flag = 0;
				else
					is_listen_validate_flag = 1;
			} else {
				is_listen_validate_flag = 1;
			}

			if (is_listen_validate_flag == 1) {
				error_code = 0;
				error_code = libdb_attr_get_string(DB_RMM, listener_subnode[j].node_id, RF_EVENT_LISTENER_DEST_STR, tmp_dest, 256, LOCK_ID_NULL);
				if(error_code == 0) {
					add_dest_to_info(header, tmp_dest, subnode[i].type);
				}
			}
		}
			
	}
	
	return RESULT_OK;
}


result_t libwrap_get_listeners_by_type(int type, int pindex/*only for zone id*/, int index, listener_dest_info_t *header) 
{
	return get_listeners_by_nodeid(type, pindex, index, header);
}

#if 0
static void* wrap_ipmi_thread(void *argv)
{
	struct wrap_ipmi* ipmi;

	for (;;) {
		pthread_mutex_lock(&wrap_mutex[wrap_ipmi_msg.curr_process]);

		list_for_each_entry(ipmi, &wrap_ipmi_msg.wrap_head, list) {
			libjipmi_rmcp_cmd(ipmi->host, IPMI_RMCP_PORT, &ipmi->msg, ipmi->cb_arg, &ipmi->cb_arg, JIPMI_NON_SYNC);
			list_del(&ipmi->list);
		}

		while (1) {
			pthread_cond_wait(&wrap_cond[wrap_ipmi_msg.curr_process], &wrap_mutex[wrap_ipmi_msg.curr_process]);
		}
		
		pthread_mutex_unlock(&wrap_mutex[wrap_ipmi_msg.curr_process]);

	}
	return NULL;

}
#endif

result_t libwrap_get_psu_node_id_by_idx(int pzone_idx, int psu_idx, memdb_integer *node_id)
{
	return get_psu_node_id_by_idx(pzone_idx, psu_idx, node_id);
}

result_t libwrap_pre_put_psu(uint32 pzone_idx, uint32 psu_idx, char *descr, char *asset_tag)
{
	memdb_integer node_id = 0;
	char output[128] = {0};

	libwrap_get_psu_node_id_by_idx((int)pzone_idx, (int)psu_idx, &node_id);
	if(0 == node_id)
		return RESULT_NO_NODE;

	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, node_id, PSU_DESCRIPT_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe(descr, output, DESCRIPTION_LEN, DESCRIPTION_LEN - 1);
	memset(output, 0, sizeof(output));
	libdb_attr_get_string(DB_RMM, node_id, PSU_ASSET_TAG_STR, output, sizeof(output), LOCK_ID_NULL);
	strncpy_safe(asset_tag, output, REST_ASSET_TAG_LEN, REST_ASSET_TAG_LEN - 1);

	return RESULT_OK;
}

result_t libwrap_put_psu(uint32 pzone_idx, uint32 psu_idx, char *descr, char *asset_tag)
{
	memdb_integer node_id = 0;
	int rc = 0;

	libwrap_get_psu_node_id_by_idx((int)pzone_idx, (int)psu_idx, &node_id);
	if(0 == node_id)
		return 0;

	rc = libdb_attr_set_string(DB_RMM, node_id, PSU_DESCRIPT_STR,
						  0x0, descr, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, node_id, PSU_ASSET_TAG_STR,
							  0x0, asset_tag, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	return RESULT_OK;
}

static int get_psu_availible_action(unsigned char db_name, avail_action_t* action, memdb_integer node_id)
{
	char buff[REST_RACK_STRING_LEN];

	strncpy_safe((char*)action->action, RMM_JSON_REQ_STATE_CHANGE, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[0].property, RMM_JSON_ENABLE_STATE, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	get_db_info_string(db_name,node_id, PSU_AV_REQ_ST_1_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[0].av, buff, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[1].property, RMM_JSON_ENABLE_STATE, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	get_db_info_string(db_name,node_id, PSU_AV_REQ_ST_2_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[1].av, buff, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	strncpy_safe((char*)action->cap[2].property, RMM_JSON_ENABLE_STATE, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	get_db_info_string(db_name,node_id, PSU_AV_REQ_ST_3_STR, (uint8*)buff, RMM_NAME_LEN);
	strncpy_safe((char*)action->cap[2].av, buff, RMM_NAME_LEN, RMM_NAME_LEN - 1);
	return 0;
}

static int get_psu_by_idx(const uint32 pzone_idx, const uint32 psu_idx,  struct psu_member * psu_member)
{
	memdb_integer node_id = 0;
#ifdef ODATA_ADD_ATTR
	char buff[RMM_NAME_LEN] = {0};
#endif

	libwrap_get_psu_node_id_by_idx((int)pzone_idx, (int)psu_idx, &node_id);
	if(0 == node_id)
		return 0;

	psu_member->be.id = 
		get_db_info_num(DB_RMM, node_id, PSU_LOC_ID_STR);
	get_db_info_string(DB_RMM, node_id, 
		PSU_UUID_STR, psu_member->be.uuid, UUID_LEN);
	get_db_info_string(DB_RMM, node_id, 
		PSU_NAME_STR, psu_member->be.name, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, 
		PSU_DESCRIPT_STR, psu_member->be.desc, DESCRIPTION_LEN);
	get_db_info_string(DB_RMM, node_id, 
		PSU_CREATE_DATE_STR, psu_member->be.create_date, DATE_LEN);
	get_db_info_string(DB_RMM, node_id, 
		PSU_UPDATE_DATE_STR, psu_member->be.update_date, DATE_LEN);
	psu_member->enable_state = 
		(int32)get_db_info_num(DB_RMM, node_id, PSU_ENABLE_ST_STR);
	snprintf((char *)&(psu_member->redundancy), REST_RACK_STRING_LEN, "%d",
		(int32)get_db_info_num(DB_RMM, node_id, PSU_REDUND_SET_STR));
	psu_member->health_state = 
		(int32)get_db_info_num(DB_RMM, node_id, PSU_HEALTH_ST_STR);

	if (psu_member->enable_state == PSU_STATE_ENABLED) {
		psu_member->power_in = 
			(int32)get_db_info_num(DB_RMM, node_id, PSU_TT_PWR_IN_STR);
		psu_member->power_cap = 
			(int32)get_db_info_num(DB_RMM, node_id, PSU_TT_PWR_CAP_STR);
		psu_member->power_out = 
			(int32)get_db_info_num(DB_RMM, node_id, PSU_TT_CURRENT_OUT_STR);
	} else {
		psu_member->power_in = 0;
		psu_member->power_cap = 0;
		psu_member->power_out = 0;
	}

	get_db_info_string(DB_RMM, node_id, PSU_SER_NUM_STR, psu_member->asset.fru.serial_num, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, PSU_MANUFACT_STR, psu_member->asset.fru.manufacture, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, PSU_MODEL_STR, psu_member->asset.fru.model, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, PSU_PART_NUM_STR, psu_member->asset.fru.part_num, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, PSU_FW_VER_STR, psu_member->asset.fru.fw_ver, REST_RACK_STRING_LEN);
	get_db_info_string(DB_RMM, node_id, PSU_ASSET_TAG_STR, psu_member->asset.asset_tag, REST_ASSET_TAG_LEN);

	get_psu_availible_action(DB_RMM, &(psu_member->av_action), node_id);

#ifdef ODATA_ADD_ATTR
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, UNITS, (uint8 *)buff, RMM_NAME_LEN);
	psu_member->psu_loc.units = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, X_LOC, (uint8 *)buff, RMM_NAME_LEN);
	psu_member->psu_loc.x_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, U_LOC, (uint8 *)buff, RMM_NAME_LEN);
	psu_member->psu_loc.u_location = (uint32)atoi((const char *)buff);
	memset(buff, 0, RMM_NAME_LEN);
	get_db_info_string(DB_RMM, node_id, U_HEIGHT, (uint8 *)buff, RMM_NAME_LEN);
	psu_member->psu_loc.u_height = (uint32)atoi((const char *)buff);
#endif

	return 0;
}

result_t libwrap_get_pzone_psu_by_idx(const uint32 pzone_idx, const uint32 psu_idx,  struct psu_member * psu_member)
{
	return get_psu_by_idx(pzone_idx, psu_idx, psu_member);
}

memdb_integer get_subnode_id_by_lid(unsigned int lid, memdb_integer parent, int type)
{
	int i, loc_id = 0;
	struct node_info *subnode = NULL;
	int subnode_num = 0;

	subnode = libdb_list_subnode_by_type(DB_RMM, parent, type, &subnode_num, NULL,LOCK_ID_NULL);

	for (i = 0; i < subnode_num; i++) {
		loc_id = get_db_info_num(DB_RMM, subnode[i].node_id, WRAP_LOC_ID_STR);
		if (loc_id == lid) {
			return subnode[i].node_id;
		}
	}

	return 0;
}


result_t libwrap_post_pzone_psu_by_idx(uint32 pzone_idx, uint32 psu_idx, int state)
{
	result_t rc;

	if ((rc = check_module_capability(JRPC_GAMI_REG_CAP_PSU, RMM_JSON_REQ_STATE_CHANGE)) != RESULT_OK)
		return rc;

	return am_post_psu_by_node_id(pzone_idx, psu_idx, state);
}

/**
 * @brief: check if the tftp service has been started, assume the port
 *         for the tftp is "69" which is for tftp by default.
 */
result_t libwrap_check_tftp_service(void)
{
	int status = 0;

	status = system("netstat  -nla | grep \":69\\>\" >/dev/null 2>/dev/null");
	if (status < 0) {
		return -1;
	}

	if (WIFEXITED(status)) {
		if (WEXITSTATUS(status) != 0) {
			return -1;
		}
	} else {
		return -1;
	}	
	return RESULT_OK;
}

static int var_firmware_update_ok = 1;

void libwrap_set_firmware_update_status(int status)
{
	var_firmware_update_ok = status;

	return;
}

int libwrap_get_firmware_update_status(void)
{
	return var_firmware_update_ok;
}


#if 0
struct _fm_update_info {
	unsigned int host;
	char dev[32];
	char file[128];
};



/**
 * Parity: N/n - None; O/o - Odd; E/e - Even.
 * Flow_control: 0 - None; 1 - XON/XOFF; 2 - RTS/CTS; 3 - DSR/DTR.
 * By default, baudrate = 115200, databits = 8, stopbits = 1, parity = 'N', flow_control = 0
 */
#define PARITY_NONE			'N'
#define PARITY_ODD			'O'
#define PARITY_EVEN			'E'

#define FLOW_CONTROL_NONE		'0'
#define FLOW_CONTROL_XON_XOFF	'1'
#define FLOW_CONTROL_RTS_CTS	'2'
#define FLOW_CONTROL_DSR_DTR	'3'

#define DATABITS_7			'7'
#define DATABITS_8			'8'

#define STOPBITS_1			'1'
#define STOPBITS_2			'2'

#define DEFAULT_BAUDRATE		115200
#define DEFAULT_DATABITS		DATABITS_8
#define DEFAULT_PARITY			PARITY_NONE
#define DEFAULT_STOPBITS		STOPBITS_1
#define DEFAULT_FLOW_CONTROL	FLOW_CONTROL_NONE

static struct _rates {
	int speed;
	int name;
} rates[] = {
	{B300,    300},
	{B1200,   1200},
	{B2400,   2400},
	{B4800,   4800},
	{B9600,   9600},
	{B19200,  19200},
	{B38400,  38400},
	{B115200, 115200},
	{B230400, 230400},
	{B460800, 460800}
};

/**
 * @brief: Do not block the serial
 */
static int serial_open(char *dev)
{
	int fd = -1;
	int val = 0;

	/* Here, we make it work with BLOCK flag, sometimes we need to
	 * read several times to get the complete message */
	fd = open(dev, O_RDWR | O_NOCTTY, 0);
	if (fd < 0) {
		perror("serial_open");
		return -1;
	}

	val = fcntl(fd, F_GETFL, 0);
	if (fcntl(fd, F_SETFL, val | O_NONBLOCK) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

/**
 * @brief: This function is used to set the attributes for the serial port.
 */
static int serial_set_attr(int fd, unsigned int rate, unsigned char databits,
						   unsigned char stopbits, unsigned char parity,
						   unsigned char flow_control)
{
	struct termios ti = {0};
	int i = 0;

	tcgetattr(fd, &ti);

	for (i = 0; i < sizeof(rates) / sizeof(struct _rates); i++) {
		if (rates[i].name == rate) {
			cfsetispeed(&ti, rates[i].speed);
			cfsetospeed(&ti, rates[i].speed);
			break;
		}
	}

	ti.c_cflag &= ~CSIZE;
	switch (databits) {
	case DATABITS_7:
		ti.c_cflag |= CS7;
		break;
	case DATABITS_8:
		ti.c_cflag |= CS8;
		break;
	default:
		return -1;
	}

	switch (parity) {
	case 'n':
	case 'N':
		ti.c_cflag &= ~PARENB;
		ti.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O':
		ti.c_cflag |= (PARODD | PARENB);
		ti.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E':
		ti.c_cflag |= PARENB;
		ti.c_cflag &= ~PARODD;
		ti.c_iflag |= INPCK;
		break;
	default:
		return -1;
	}

	switch (stopbits) {
	case STOPBITS_1:
		ti.c_cflag &= ~CSTOPB;
		break;
	case STOPBITS_2:
		ti.c_cflag |= CSTOPB;
		break;
	default:
		return -1;
	}

	switch (flow_control) {
	case FLOW_CONTROL_NONE:
		ti.c_cflag &= ~CRTSCTS;
		ti.c_iflag &= ~(IXON | IXOFF | IXANY);
		break;
	case FLOW_CONTROL_RTS_CTS:
		ti.c_cflag |= CRTSCTS;
		ti.c_iflag &= ~(IXON | IXOFF | IXANY);
		break;
	case FLOW_CONTROL_XON_XOFF:
		ti.c_cflag &= ~CRTSCTS;
		ti.c_iflag |= (IXON | IXOFF | IXANY);
		break;
	default:
		break;
	}

	ti.c_iflag &= ~(IGNBRK | IGNCR | INLCR | ICRNL | INPCK | ISTRIP);
	ti.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	ti.c_oflag &= ~OPOST;

	tcsetattr(fd, TCSAFLUSH, &ti);

	return 0;
} /* End of serial_set_attr(...) */

static int serial_close(int fd)
{
	return close(fd);
}

/**
*	Work around after upgrade cm, need reboot the CM before CM v0.9
**/
static int upgraded_serial_inf_close_rsp_cb(int result, unsigned char *rsp, int rsp_len,
			void *cb_data)
{
	int i;
	struct _fm_update_info *mbp_update = (struct _fm_update_info *)cb_data;
	struct stat sb;
	int fd = -1;
	char file[128] = {0};
	char prefix[64] = {0};
	int ret = -1;
	char *search = NULL;

	printf("SERIAL CLOSE RSP RCV'D: ");
	for (i = 0; i < rsp_len; i++)
		printf("%02X ", rsp[i]);
	printf("\n");

	for (i = 0; i < 45; i++) {
		if (stat(mbp_update->dev, &sb) != 0) {
			if (errno == ENOENT) {
				libwrap_set_firmware_update_status(1);
				return 0;
			}
		}

		sleep(1);
	}

	search = strstr(mbp_update->dev, "IPMI");
	if (NULL == search) {
		libwrap_set_firmware_update_status(1);
		return -1;
	}

	strncpy_safe(prefix, mbp_update->dev, search - mbp_update->dev);

	snprintf(file, sizeof(file), "%sConsole", prefix);

	fd = serial_open(file);
	if (fd < 0) {
		printf("serial_open failed: %s\n", file);
		libwrap_set_firmware_update_status(1);
		return -1;
	}

	unsigned int rate = DEFAULT_BAUDRATE;
	unsigned char databits = DEFAULT_DATABITS;
	unsigned char stopbits = DEFAULT_STOPBITS;
	unsigned char parity = DEFAULT_PARITY;
	unsigned char flow_control = DEFAULT_FLOW_CONTROL;

	ret = serial_set_attr(fd, rate, databits, stopbits, parity, flow_control);
	if (ret < 0) {
		printf("serial_set_attr failed\n");
		serial_close(fd);
		libwrap_set_firmware_update_status(1);
		return -1;
	}

	ret = write(fd, "reboot\r", 7);

	serial_close(fd);
	libwrap_set_firmware_update_status(1);

	return 0;
}


static int ipmi_upgrade_close_rsp_handler(int result, unsigned char *rsp, int rsp_len,
										  void *cb_data)
{
	struct _fm_update_info *mbp_update = (struct _fm_update_info *)cb_data;

	if (rsp[0] == IPMI_CC_OK) {
		unlink(mbp_update->file);

		struct jipmi_serial_msg req = {};


		memcpy(req.msg_base.data, mbp_update->dev, strlen(mbp_update->dev));


		FILL_INT(req.msg_base.netfn,		OEM_IPMI_NETFN);
		FILL_INT(req.msg_base.cmd,		OEM_SERIAL_CLOSE_CMD);
		FILL_INT(req.msg_base.data_len,	strlen(mbp_update->dev));
		req.serial_flag = IPMI_SERIAL_CLOSE_DEV;

		libjipmi_serial_cmd(&req, upgraded_serial_inf_close_rsp_cb, mbp_update, JIPMI_NON_SYNC);

		return 0;
	} else {
		unlink(mbp_update->file);
		free(mbp_update);
		
		libwrap_set_firmware_update_status(1);
		return -1;
	}

	return 0;
}

static int ipmi_upgrade_start_rsp_handler(int result, unsigned char *rsp, int rsp_len,
										  void *cb_data)
{
	struct _fm_update_info *mbp_update = (struct _fm_update_info *)cb_data;

	if (rsp[0] == IPMI_CC_OK) {
		libjipmi_rmcp_close_session(mbp_update->host, IPMI_RMCP_PORT,
								   ipmi_upgrade_close_rsp_handler, mbp_update, JIPMI_NON_SYNC);

		return 0;
	} else {
		unlink(mbp_update->file);
		free(mbp_update);

		libwrap_set_firmware_update_status(1);
		return -1;
	}

	return 0;
}

static int ipmi_upgrade_file_rsp_handler(int result, unsigned char *rsp, int rsp_len,
										 void *cb_data)
{
	struct _fm_update_info *mbp_update = (struct _fm_update_info *)cb_data;

	if (rsp[0] == IPMI_CC_OK) {
		struct jipmi_msg req = {};
		/* Step3: Start Upgrading */
		memset((char *)&req, 0, sizeof(req));


		FILL_INT(req.netfn,		UPDATE_FIRMWARE_NETFN);
		FILL_INT(req.cmd,		UPDATE_FM_START_CMD);
		FILL_INT(req.data_len,	0x00);

		libjipmi_rmcp_cmd(mbp_update->host, IPMI_RMCP_PORT, &req,
						 ipmi_upgrade_start_rsp_handler, mbp_update, JIPMI_NON_SYNC);
		return 0;
	} else {
		unlink(mbp_update->file);
		free(mbp_update);

		libwrap_set_firmware_update_status(1);
		return -1;
	}

	return 0;
}

static int ipmi_upgrade_ip_rsp_handler(int result, unsigned char *rsp, int rsp_len,
									void *cb_data)
{
	struct _fm_update_info *mbp_update = (struct _fm_update_info *)cb_data;

	if (rsp[0] == IPMI_CC_OK) {
		struct jipmi_msg req = {};
		int len = 0;
		int data_len = 0;
		int i = 0;

		/* Step2: Send File Name */
		len = strlen(mbp_update->file);
		memset((char *)&req, 0, sizeof(req));

		req.data[0] = 0x02;
		data_len = 1;

		FILL_INT(req.netfn,		UPDATE_FIRMWARE_NETFN);
		FILL_INT(req.cmd,		UPDATE_FM_SEND_FILE_CMD);
		

		for (i = 0; i < len; i++) {
			req.data[i+1] = mbp_update->file[i];
			data_len++;
		}

		req.data[len+1] = 0x00;
		data_len++;

		FILL_INT(req.data_len,	data_len);

		libjipmi_rmcp_cmd(mbp_update->host, IPMI_RMCP_PORT, &req,
						 ipmi_upgrade_file_rsp_handler, mbp_update, JIPMI_NON_SYNC);

		return 0;
	} else {
		unlink(mbp_update->file);
		free(mbp_update);

		libwrap_set_firmware_update_status(1);
		return -1;
	}

	return 0;
}

#endif


int get_asset_module_port(int *port)
{
	memdb_integer reg_node_id = 0;
	int self_ip = 0;
	int self_port = 0;
	int i = 0;
	char cap_name[GAMI_MAX_NAME_LEN] = {0};
	struct node_info *subnode = NULL;
	int node_num = 0;

	subnode = libdb_list_subnode_by_type(DB_RMM, MC_TYPE_RMC, MC_TYPE_REG, &node_num, NULL, LOCK_ID_NULL);
	if (node_num != 1) {
		printf("invalid registry node num in memdb\n");
		return -1;
	}

	reg_node_id = subnode[0].node_id;
	subnode = libdb_list_subnode_by_type(DB_RMM, reg_node_id, MC_TYPE_REG_MODULE, &node_num, NULL, LOCK_ID_NULL);
	if (node_num == 0) {
		printf("no asset module registry info found\n");
		return -1;
	}

	printf("reg_node is is %llu, sub node num is %d\n", reg_node_id, node_num);

	for (i = 0; i < node_num; i++) {
		libdb_attr_get_string(DB_RMM, subnode[i].node_id, JRPC_GAMI_REG_NAME, cap_name, GAMI_MAX_NAME_LEN, LOCK_ID_NULL);
		printf("subnode cap name is %s\n", cap_name);
		if (strcmp(cap_name, PROC_ASSET_MODULE) == 0) {
			return libdb_attr_get_int(DB_RMM, subnode[i].node_id, JRPC_GAMI_REG_PORT, port, LOCK_ID_NULL);
		}
	}

	return -1;
}

result_t libwrap_connect_asset_module()
{
	int port = 0;

	if ( 0 != get_asset_module_port(&port) || 0 == port || 0 != connect_asset_module(port))
		return RESULT_OTHER_ERR;

	return RESULT_OK;
}

//void fill_array_elem(json_t *array, char *attr_name, unsigned char *value_str, int *value_num, int *value_bool)



result_t libwrap_check_update_capability()
{
	return check_module_capability(JRPC_GAMI_REG_CAP_MBP, JRPC_GAM_CAP_UPDATE);
}


/**
 * @brief: upgrade the CM firmware via IPMI.
 *         Steps:
 *         1. send the tftp server ip to the CM
 *         2. send the file name for upgrading to the CM
 *         3. send the command to start to do the upgradation
 *         4. check the serial port if disappeared for a while,
 *            if yes, the cm reboot itself and the update finished.
 *            or need to send the command "reboot" via serial console
 */
result_t libwrap_update_mbp_firmware(int idx, char *dev, char *file)
{
	jrpc_req_pkg_t req_pkg = {};
	jrpc_rsp_pkg_t rsp_pkg = {};
	memdb_integer cm_node_id = 0;
	int host = 0;

	if (NULL == file)
		return RESULT_OTHER_ERR;

	cm_node_id = get_subnode_id_by_lid(idx, MC_TYPE_RMC, MC_TYPE_CM);
	libdb_attr_get_int(DB_RMM, cm_node_id, MBP_IP_ADDR_STR, &host, LOCK_ID_NULL);

	if (0 == host) {
		printf("cm host ip got 0. Error.\n");
		return RESULT_OTHER_ERR;
	}
	
	json_t *attr_array = json_array();
	fill_array_elem(attr_array, JRPC_ACTION_MBP_UPDATE_HOST, (void *)&host, JSON_INTEGER);
	fill_array_elem(attr_array, JRPC_ACTION_MBP_UPDATE_DEV, (void *)dev, JSON_STRING);
	fill_array_elem(attr_array, JRPC_ACTION_MBP_UPDATE_FILE, (void *)file, JSON_STRING);
	fill_jrpc_action_pkg(&req_pkg, AMACTION_MBP_FW_UPDATE, attr_array);

	send_msg_to_asset_module(&req_pkg, &rsp_pkg, DO_ACTION);

	libwrap_set_firmware_update_status(1);

	jrpc_req_pkg_free(&req_pkg);
	jrpc_rsp_pkg_free(&rsp_pkg);

#if 0
	fm_update = (struct _fm_update_info *)malloc(sizeof(struct _fm_update_info));
	if (NULL == fm_update)
		return RESULT_MALLOC_ERR;

	memset(fm_update, 0, sizeof(struct _fm_update_info));

	fm_update->host = host;
	if (dev != NULL)
		memcpy(fm_update->dev, dev, sizeof(fm_update->dev));

	len = (sizeof(fm_update->file) > strlen(file) ? strlen(file) : sizeof(fm_update->file));
	memcpy(fm_update->file, file, len);

	req.data[0] = 0x01;

	FILL_INT(req.netfn,		UPDATE_FIRMWARE_NETFN);
	FILL_INT(req.cmd,		UPDATE_FM_SEND_IP_CMD);
	FILL_INT(req.data_len,	1 + sizeof(int));

	inet_aton(ip, &addr);
	memcpy(req.data + 1, (char *)&addr.s_addr, sizeof(int));


	libjipmi_rmcp_cmd(host, IPMI_RMCP_PORT, &req, ipmi_upgrade_ip_rsp_handler, fm_update, JIPMI_NON_SYNC);
#endif


	return RESULT_OK;
}


static int get_rf_evt_type(char *input)
{
	if (0 == strcmp(input, RF_EVENT_TYPE_ADD_STR))
		return MC_REDFISH_ADD;
	else if (0 == strcmp(input, RF_EVENT_TYPE_DELETE_STR))
		return MC_REDFISH_REMOVE;
	else if (0 == strcmp(input, RF_EVENT_TYPE_UPDATE_STR))
		return MC_REDFISH_UPDATE;
	else if (0 == strcmp(input, RF_EVENT_TYPE_CHANGE_STR))
		return MC_REDFISH_CHANGE;
	else if (0 == strcmp(input, RF_EVENT_TYPE_ALERT_STR))
		return MC_REDFISH_ALERT;

	return -1;
}


memdb_integer libwrap_get_uniq_node_by_type(int type)
{
	int output = 0;
	struct node_info *subnode = NULL;
	int subnode_num = 0;
	memdb_integer node_id = 0;

	subnode = libdb_list_node_by_type(DB_RMM, type, type, &subnode_num, NULL, LOCK_ID_NULL);

	if (subnode_num != 1) {
		libdb_free_node(subnode);
		return 0;
	}

	node_id = subnode[0].node_id;
	libdb_free_node(subnode);

	return node_id;
}


memdb_integer libwrap_get_listener_id_by_dest(char *dest, memdb_integer parent, int type)
{
	int i = 0;
	char output[1024] = {0};
	struct node_info *subnode = NULL;
	int subnode_num = 0;

	if (dest == NULL) {
		return 0;
	}

	subnode = libdb_list_subnode_by_type(DB_RMM, parent, type, &subnode_num, NULL, LOCK_ID_NULL);

	for (i = 0; i < subnode_num; i++) {
		libdb_attr_get_string(DB_RMM, subnode[i].node_id, RF_EVENT_LISTENER_DEST_STR, output, 1024, LOCK_ID_NULL);

		if (0 == strcmp(dest, output))
			return subnode[i].node_id;
	}

	return 0;
}

memdb_integer libwrap_get_subnode_id_by_type(memdb_integer parent, int type)
{
	int output = 0;
	struct node_info *subnode = NULL;
	int subnode_num = 0;

	subnode = libdb_list_subnode_by_type(DB_RMM, parent, type, &subnode_num,NULL, LOCK_ID_NULL);

	if (subnode_num != 1)
		return 0;

	return subnode[0].node_id;
}


memdb_integer libwrap_get_listener(char *dest, char *evt_str, memdb_integer *evt_nid, int type)
{
	int nodenum  = 0;
	memdb_integer listener_nid = 0;
	int event_type = 0;
	struct node_info *nodeinfo;

	nodeinfo = libdb_list_node_by_type(DB_RMM, type, type, &nodenum, NULL,LOCK_ID_NULL);
	if (nodenum != 1) {
		libdb_free_node(nodeinfo);
		return -1;
	}

	event_type = get_rf_evt_type(evt_str);
	if (event_type == -1) {
		libdb_free_node(nodeinfo);
		return -1;
	}

	*evt_nid = libwrap_get_subnode_id_by_type(nodeinfo[0].node_id, event_type);
	if (*evt_nid == 0) {
		libdb_free_node(nodeinfo);
		return -1;
	}
	listener_nid = libwrap_get_listener_id_by_dest(dest, *evt_nid, MC_REDFISH_LISTENER);

	libdb_free_node(nodeinfo);
	return listener_nid;

}

result_t libwrap_subscribe_listener(listener_dest_t *listener, memdb_integer listener_nid, memdb_integer evt_nid)
{
	int mask = 0;
	int rc = 0;

	if (listener_nid == 0) {
		listener_nid = libdb_create_node(DB_RMM, evt_nid, MC_REDFISH_LISTENER, SNAPSHOT_NEED, LOCK_ID_NULL);
		rc = libdb_attr_set_string(DB_RMM, listener_nid, RF_EVENT_LISTENER_DEST_STR, 0x0, listener->dest, SNAPSHOT_NEED, LOCK_ID_NULL);
		if (rc == -1) {
			return RESULT_ATTR_ERR;
		}
	}

	libdb_attr_get_int(DB_RMM, listener_nid, RF_EVENT_LISTENER_INDEXES_STR, &mask, LOCK_ID_NULL);
	mask |= listener->idx_mask;
	rc = libdb_attr_set_int(DB_RMM, listener_nid, RF_EVENT_LISTENER_INDEXES_STR, 0x0, mask, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, listener_nid, RF_EVENT_LISTENER_TYPE_STR, 0x0, listener->type, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, listener_nid, RF_EVENT_LISTENER_PROTOCOL_STR, 0x0, listener->protocol, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	return RESULT_OK;
}


result_t libwrap_unsubscribe_listener(listener_dest_t *listener, memdb_integer listener_nid)
{
	int mask = 0;
	int rc = 0;

	libdb_attr_get_int(DB_RMM, listener_nid, RF_EVENT_LISTENER_INDEXES_STR, &mask, LOCK_ID_NULL);
	mask &= ~(listener->idx_mask);
	if (mask == 0) {
		libdb_destroy_node(DB_RMM, listener_nid, LOCK_ID_NULL);
	}

	rc = libdb_attr_set_int(DB_RMM, listener_nid, RF_EVENT_LISTENER_INDEXES_STR, 0x0, mask, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, listener_nid, RF_EVENT_LISTENER_TYPE_STR, 0x0, listener->type, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	rc = libdb_attr_set_string(DB_RMM, listener_nid, RF_EVENT_LISTENER_PROTOCOL_STR, 0x0, listener->protocol, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		return RESULT_ATTR_ERR;
	}

	return RESULT_OK;
}


result_t libwrap_get_listener_links(int reg, memdb_integer nid, rf_link_t *links)
{
	int mask = 0;
	int subnode_num, i, j, index = 0;
	memdb_integer listener_nid = 0;
	char listener[256] = {0};
	struct node_info *subnode = NULL;

	subnode = libdb_list_subnode(DB_RMM, nid, &subnode_num, NULL, LOCK_ID_NULL);
	
	for (i = 0; i < subnode_num; i++) {
		listener_nid = libwrap_get_subnode_id_by_type(subnode[i].node_id, MC_REDFISH_LISTENER);
		if ( listener_nid == 0)
			continue;

		libdb_attr_get_int(DB_RMM, listener_nid, RF_EVENT_LISTENER_INDEXES_STR, &mask, LOCK_ID_NULL);
		if ((mask & reg) != 0) {
			libdb_attr_get_string(DB_RMM, listener_nid, RF_EVENT_LISTENER_DEST_STR, listener, 256, LOCK_ID_NULL);
			for (j = 0; j < index; j++) {
				if (strcmp(listener, links->subscription[j]) == 0)
					break;
			}
			if (j == index) {
				snprintf(links->subscription[index], sizeof(links->subscription[index]), "%s", listener);
				index++;
			}
		}
	}

	return RESULT_OK;
}

