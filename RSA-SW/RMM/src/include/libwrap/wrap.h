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


#ifndef __LIBWRAP_WRAP_H__
#define __LIBWRAP_WRAP_H__

#include <time.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "libutils/list.h"
#include "libjipmi/common.h"
#include "libmemdb/memdb.h"

#include "libjipmi/libjipmi.h"

#include "libredfish/rf_event.h"
#include "libutils/rmm.h"
#include "libutils/types.h"

#define WRAP_DEBUG 0
#define ODATA_ADD_ATTR

//********************** MACRO and Constant definition **********************************

#define REST_RACK_STRING_LEN		64
#define REST_MAX_DESCR_LEN			64
#define REST_TIME_MAX_LEN			64
#define REST_ASSET_TAG_LEN			64
#define REST_PRESENCE_LEN			64

#define PSU_CAP_REQ_ST_SUP_NUM		3
#define MAX_REQ_PROCESS       32
#define IPMI_MAX_RESP_LEN     128

#define RACK_STRING_LEN       64
#define RACK_TAG_LEN          32
#define DCUID_LEN             64
#define UUID_LEN              64
#define RMM_NAME_LEN          32
#define DESCRIPTION_LEN       128
#define MAC_LEN               6
#define VERSION_LEN           16
#define HREF_URL_LEN          128 
#define CURR_PRESENT_LEN      8
#define TRAY_PUID_LEN         16
#define DATE_LEN              64

#define LOWER_POWER_ZONE_ID   1
#define UPPER_POWER_ZONE_IP   2

#define WRAP_RET_OK             0
#define WRAP_RET_TIMEOUT        1
#define WRAP_RET_NULL_NODE      2
#define WRAP_RET_ATTR_ERR       3
#define WRAP_RET_OTHER_ERR      4
#define WRAP_RET_INPUT_ERR      5

#define WRAP_LOC_ID_STR			"loc_id"
#define WRAP_UUID_STR			"uuid"
#define WRAP_NAME_STR			"name"
#define WRAP_DESCRIPT_STR		"descript"
#define WRAP_PUID_STR   		"puid"
#define WRAP_DCUID_STR   		"dcuid"
#define WRAP_ONLINE_STATE_STR	"online_state"

#define RACK_LOC_ID_STR        WRAP_LOC_ID_STR
#define RACK_UUID_STR          WRAP_UUID_STR
#define RACK_NAME_STR          WRAP_NAME_STR
#define RACK_DESCRIPT_STR      WRAP_DESCRIPT_STR
#define RACK_CREATE_DATE_STR   "create_date"
#define RACK_UPDATE_DATE_STR   "update_date"
#define RACK_PUID_STR          WRAP_PUID_STR
#define RACK_DCUID_STR         WRAP_DCUID_STR
#define RACK_GEOTAG_STR        "geo_tag"
#define RACK_API_VER_STR       "api_ver"
#define RACK_PODM_ADDR_STR     "podm_addr"
#define RACK_ASSET_TAG_STR     "asset_tag"
#define RACK_SER_NUM_STR       "serial_num"
#define RACK_MANUFACT_STR      "manufacture"
#define RACK_MODEL_STR         "rack_model"
#define RACK_PART_NUM_STR      "part_num"
#define RACK_FW_VER_STR        "fw_ver"
#define RACK_AV_RST_1_STR      "rack_av_resettype_1"
#define RACK_AV_RST_2_STR      "rack_av_resettype_2"
#define RACK_AV_UPDATE_1_STR   "rack_av_update_1"
#define RACK_AV_UPDATE_2_STR   "rack_av_update_2"
#define RACK_AV_DEFAULT_STR    "rack_av_default"
#define RACK_HREF_DRAWER_STR   "href_drawer"
#define RACK_HREF_PZONE_STR    "href_pzone"
#define RACK_HREF_TZONE_STR    "href_tzone"
#define RACK_HREF_EVENT_STR    "href_event"
#define RACK_HREF_MBP_STR      "href_mbp"
#define POD_DCUID_STR          "pod_dcuid"
#define RACK_RMM_PRES          "rmm_present"
#define RACK_DISAGGR           "rack_disaggr"
#define RACK_POD               "rack_pod"
#define RACK_ONLINE_STATE_STR		WRAP_ONLINE_STATE_STR

#define DZONE_LOC_ID_STR            WRAP_LOC_ID_STR
#define DZONE_UUID_STR              WRAP_UUID_STR
#define DZONE_NAME_STR              WRAP_NAME_STR
#define DZONE_DESCRIPT_STR          WRAP_DESCRIPT_STR
#define DZONE_CREATE_DATE_STR       "create_date"
#define DZONE_UPDATE_DATE_STR       "update_date"
#define DZONE_X_LOC_STR             "x_location"
#define DZONE_U_LOC_STR             "u_location"
#define DZONE_U_HEIGHT_STR          "u_height"
#define DZONE_ONLINE_STATE_STR		WRAP_ONLINE_STATE_STR
#define DZONE_DRAWER_NUM            "drawer_num"

#define DRAWER_LOC_ID_STR              WRAP_LOC_ID_STR
#define DRAWER_UUID_STR                WRAP_UUID_STR
#define DRAWER_NAME_STR                WRAP_NAME_STR
#define DRAWER_DESCRIPT_STR            WRAP_DESCRIPT_STR
#define DRAWER_CREATE_DATE_STR         "create_date"
#define DRAWER_UPDATE_DATE_STR         "update_date"
#define DRAWER_TMC_IP_STR              "drawer_tmc_ip"
#define DRAWER_RUID_STR                "drawer_ruid"
#define DRAWER_PUID_STR                "drawer_puid"
#define DRAWER_X_LOC_STR               "x_location"
#define DRAWER_U_LOC_STR               "u_location"
#define DRAWER_U_HEIGHT_STR            "u_height"
#define DRAWER_PWR_CONSUM_STR          "drawer_pwr_consum"
#define DRAWER_THERMAL_ST_STR          "drawer_termal_st"
#define DRAWER_AV_RST_TYPE_1_STR       "drawer_av_resettype_1"
#define DRAWER_AV_RST_TYPE_2_STR       "drawer_av_resettype_2"
#define DRAWER_POD					"pod"
#define DRAWER_RACK					"rack"
#define DRAWER_RMM_PRES             "rmm_present"
#define DRAWER_DISAGGR              "rack_disaggr"
#define DRAWER_PRES_TEMP            "present_temperature"
#define DRAWER_ONLINE_STATE_STR		WRAP_ONLINE_STATE_STR


#define MBP_LOC_ID_STR              WRAP_LOC_ID_STR
#define MBP_UUID_STR                WRAP_UUID_STR
#define MBP_NAME_STR                WRAP_NAME_STR
#define MBP_DESCRIPT_STR            WRAP_DESCRIPT_STR
#define MBP_CREATE_DATE_STR         "create_date"
#define MBP_UPDATE_DATE_STR         "update_date"
#define MBP_MBPID_STR               "mbp_mbpid"
#define MBP_HW_ADDR_STR             "mbp_hw_addr"
#define MBP_IP_ADDR_STR             "mbp_ip_addr"
#define MBP_FW_STATE                "mbp_fw_state"
#define MBP_X_LOC_STR               "x_location"
#define MBP_U_LOC_STR               "u_location"
#define MBP_U_HEIGHT_STR            "u_height"
#define MBP_ASSET_TAG_STR           "asset_tag"
#define MBP_SER_NUM_STR             "serial_num"
#define MBP_MANUFACT_STR            "manufacture"
#define MBP_MODEL_STR               "cm_model"
#define MBP_PART_NUM_STR            "part_num"
#define MBP_FW_VER_STR              "fw_ver"
#define MBP_AV_RST_TYPE_1_STR       "mbp_av_resettype_1"
#define MBP_AV_SET_UART_1_STR       "mbp_av_set_uart_1"
#define MBP_AV_UPDATE_1_STR         "mbp_av_update_1"
#define MBP_ONLINE_STATE_STR        WRAP_ONLINE_STATE_STR

#define X_LOC_STR                   "x_location"
#define U_LOC_STR                   "u_location"
#define U_HEIGHT_STR                "u_height"

#define PZONE_LOC_ID_STR            WRAP_LOC_ID_STR
#define PZONE_UUID_STR              WRAP_UUID_STR
#define PZONE_NAME_STR              "pzone_name"
#define PZONE_DESCRIPT_STR          "pzone_descript"
#define PZONE_CREATE_DATE_STR       "create_date"
#define PZONE_UPDATE_DATE_STR       "update_date"
#define PZONE_X_LOC_STR             X_LOC_STR
#define PZONE_U_LOC_STR             U_LOC_STR
#define PZONE_U_HEIGHT_STR          U_HEIGHT_STR
#define PZONE_TT_PWR_CONSUM_STR     "pzone_tt_pwr_consum"
#define PZONE_TT_PWR_CAP_STR        "pzone_tt_pwr_cap"
#define PZONE_TT_PWR_PROD_STR       "pzone_tt_pwr_prod"
#define PZONE_ONLINE_STATE_STR		WRAP_ONLINE_STATE_STR
#define PZONE_PSU_NUM               "psu_num"

#define PSU_LOC_ID_STR              WRAP_LOC_ID_STR
#define PSU_UUID_STR                WRAP_UUID_STR
#define PSU_NAME_STR                "psu_name"
#define PSU_DESCRIPT_STR            "psu_descript"
#define PSU_CREATE_DATE_STR         "create_date"
#define PSU_UPDATE_DATE_STR         "update_date"
#define PSU_ENABLE_ST_STR           "psu_enable_state"
#define PSU_REDUND_SET_STR          "psu_redundancy_set"
#define PSU_HEALTH_ST_STR           "psu_health_state"
#define PSU_TT_PWR_IN_STR           "psu_tt_pwr_in"
#define PSU_TT_PWR_CAP_STR          "psu_tt_pwr_cap"
#define PSU_TT_CURRENT_OUT_STR      "psu_tt_current_out"
#define PSU_ASSET_TAG_STR     		"asset_tag"
#define PSU_SER_NUM_STR       		"serial_num"
#define PSU_MANUFACT_STR      		"manufacture"
#define PSU_MODEL_STR         		"psu_model"
#define PSU_PART_NUM_STR      		"part_num"
#define PSU_FW_VER_STR        		"fw_ver"
#define PSU_AV_REQ_ST_1_STR       	"psu_av_req_st_1"
#define PSU_AV_REQ_ST_2_STR       	"psu_av_req_st_2"
#define PSU_AV_REQ_ST_3_STR       	"psu_av_req_st_3"
#define PSU_X_LOC_STR               X_LOC_STR
#define PSU_U_LOC_STR               U_LOC_STR
#define PSU_U_HEIGHT_STR            U_HEIGHT_STR
#define PSU_ONLINE_STATE_STR		WRAP_ONLINE_STATE_STR


#define TZONE_LOC_ID_STR            WRAP_LOC_ID_STR
#define TZONE_UUID_STR              WRAP_UUID_STR
#define TZONE_NAME_STR              "tzone_name"
#define TZONE_DESCRIPT_STR          "tzone_descript"
#define TZONE_CREATE_DATE_STR       "treate_date"
#define TZONE_UPDATE_DATE_STR       "update_date"
#define TZONE_X_LOC_STR             "x_location"
#define TZONE_U_LOC_STR             "u_location"
#define TZONE_U_HEIGHT_STR          "u_height"
#define TZONE_POLICY_STR            "tzone_policy"
#define TZONE_ONLINE_STATE_STR		WRAP_ONLINE_STATE_STR
#define TZONE_FAN_NUM               "fan_num"

#define FAN_LOC_ID_STR              WRAP_LOC_ID_STR
#define FAN_UUID_STR                WRAP_UUID_STR
#define FAN_NAME_STR                "fan_name"
#define FAN_DESCRIPT_STR            "fan_descript"
#define FAN_CREATE_DATE_STR         "create_date"
#define FAN_UPDATE_DATE_STR         "update_date"
#define FAN_ENABLE_STATE_STR        "enable_state"
#define FAN_HEALTH_STATE_STR        "health_state"
#define FAN_DESIRED_SPD_PWM_STR     "desired_pwm_spd"
#define FAN_DESIRED_SPD_RPM_STR     "desired_rpm_spd"
#define FAN_SPEED_PWM_RESTORE_STR   "pwm_restore"
#define FAN_TACH_READ_STR           "tach_read"
#define FAN_TACH_STATE_STR          "tach_state"
#define FAN_TACH_THRESH_LNC_STR     "tach_thresh_lnc"
#define FAN_TACH_THRESH_UNC_STR     "tach_thresh_unc"
#define FAN_TACH_THRESH_LC_STR      "tach_thresh_lc"
#define FAN_TACH_THRESH_UC_STR      "tach_thresh_uc"
#define FAN_TACH_PRESENT_STR        "tach_present"
#define FAN_ASSET_TAG_STR           "asset_tag"
#define FAN_SER_NUM_STR       		"serial_num"
#define FAN_MANUFACT_STR      		"manufacture"
#define FAN_MODEL_STR         		"fan_model"
#define FAN_PART_NUM_STR      		"part_num"
#define FAN_FW_VER_STR        		"fw_ver"
#define FAN_AV_RSC_EN_VAL_1_STR     "rsc_en_val_1"
#define FAN_AV_RSC_EN_VAL_2_STR     "rsc_en_val_2"
#define FAN_AV_SDS_UNIT_1_STR       "sds_unit_1"
#define FAN_AV_SDS_UNIT_2_STR       "sds_unit_2"
#define FAN_AV_SDS_VAL_STR          "sds_val"
#define FAN_PWM_PREFIX_STR          "fan_pwm"
#define FAN_X_LOC_STR               X_LOC_STR
#define FAN_U_LOC_STR               U_LOC_STR
#define FAN_U_HEIGHT_STR            U_HEIGHT_STR
#define FAN_ONLINE_STATE_STR		WRAP_ONLINE_STATE_STR


#define WRAP_DB_MAX_KEY_LEN         32
#define WRAP_DB_MAX_VALUE_LEN       128

#define MAX_DRAWER_PRESENCE	        16
#define MAX_MBP_PRESENCE	        2

#define FAN_THRESH_MAX		    20000
#define FAN_STATE_ENABLED   	2
#define FAN_STATE_DISABLED   	3
#define PSU_STATE_ENABLED   	2
#define PSU_STATE_DISABLED   	3
#define PSU_STATE_NOT_APP   	5
#define PSU_STATE_EN_OFFLINE   	6

#define TFTP_SERVER_PATH	"/srv/tftp"

//********************** Structure definition *********************************


typedef enum{
	ACT_RESET   = 0,
	ACT_UPDATE  = 1,
	ACT_DEFAULT = 2,
	ACT_MAX     = 3,
} action_enum_t;

typedef enum{
	HREF_SYS    = 0,
	HREF_PZONE  = 1,
	HREF_CZONE  = 2,
	HREF_EVENT  = 3,
	HREF_MBP = 4,
	HREF_MAX    = 5,
} href_enum_t;


typedef enum{
	RESULT_OK      		= 0,
	RESULT_NO_RESP  	= 1,	//socket no response, timeout
	RESULT_NO_NODE  	= 2,	//node not exist
	RESULT_ATTR_ERR		= 3,	//attributor name error
	RESULT_IPMI_ERR		= 4,	//IPMI command fail
	RESULT_MALLOC_ERR	= 5,
	RESULT_UNMATCHED_VERSION =6,
	RESULT_SAME_VERSION = 7,
	RESULT_LEN_ERR 		= 8,
	RESULT_SKIP			= 9,
	RESULT_OTHER_ERR 	= 100	//other error
} result_t;

typedef enum{
	REQ_STATE_EN 				= 2,	//Enabled
	REQ_STATE_DIS 				= 3,	//Disabled
	REQ_STATE_NOT_APP 			= 5,	//Not applicable
	REQ_STATE_EN_BUT_OFFLINE 	= 6		//Enabled but offline(for redundancy design)
}request_enabled_state_t;

typedef struct base_element{
	int		id;
	uint8	uuid[UUID_LEN];
	uint8	name[RMM_NAME_LEN];
	uint8	desc[DESCRIPTION_LEN];
	uint8   create_date[DATE_LEN];
	uint8   update_date[DATE_LEN];
}base_element_t;

//#define MAX_ACTION			3
#define MAX_CAPABILITY		3

typedef struct capability{
	uint8 property[RMM_NAME_LEN];
	uint8 av[REST_RACK_STRING_LEN];
}capability_t;

typedef struct avail_action{
	uint8 action[RMM_NAME_LEN];
	capability_t cap[MAX_CAPABILITY];
}avail_action_t;

typedef struct collections{
	uint32	id;
	uint8	uuid[UUID_LEN];
	uint8	name[RMM_NAME_LEN];
	uint8 	href[HREF_URL_LEN];
}collections_t;

typedef enum{
	RACK_UNITS_RO 				= 0,
	RACK_UNITS_OU 				= 1
}rack_units_t;

typedef struct location{
	uint32 units;
	uint32 x_location;
	uint32 u_location;
	uint32 u_height;
}location_t;

typedef struct fru_info{
	uint8 serial_num[REST_RACK_STRING_LEN];
	uint8 manufacture[REST_RACK_STRING_LEN];
	uint8 model[REST_RACK_STRING_LEN];
	uint8 part_num[REST_RACK_STRING_LEN];
	uint8 fw_ver[REST_RACK_STRING_LEN];
}fru_info_t;

typedef struct psu_asset_info{
	uint8 asset_tag[REST_ASSET_TAG_LEN];
	fru_info_t fru;
}asset_info_t;

typedef struct href{
	uint8 drawer[HREF_URL_LEN];
	uint8 powerzones[HREF_URL_LEN];
	uint8 thermalzones[HREF_URL_LEN];
	uint8 rf_event[HREF_URL_LEN];
	uint8 mbps[HREF_URL_LEN];
}href_t;

typedef struct rack_info{
	base_element_t	be;
	int64	rack_puid;
	uint8	pod_dcuid[DCUID_LEN];
	uint8	rack_dcuid[DCUID_LEN];
	uint8	geo_tag[RACK_TAG_LEN];
	uint8	api_ver[VERSION_LEN];
	uint8	podm_addr[REST_RACK_STRING_LEN];
	asset_info_t	asset;
	unsigned char drawer_pres[MAX_DRAWER_PRESENCE + 1];
	unsigned char mbp_pres[MAX_MBP_PRESENCE + 1];
	avail_action_t av_action[3];// 3	
	href_t	href;
	//uint8 rmm_present[REST_RACK_STRING_LEN];
	int64 rmm_present;
	//uint8 rack_disaggr[REST_RACK_STRING_LEN];
	int64 rack_disaggr;
	uint8 pod[REST_RACK_STRING_LEN];
}rack_info_t;

typedef struct put_rack_info{
	uint8	descr[DESCRIPTION_LEN];
	int64	rack_puid;
	uint8	pod_dcuid[DCUID_LEN];
	uint8	geo_tag[RACK_TAG_LEN];
	uint8	podm_addr[REST_RACK_STRING_LEN];
	uint8	asset_tag[REST_RACK_STRING_LEN];
}put_rack_info_t;

typedef struct put_drawer{
	uint8	descr[DESCRIPTION_LEN];
}put_drawer_t;

typedef struct put_pzone{
	uint8	descr[DESCRIPTION_LEN];
}put_pzone_t;

typedef struct put_tzone{
	uint8	descr[DESCRIPTION_LEN];
}put_tzone_t;

typedef struct put_mbp{
	uint8	descr[DESCRIPTION_LEN];
	uint8 asset_tag[REST_ASSET_TAG_LEN];
}put_mbp_t;

typedef struct pzone_member{
	base_element_t	be;
	location_t power_zone_loc;
	uint32 tot_power_consumption;
	uint32 tot_power_cap;
	uint32 tot_power_production;
	char presence[REST_PRESENCE_LEN];
	//avail_action_t av_action;// 0
}pzone_member_t;	

typedef struct psu_member{
	base_element_t	be;
	int32	enable_state;
	int8	redundancy[REST_RACK_STRING_LEN];
	int32	health_state;
	int32	power_in;
	int32	power_cap;
	int32	power_out;
	asset_info_t asset;
	//capability_t	capability;
	avail_action_t av_action; // 1
	location_t psu_loc;
}psu_member_t;

typedef struct tzone_member{
	base_element_t	be;
	location_t tzone_loc;
	char presence[REST_PRESENCE_LEN];
	//avail_action_t av_action;// 0
	//uint8 	href[HREF_URL_LEN];
	uint8 policy[RACK_STRING_LEN];
	int64 pres_temp;
	int64 outlet_temp;
	int64 volumetric_airflow;
	int64 max_fan_sup;
}tzone_member_t;

typedef struct threshold{
	uint32 lower_non_critical;
	uint32 upper_non_critical;
	uint32 lower_critical;
	uint32 upper_critical;
}threshold_t;

typedef struct fan_tachometer{
	uint32 reading;
	uint8	current_state[RMM_NAME_LEN];
	threshold_t threshold;
}fan_tachometer_t;

typedef struct fan_member{
	base_element_t	be;
	uint32	enable_state;
	uint32	health_state;
	uint8	desired_speed[RMM_NAME_LEN];
	fan_tachometer_t tachometer;
	uint8 presence[REST_PRESENCE_LEN];
	//uint8 asset_tag[REST_ASSET_TAG_LEN];
	asset_info_t asset;
	avail_action_t av_action[2];// 0	
	location_t fan_loc;
	uint8 speed_unit;// 0--PWM, 1---RPM
} fan_member_t;

typedef struct fan_pwm{
	memdb_integer node_id;
	uint32 fan_id;
	uint32 pwm;
}fan_pwm_t;

typedef struct dzone_member{
	base_element_t	be;
	location_t dzone_loc;
	char presence[REST_PRESENCE_LEN];
	int64 max_drawer_sup;
} dzone_member_t;

typedef struct drawer_member{
	base_element_t	be;
	uint8 TMC_ip_addr[REST_RACK_STRING_LEN];
	uint32	tray_ruid;
	uint8	tray_puid[REST_RACK_STRING_LEN];
	location_t loc;
	uint32	power_consumption;
	uint32	thermal_st;
	uint32  pres_temp;
	//uint8   rmm_present[REST_RACK_STRING_LEN];
	int64 rmm_present;
	//uint8   rack_disaggr[REST_RACK_STRING_LEN];
	int64 rack_disaggr;
	uint8   pod[REST_RACK_STRING_LEN];
	uint8   rack[REST_RACK_STRING_LEN];
	avail_action_t av_action;// 1	
	uint8 	href[HREF_URL_LEN];	
} drawer_member_t;

typedef struct mbp_member{
	base_element_t	be;
	uint8 MBPID[REST_RACK_STRING_LEN];
	uint8 MBPHWAddress[REST_RACK_STRING_LEN];
	uint8 MBPIPAddress[REST_RACK_STRING_LEN];
	uint8 fw_state[REST_RACK_STRING_LEN];
	location_t loc;
	asset_info_t	asset;
	avail_action_t av_action[3];// 3
} mbp_member_t;

typedef struct put_fan{
	uint8	descr[DESCRIPTION_LEN];
	//uint8	desired_speed[RMM_NAME_LEN];
	threshold_t threshold;
	uint8 asset_tag[REST_ASSET_TAG_LEN];
}put_fan_t;

struct wrap_cb {
	uint8 resp;
	int8  data[IPMI_MAX_RESP_LEN];
} wrap_cb_t;

struct wrap_ipmi {
	struct list_head  list;
	unsigned int host;
	struct jipmi_msg msg;
	ipmi_rsp_callback_fn cb_fn;
	void *cb_arg;
} wrap_ipmi_t;

typedef struct wrap_msg {
	struct list_head wrap_head;
	int curr_process;
} wrap_msg_t;

typedef struct id_tbl {
	int32 pid;
	int32 idx;
} id_tbl_t;


typedef result_t (*memdb_get_fn) (char* name, char* value, int size, va_list* valist);

typedef struct memdb_tbl {
	char   key[WRAP_DB_MAX_KEY_LEN];
	unsigned char snapshot_flag;
	char   reset_flag;
	memdb_get_fn get_fn;
	char   value[WRAP_DB_MAX_VALUE_LEN];
} memdb_tbl_t;

typedef struct json_map_tbl {
	char  name[32];
	char  submenu[32];
}json_map_tbl_t;

typedef struct listener_dest_info {
	char  dest[128];
	int   event_types[6];
	int   evt_index;
	struct listener_dest_info *pnext;
} listener_dest_info_t;


//********************** Function prototype ***********************************

//extern result_t libwrap_get_time(char *output);
//extern result_t libwrap_get_time(char *uuid);

//extern result_t libwrap_get_time(char *key, char *output);
//extern result_t libwrap_get_ip(char *ip);
extern result_t libwrap_get_mbp_hw_addr(unsigned int pos,
										unsigned char *hw_addr);
extern result_t libwrap_get_node_id_by_uuid(unsigned char *uuid,
											memdb_integer *node_id);
extern result_t libwrap_get_rack(rack_info_t *rack_info);
extern result_t libwrap_put_rack(const put_rack_info_t put_rack_info);
extern result_t libwrap_get_pzone_coll(collections_t *pzone, uint32 *number);
extern result_t libwrap_get_pzone_by_idx(uint32 pzone_idx, pzone_member_t *pzone_member);
extern result_t libwrap_get_pzone_psu_coll(uint32 pzone_idx, collections_t *psu, uint32 *number);
extern result_t libwrap_get_pzone_psu_by_idx(const uint32 pzone_idx, const uint32 psu_idx,  psu_member_t * psu_member);
extern result_t libwrap_post_pzone_psu_by_idx(uint32 pzone_idx, uint32 psu_idx, int state);
extern result_t libwrap_get_tzone_coll(collections_t *tzone, uint32 *number);
extern result_t libwrap_get_tzone_by_idx(uint32 tzone_idx, tzone_member_t *tz_member);
extern result_t libwrap_get_tzone_fan_coll(uint32 tzone_idx, collections_t *fan, uint32 *number);
extern result_t libwrap_get_tzone_fan_by_idx(uint32 tzone_idx, uint32 fan_idx, fan_member_t *fan_member);
extern result_t libwrap_get_dzone_coll(collections_t *dzone, uint32 *number);
extern result_t libwrap_get_dzone_by_idx(uint32 dzone_idx, dzone_member_t *dz_member);
extern result_t libwrap_get_drawer_coll(collections_t *drawer, uint32 *number);
extern result_t libwrap_get_drawer_by_idx(int32 dzone_idx, int32 drawer_idx, drawer_member_t *drawer_member);
extern result_t libwrap_get_log(int begin, int count, void* info);
extern result_t libwrap_put_drawer(int32 dzone_idx, int32 drawer_idx, const put_drawer_t put_drawer_info);
extern result_t libwrap_check_update_capability();
extern void wrap_init(void);
//extern result_t libwrap_gen_uuid(char *key, char *uuid);
extern result_t libwrap_put_fan(uint32 tzone_id, uint32 fan_idx, const put_fan_t put_fan_info);
extern result_t libwrap_get_mbp_by_idx( int id, mbp_member_t *mbp_member);
extern result_t libwrap_put_mbp(int id, const put_mbp_t put_mbp_info);
extern result_t libwrap_get_mbp_coll(collections_t* mbp, uint32 *number);
extern memdb_integer get_subnode_id_by_lid(unsigned int lid, memdb_integer node_id, int type);
extern result_t libwrap_put_psu(uint32 pzone_idx, uint32 psu_idx, char *descr, char *asset_tag);
extern result_t libwrap_put_pzone_by_idx(int idx, const put_pzone_t put_pzone_info);
extern int get_zone_lid(int zone_type, int zone_idx);
extern int get_cm_lid(int zone_type, int zone_idx);
extern int libwrap_get_asset_num(int zone_type);
#if 0
extern result_t libwrap_get_psu_serial(memdb_integer *node_id, int psu_idx, int pzone_idx);
extern result_t libwrap_get_psu_manufacture(memdb_integer *node_id, int psu_idx, int pzone_idx);
extern result_t libwrap_get_psu_model(memdb_integer *node_id, int psu_idx, int pzone_idx);
extern result_t libwrap_get_psu_fw_ver(memdb_integer *node_id, int psu_idx, int pzone_idx);
extern result_t libwrap_get_drawer_ip(memdb_integer* node_id, int32 dz_idx, int32 drawer_idx);
#endif

extern result_t libwrap_put_tzone_by_idx(int idx, const put_tzone_t put_tzone_info);
extern result_t libwrap_get_psu_node_id_by_idx(int pzone_idx, int psu_idx, memdb_integer *node_id);
extern result_t libwrap_post_fan_change_state(uint32 tzone_idx, uint32 fan_idx, int64 enabled_state);
extern result_t libwrap_post_fan_set_speed(uint32 tzone_idx, uint32 fan_idx, uint32 type, char *speed);
extern result_t libwrap_drawer_hard_reset(int32 dzone_idx, int32 drawer_idx, int* result);
extern result_t libwrap_drawer_soft_reset(int32 dzone_idx, int32 drawer_idx, int* result);
extern result_t libwrap_mbp_soft_reset(int idx, int *result);
extern result_t libwrap_mbp_hard_reset(int idx, int *result);

#ifdef PLATFORM_BDC_R
extern result_t libwrap_mbp_uart_switch(int cm_idx, int tray_idx, int tgt_idx);
#else
extern result_t libwrap_mbp_uart_switch(int cm_idx, int tgt_idx);
#endif

extern result_t libwrap_rmm_factory_reset(int *result);
extern result_t libwrap_pre_put_rack(put_rack_info_t *put_rack_info);
extern result_t libwrap_pre_put_mbp(int id, put_mbp_t *put_mbp_info);
extern result_t libwrap_pre_put_pzone_by_idx(int idx, put_pzone_t *put_pzone_info);
extern result_t libwrap_pre_put_drawer(uint32 dzone_idx, uint32 drawer_idx, put_drawer_t *put_sys_info);
extern result_t libwrap_pre_put_psu(uint32 pzone_idx, uint32 psu_idx, char *descr, char *asset_tag);
extern result_t libwrap_pre_put_tzone_by_idx(int idx, put_tzone_t *put_tzone_info);
extern result_t libwrap_pre_put_fan(uint32 tzone_idx, uint32 fan_idx, put_fan_t *put_fan_info);
extern result_t libwrap_update_mbp_firmware(int idx, char *dev, char *file);
extern result_t libwrap_check_tftp_service(void);
extern int libwrap_get_firmware_update_status(void);
extern void libwrap_set_firmware_update_status(int status);
extern result_t libwrap_get_listeners_by_type(int type, int pindex /*only for zone id*/, int index, listener_dest_info_t *header);
extern result_t libwrap_subscribe_listener(listener_dest_t *listener, memdb_integer listener_nid, memdb_integer evt_nid);
extern result_t libwrap_unsubscribe_listener(listener_dest_t *listener, memdb_integer listener_nid);
extern memdb_integer libwrap_get_uniq_node_by_type(int type);
extern memdb_integer libwrap_get_listener(char *dest, char *evt_str, memdb_integer *evt_nid, int type);
extern memdb_integer libwrap_get_subnode_id_by_type(memdb_integer parent, int type);
extern memdb_integer libwrap_get_listener_id_by_dest(char *dest, memdb_integer parent, int type);
extern result_t libwrap_get_listener_links(int reg, memdb_integer nid, rf_link_t *links);
extern result_t libwrap_connect_asset_module();
#endif
