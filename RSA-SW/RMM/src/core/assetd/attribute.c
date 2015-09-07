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


#include <stdarg.h>
#include "libutils/rmm.h"
#include "libutils/time.h"
#include "libutils/ip.h"
#include "libwrap/wrap.h"
#include "librmmcfg/rmm_cfg.h"
#include "librmmlog/rmmlog.h"
#include "attribute.h"
#include "libutils/utils.h"
#include "libutils/string.h"

static inline result_t get_time(char *name, char *value, int size, va_list *valist)
{
	return libutils_get_time(name, value);
}

static inline result_t get_uuid(char *key, char *uuid, int size, va_list *valist)
{
	gen_uuid(uuid);
	return RESULT_OK;
}

static inline result_t get_rack_api_ver(char *name, char *value, int size, va_list *valist)
{
	strncpy_safe(value, RACK_API_VERSION, size, strlen(RACK_API_VERSION));
	return RESULT_OK;
}

static inline result_t get_rack_fw_ver(char *name, char *value, int size, va_list *valist)
{
	strncpy_safe(value, RSA_RMM_FIRMWARE_VERSION, size, strlen(RSA_RMM_FIRMWARE_VERSION));
	return RESULT_OK;
}

static inline result_t get_href_url(char *value, int size, va_list *valist, char *link)
{
	char *ip_addr;
	int port;
	char prefix[MAX_URL] = {0};
	char new_link[MAX_URL + 12] = {0};

	rmm_cfg_get_rest_prefix(prefix, MAX_URL);
	insert_str(new_link, link, 12, prefix);/// "http://%s:%d/rack/" --->http://%s:%d/prefix/rack/

	port = rmm_cfg_get_port(RESTD_PORT);
	if (port == 0) {
		rmm_log(ERROR, "Call rmm_cfg_get_restd_port failed.\n");
	}

	ip_addr = va_arg(*valist, char *);
	snprintf(value, size, new_link, ip_addr, port);

	return RESULT_OK;
}

static inline result_t get_rack_href_drawer(char *name, char *value, int size, va_list *valist)
{
	return get_href_url(value, size, valist, "http://%s:%d/rack/drawers/");
}

static inline result_t get_rack_href_pzone(char *name, char *value, int size, va_list *valist)
{
	return get_href_url(value, size, valist, "http://%s:%d/rack/powerzones/");
}

static inline result_t get_rack_href_tzone(char *name, char *value, int size, va_list *valist)
{
	return get_href_url(value, size, valist, "http://%s:%d/rack/thermalzones/");
}

static inline result_t get_rack_href_mbp(char *name, char *value, int size, va_list *valist)
{
	return get_href_url(value, size, valist, "http://%s:%d/rack/mbps/");
}

static inline result_t get_rack_href_event(char *name, char *value, int size, va_list *valist)
{
	return get_href_url(value, size, valist, "http://%s:%d/rack/rf_event/");
}

static memdb_tbl_t rack_attr_memdb[] = {
	{RACK_LOC_ID_STR,		PERSISTENT_Y, RESET_Y,	NULL,					"1"},
	{RACK_UUID_STR,		    PERSISTENT_N, RESET_N,	get_uuid,				{0} /*"25892e17-80f6-415f-9c65-7395632f0223"*/},
	{RACK_NAME_STR,		    PERSISTENT_N, RESET_Y,	NULL,					"RSA_Rack_1"},
	{RACK_DESCRIPT_STR,	    PERSISTENT_Y, RESET_Y,	NULL,					"for storage pool"},
	{RACK_CREATE_DATE_STR,  PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{RACK_UPDATE_DATE_STR,  PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{RACK_PUID_STR,		    PERSISTENT_Y, RESET_Y,	NULL,					"1"},
	{RACK_DCUID_STR,		PERSISTENT_N, RESET_Y,	NULL,					"rack.jf5.intel.com-1"},
	{RACK_GEOTAG_STR,		PERSISTENT_Y, RESET_Y,	NULL,					"Oregon-JF4"},
	{RACK_API_VER_STR,		PERSISTENT_N, RESET_N,	get_rack_api_ver,		{0} /*"0.9"*/},
	{RACK_PODM_ADDR_STR,	PERSISTENT_Y, RESET_Y,	NULL,					"podm2.pod1.jf5.intel.com"},
	{RACK_ASSET_TAG_STR,	PERSISTENT_Y, RESET_Y,	NULL,					"asset_tag_1"},
	{RACK_SER_NUM_STR,		PERSISTENT_N, RESET_N,	NULL,					"1234567890"},
	{RACK_MANUFACT_STR,	    PERSISTENT_N, RESET_N,	NULL,					"Intel"},
	{RACK_MODEL_STR,		PERSISTENT_N, RESET_N,	NULL,					"RSA_Rack"},
	{RACK_PART_NUM_STR,	    PERSISTENT_N, RESET_N,	NULL,					"0987654321"},
	{RACK_FW_VER_STR,		PERSISTENT_N, RESET_N,	get_rack_fw_ver,		{0} /*"v0.5"*/},
	{RACK_AV_RST_1_STR,	    PERSISTENT_N, RESET_N,	NULL,					"service"},
	{RACK_AV_RST_2_STR,		PERSISTENT_N, RESET_N,	NULL,					"rmm"},
	{RACK_AV_UPDATE_1_STR,	PERSISTENT_N, RESET_N,	NULL,					"bmc_fw1"},
	{RACK_AV_UPDATE_2_STR,	PERSISTENT_N, RESET_N,	NULL,					"bmc_fw2"},
	{RACK_AV_DEFAULT_STR,	PERSISTENT_N, RESET_N,	NULL,					"bmc_fw3"},
	{RACK_HREF_DRAWER_STR,	PERSISTENT_N, RESET_N,	get_rack_href_drawer,		{0} /*"http://x:8080/rack/drawer/"*/},
	{RACK_HREF_PZONE_STR,	PERSISTENT_N, RESET_N,	get_rack_href_pzone,	{0} /*"http://x:8080/rack/powerzones/"*/},
	{RACK_HREF_TZONE_STR,	PERSISTENT_N, RESET_N,	get_rack_href_tzone,	{0} /*"http://x:8080/rack/thermalzones/"*/},
	{RACK_HREF_EVENT_STR,	PERSISTENT_N, RESET_N,	get_rack_href_event,	{0} /*"http://x:8080/rack/rf_event/"*/},
	{RACK_HREF_MBP_STR,	    PERSISTENT_N, RESET_N,	get_rack_href_mbp,		{0} /*"http://x:8080/rack/mbps/"*/},
	{POD_DCUID_STR,		    PERSISTENT_Y, RESET_Y,	NULL,					"pod2.jf5.intel.com"},
};

#define DRAWER_ATTR_ARG_DELCARE(valist) \
		memdb_integer *node_id = va_arg(*valist, memdb_integer *); \
		int drawer_lid = va_arg(*valist, int); \
		int dz_lid = va_arg(*valist, int);\
		int cm_lid = va_arg(*valist, int);\

static inline result_t get_drawer_id(char *name, char *value, int size, va_list *valist)
{
	DRAWER_ATTR_ARG_DELCARE(valist);
	snprintf(value, size, "%d", drawer_lid);
	return RESULT_OK;
}

#if 0
static inline result_t get_drawer_ip(char *name, char *value, int size, va_list *valist)
{
	DRAWER_ATTR_ARG_DELCARE(valist);
	assetd_get_drawer_ip(node_id, dz_idx, drawer_idx);
	return RESULT_SKIP;
}
#endif

static inline result_t get_drawer_name(char *name, char *value, int size, va_list *valist)
{
	DRAWER_ATTR_ARG_DELCARE(valist);
	snprintf(value, size, "RSA_drawer_%d", drawer_lid + (cm_lid - 1) * MAX_DRAWER_NUM);
	return RESULT_OK;
}


static memdb_tbl_t drawer_attr_memdb[] = {
	{DRAWER_LOC_ID_STR,				PERSISTENT_Y, RESET_N,	get_drawer_id,			{0} /*"1"*/},
	{DRAWER_UUID_STR,				PERSISTENT_N, RESET_N,	NULL,					{0} /*"25892e17-80f6-415f-9c65-7395632f0223"*/},
	{DRAWER_NAME_STR,				PERSISTENT_N, RESET_Y,	get_drawer_name,		{0}},
	{DRAWER_DESCRIPT_STR,			PERSISTENT_Y, RESET_Y,	NULL,					"for storage pool"},
	{DRAWER_CREATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{DRAWER_UPDATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{DRAWER_PUID_STR,				PERSISTENT_N, RESET_N,	NULL,					"1"},
	//{DRAWER_TMC_IP_STR,				PERSISTENT_N,	get_drawer_ip,		{0} /*"127.0.0.1"*/},
	{DRAWER_RUID_STR,				PERSISTENT_N, RESET_N,	NULL,					"drawer_ruid"},
	{DRAWER_PUID_STR,				PERSISTENT_N, RESET_N,	NULL,					"drawer_puid"},
	{DRAWER_X_LOC_STR,				PERSISTENT_N, RESET_N,	NULL,					"1"},
	{DRAWER_U_LOC_STR,				PERSISTENT_N, RESET_N,	NULL,					"5"},
	{DRAWER_U_HEIGHT_STR,			PERSISTENT_N, RESET_N,	NULL,					"2"},
	{DRAWER_PWR_CONSUM_STR,			PERSISTENT_N, RESET_N,	NULL,					"800"},
	{DRAWER_THERMAL_ST_STR,			PERSISTENT_N, RESET_N,	NULL,					"2"},
	{DRAWER_AV_RST_TYPE_1_STR,		PERSISTENT_N, RESET_N,	NULL,					"soft"},
	{DRAWER_AV_RST_TYPE_2_STR,		PERSISTENT_N, RESET_N,	NULL,					"hard"},
};

#define MBP_ARG_DELCARE(valist) \
	int cm_lid = va_arg(*valist, int)

static inline result_t get_mbp_id(char *name, char *value, int size, va_list *valist)
{
	MBP_ARG_DELCARE(valist);
	snprintf(value, size, "%d", cm_lid);
	return RESULT_OK;
}

static inline result_t get_mbpid(char *name, char *value, int size, va_list *valist)
{
	MBP_ARG_DELCARE(valist);
	snprintf(value, size, "%d", cm_lid);
	return RESULT_OK;
}

static inline result_t get_mbp_name(char *name, char *value, int size, va_list *valist)
{
	MBP_ARG_DELCARE(valist);
	snprintf(value, size, "RSA_mbp_%d", cm_lid);
	return RESULT_OK;
}

static memdb_tbl_t mbp_attr_memdb[] = {
	{MBP_LOC_ID_STR,			PERSISTENT_Y, RESET_N,	get_mbp_id,				{0} /*"1"*/},
	{MBP_UUID_STR,				PERSISTENT_N, RESET_N,	NULL,					{0} /*"25892e17-80f6-415f-9c65-7395632f0223"*/},
	{MBP_NAME_STR,				PERSISTENT_N, RESET_Y,	get_mbp_name,			"RSA_mbp_1"},
	{MBP_DESCRIPT_STR,			PERSISTENT_Y, RESET_Y,	NULL,					"for storage pool"},
	{MBP_CREATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{MBP_UPDATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{MBP_MBPID_STR,				PERSISTENT_Y, RESET_N,	get_mbpid,				{0} /*"192.168.10.2"*/},
	{MBP_HW_ADDR_STR,			PERSISTENT_N, RESET_N,	NULL,					"pod2.jf5.intel.com-1"},
/*  {MBP_IP_ADDR_STR,	 RESET_N,		PERSISTENT_N,	NULL,					"192.168.10.100"},*/
	{MBP_FW_STATE,				PERSISTENT_N, RESET_N,	NULL,					"up"},
	{MBP_X_LOC_STR,				PERSISTENT_N, RESET_N,	NULL,					"1"},
	{MBP_U_LOC_STR,				PERSISTENT_N, RESET_N,	NULL,					"5"},
	{MBP_U_HEIGHT_STR,			PERSISTENT_N, RESET_N,	NULL,					"11"},
	{MBP_ASSET_TAG_STR,			PERSISTENT_Y, RESET_Y,	NULL,					"1234567890"},
	{MBP_SER_NUM_STR,			PERSISTENT_N, RESET_N,	NULL,					"Intel"},
	{MBP_MANUFACT_STR,			PERSISTENT_N, RESET_N,	NULL,					"RSA_RMM"},
	{MBP_MODEL_STR,				PERSISTENT_N, RESET_N,	NULL,					"0987654321"},
	{MBP_PART_NUM_STR,			PERSISTENT_N, RESET_N,	NULL,					"0987654321"},
	{MBP_FW_VER_STR,			PERSISTENT_N, RESET_N,	NULL,					"v0.5"},
	{MBP_AV_RST_TYPE_1_STR,		PERSISTENT_N, RESET_N,	NULL,					"soft"},
	{MBP_AV_SET_UART_1_STR,		PERSISTENT_N, RESET_N,	NULL,					"115200,8n"},
	{MBP_AV_UPDATE_1_STR,		PERSISTENT_N, RESET_N,	NULL,					"URL"},
};

#define TZONE_ARG_DELCARE(valist) \
	int tzone_lid = va_arg(*valist, int);\
	int cm_lid = va_arg(*valist, int);\

static inline result_t get_tzone_id(char *name, char *value, int size, va_list *valist)
{
	TZONE_ARG_DELCARE(valist);
	snprintf(value, size, "%d", tzone_lid);
	return RESULT_OK;
}

static inline result_t get_tzone_name(char *name, char *value, int size, va_list *valist)
{
	TZONE_ARG_DELCARE(valist);
	snprintf(value, size, "tzone %d on cm %d", tzone_lid, cm_lid);
	return RESULT_OK;
}

static inline result_t get_tzone_descript(char *name, char *value, int size, va_list *valist)
{
	TZONE_ARG_DELCARE(valist);
	snprintf(value, size, "thermalzone %d on cm %d", tzone_lid, cm_lid);
	return RESULT_OK;
}

static memdb_tbl_t tzone_attr_memdb[] = {
	{TZONE_LOC_ID_STR,			PERSISTENT_Y, RESET_N,	get_tzone_id,			{0} /*"1"*/},
	{TZONE_UUID_STR,			PERSISTENT_N, RESET_N,	NULL,					{0} /*"25892e17-80f6-415f-9c65-7395632d0223"*/},
	{TZONE_NAME_STR,			PERSISTENT_N, RESET_Y,	get_tzone_name,			{0} /*"coolingzone 1"*/},
	{TZONE_DESCRIPT_STR,		PERSISTENT_Y, RESET_Y,	get_tzone_descript,		{0} /*"coolingzone 1"*/},
	{TZONE_CREATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{TZONE_UPDATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{TZONE_X_LOC_STR,			PERSISTENT_N, RESET_N,	NULL,					"1"},
	{TZONE_U_LOC_STR,			PERSISTENT_N, RESET_N,	NULL,					"5"},
	{TZONE_U_HEIGHT_STR,		PERSISTENT_N, RESET_N,	NULL,					"8"},
};

#define DZONE_ARG_DELCARE(valist) \
	int dzone_lid = va_arg(*valist, int);\
	int cm_lid = va_arg(*valist, int);\

static inline result_t get_dzone_id(char *name, char *value, int size, va_list *valist)
{
	DZONE_ARG_DELCARE(valist);
	snprintf(value, size, "%d", dzone_lid);
	return RESULT_OK;
}

static inline result_t get_dzone_name(char *name, char *value, int size, va_list *valist)
{
	DZONE_ARG_DELCARE(valist);
	snprintf(value, size, "dzone %d on cm %d", dzone_lid, cm_lid);
	return RESULT_OK;
}

static inline result_t get_dzone_descript(char *name, char *value, int size, va_list *valist)
{
	DZONE_ARG_DELCARE(valist);
	snprintf(value, size, "drawerzone %d on cm %d", dzone_lid, cm_lid);
	return RESULT_OK;
}

static memdb_tbl_t dzone_attr_memdb[] = {
	{DZONE_LOC_ID_STR,			PERSISTENT_Y, RESET_N,	get_dzone_id,			{0} /*"1"*/},
	{DZONE_UUID_STR,			PERSISTENT_N, RESET_N,	NULL,					{0} /*"25892e17-80f6-415f-9c65-7395632d0223"*/},
	{DZONE_NAME_STR,			PERSISTENT_N, RESET_Y,	get_dzone_name,			{0} /*"coolingzone 1"*/},
	{DZONE_DESCRIPT_STR,		PERSISTENT_Y, RESET_Y,	get_dzone_descript,		{0} /*"coolingzone 1"*/},
	{DZONE_CREATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{DZONE_UPDATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{DZONE_X_LOC_STR,			PERSISTENT_N, RESET_N,	NULL,					"0"},
	{DZONE_U_LOC_STR,			PERSISTENT_N, RESET_N,	NULL,					"1"},
	{DZONE_U_HEIGHT_STR,		PERSISTENT_N, RESET_N,	NULL,					"8"},
};

#define FAN_ARG_DELCARE(valist) \
		memdb_integer node_id = va_arg(*valist, memdb_integer); \
		int fan_lid = va_arg(*valist, int);\
		int zone_lid = va_arg(*valist, int);\
		int cm_lid = va_arg(*valist, int);\

static inline result_t get_fan_id(char *name, char *value, int size, va_list *valist)
{
	FAN_ARG_DELCARE(valist);
	snprintf(value, size, "%d", fan_lid);
	return RESULT_OK;
}

static inline result_t get_fan_name(char *name, char *value, int size, va_list *valist)
{
	FAN_ARG_DELCARE(valist);
	snprintf(value, size, "fan %d", fan_lid);
	return RESULT_OK;
}

static inline result_t get_fan_discript(char *name, char *value, int size, va_list *valist)
{
	FAN_ARG_DELCARE(valist);
	snprintf(value, size, "fan %d", fan_lid);
	return RESULT_OK;
}

static inline result_t get_fan_desired_spd_pwm(char *name, char *value, int size, va_list *valist)
{
	FAN_ARG_DELCARE(valist);
	snprintf(value, size, "%d", 40);
	return RESULT_OK;
}

static memdb_tbl_t fan_attr_memdb[] = {
	{FAN_LOC_ID_STR,			PERSISTENT_Y, RESET_N,	get_fan_id,				{0} /*"1"*/},
	{FAN_UUID_STR,				PERSISTENT_N, RESET_N,	NULL,					{0} /*"25892e17-80f6-415f-9c65-7395632e0232"*/},
	{FAN_NAME_STR,				PERSISTENT_Y, RESET_Y,	get_fan_name,			{0} /*"fan 1"*/},
	{FAN_DESCRIPT_STR,			PERSISTENT_Y, RESET_Y,	get_fan_discript,		{0} /*"fan 1"*/},
	{FAN_CREATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{FAN_UPDATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T02:11:11+08:00"*/},
	{FAN_ENABLE_STATE_STR,		PERSISTENT_Y, RESET_N,	NULL,					"2"},
	{FAN_HEALTH_STATE_STR,		PERSISTENT_N, RESET_N,	NULL,					"5"},
	{FAN_DESIRED_SPD_PWM_STR,	PERSISTENT_Y, RESET_N,	get_fan_desired_spd_pwm, {0} /*"40 PWM"*/},
	{FAN_TACH_READ_STR,			PERSISTENT_N, RESET_N,	NULL,					"5000"},
	{FAN_TACH_STATE_STR,		PERSISTENT_N, RESET_N,	NULL,					"normal"},
	{FAN_TACH_THRESH_LNC_STR,	PERSISTENT_Y, RESET_Y,	NULL,					"1000"},
	{FAN_TACH_THRESH_LC_STR,	PERSISTENT_Y, RESET_Y,	NULL,					"800"},
	{FAN_TACH_THRESH_UNC_STR,	PERSISTENT_Y, RESET_Y,	NULL,					"1500"},
	{FAN_TACH_THRESH_UC_STR,	PERSISTENT_Y, RESET_Y,	NULL,					"2000"},
	{FAN_TACH_PRESENT_STR,		PERSISTENT_N, RESET_Y,	NULL,					"present"},
	{FAN_ASSET_TAG_STR,			PERSISTENT_Y, RESET_Y,	NULL,					"4321"},
	{FAN_SER_NUM_STR,			PERSISTENT_N, RESET_N,	NULL,					"N/A"},
	{FAN_MANUFACT_STR,			PERSISTENT_N, RESET_N,	NULL,					"N/A"},
	{FAN_MODEL_STR,				PERSISTENT_N, RESET_N,	NULL,					"N/A"},
	{FAN_PART_NUM_STR,			PERSISTENT_N, RESET_N,	NULL,					"N/A"},
	{FAN_FW_VER_STR,			PERSISTENT_N, RESET_N,	NULL,					"N/A"},
	{FAN_AV_RSC_EN_VAL_1_STR,	PERSISTENT_N, RESET_N,	NULL,					"2"},
	{FAN_AV_RSC_EN_VAL_2_STR,	PERSISTENT_N, RESET_N,	NULL,					"3"},
	{FAN_AV_SDS_UNIT_1_STR,		PERSISTENT_N, RESET_N,	NULL,					"RPM"},
	{FAN_AV_SDS_UNIT_2_STR,		PERSISTENT_N, RESET_N,	NULL,					"PWM"},
	{FAN_AV_SDS_VAL_STR,		PERSISTENT_N, RESET_N,	NULL,					"40"},
	{FAN_X_LOC_STR,			    PERSISTENT_N, RESET_N,	NULL,					"1"},///fan1-0, fan2-1, fan3-2, fan4-0, fan5-1, fan6-2
	{FAN_U_LOC_STR,			    PERSISTENT_N, RESET_N,	NULL,				    "5"},
	{FAN_U_HEIGHT_STR,		    PERSISTENT_N, RESET_N,	NULL,					"4"},
};

#define PZONE_ARG_DELCARE(valist) \
	int pzone_lid = va_arg(*valist, int);\
	int cm_lid = va_arg(*valist, int);\

static inline result_t get_pzone_id(char *key, char *value, int size, va_list *valist)
{
	PZONE_ARG_DELCARE(valist);
	snprintf(value, size, "%d", pzone_lid);
	return RESULT_OK;
}

static inline result_t get_pzone_name(char *key, char *value, int size, va_list *valist)
{
	PZONE_ARG_DELCARE(valist);
	snprintf(value, size, "pzone %d on cm %d", pzone_lid, cm_lid);
	return RESULT_OK;
}

static inline result_t get_pzone_discript(char *key, char *value, int size, va_list *valist)
{
	PZONE_ARG_DELCARE(valist);
	snprintf(value, size, "powerzone %d on cm %d", pzone_lid, cm_lid);
	return RESULT_OK;
}

static memdb_tbl_t pzone_attr_memdb[] = {
	{PZONE_LOC_ID_STR,			PERSISTENT_Y, RESET_N,	get_pzone_id,			{0} /*"1"*/},
	{PZONE_UUID_STR,			PERSISTENT_N, RESET_N,	NULL,					{0} /*"25892e17-80f6-415f-9c65-7395632d0223"*/},
	{PZONE_NAME_STR,			PERSISTENT_N, RESET_Y,	get_pzone_name,			{0} /*"powerzone 1"*/},
	{PZONE_DESCRIPT_STR,		PERSISTENT_Y, RESET_Y,	get_pzone_discript,		{0} /*"powerzone 1"*/},
	{PZONE_CREATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{PZONE_UPDATE_DATE_STR,		PERSISTENT_N, RESET_N,	get_time,				{0} /*"2014-09-11T01:15:16+08:00"*/},
	{PZONE_X_LOC_STR,			PERSISTENT_N, RESET_N,	NULL,					"1"},
	{PZONE_U_LOC_STR,			PERSISTENT_N, RESET_N,	NULL,					"13"},
	{PZONE_U_HEIGHT_STR,		PERSISTENT_N, RESET_N,	NULL,					"3"},
	{PZONE_TT_PWR_CONSUM_STR,	PERSISTENT_N, RESET_N,	NULL,					"2700"},
	{PZONE_TT_PWR_CAP_STR,		PERSISTENT_N, RESET_N,	NULL,					"3200"},
	{PZONE_TT_PWR_PROD_STR,		PERSISTENT_N, RESET_N,	NULL,					"3200"},
};

#define PSU_ARG_DELCARE(valist) \
		memdb_integer * node_id  = va_arg(*valist, memdb_integer*); \
		int psu_lid = va_arg(*valist, int); \
		int pzone_lid = va_arg(*valist, int);\
		int cm_lid = va_arg(*valist, int);\

static inline result_t get_psu_id(char *key, char *value, int size, va_list *valist)
{
	PSU_ARG_DELCARE(valist);
	snprintf(value, size, "%d", psu_lid);
	return RESULT_OK;
}

static inline result_t get_psu_name(char *key, char *value,  int size, va_list *valist)
{
	PSU_ARG_DELCARE(valist);
	snprintf(value, size, "psu %d", psu_lid);
	return RESULT_OK;
}

static inline result_t get_psu_discript(char *key, char *value, int size, va_list *valist)
{
	PSU_ARG_DELCARE(valist);
	snprintf(value, size, "psu %d in zone %d", psu_lid, pzone_lid);
	return RESULT_OK;
}

static memdb_tbl_t psu_attr_memdb[] = {
	{PSU_LOC_ID_STR,		PERSISTENT_Y, RESET_N,		get_psu_id,			{0} /*"1"*/},
	{PSU_UUID_STR,			PERSISTENT_N, RESET_N,		NULL,				{0} /*"25892e17-80f6-415f-9c65-7395632d0223"*/},
	{PSU_NAME_STR,			PERSISTENT_N, RESET_Y,		get_psu_name,		{0} /*"PSU 1"*/},
	{PSU_DESCRIPT_STR,		PERSISTENT_Y, RESET_Y,		get_psu_discript,	{0} /*"PSU 1 in zone 1"*/},
	{PSU_CREATE_DATE_STR,	PERSISTENT_N, RESET_N,		get_time,			{0} /*"2014-09-11T01:15:16+08:00"*/},
	{PSU_UPDATE_DATE_STR,	PERSISTENT_N, RESET_N,		get_time,			{0} /*"2014-09-11T01:15:16+08:00"*/},
	{PSU_ENABLE_ST_STR,		PERSISTENT_Y, RESET_N,		NULL,				"2"},
	{PSU_REDUND_SET_STR,	PERSISTENT_Y, RESET_N,		NULL,				"N/A"},
	{PSU_HEALTH_ST_STR,		PERSISTENT_N, RESET_N,		NULL,				"5"},
	{PSU_TT_PWR_IN_STR,		PERSISTENT_N, RESET_N,		NULL,				"0"},
	{PSU_TT_PWR_CAP_STR,	PERSISTENT_N, RESET_N,		NULL,				"350"},
	{PSU_TT_CURRENT_OUT_STR,PERSISTENT_N, RESET_N,		NULL,				"0"},
	{PSU_ASSET_TAG_STR,		PERSISTENT_Y, RESET_Y,		NULL,				"1234567890"},
	{PSU_SER_NUM_STR,		PERSISTENT_N, RESET_N,		NULL,				{0} /*"233-444-5552"*/},
	{PSU_MANUFACT_STR,		PERSISTENT_N, RESET_N,		NULL,				{0} /*"Intel"*/},
	{PSU_MODEL_STR,			PERSISTENT_N, RESET_N,		NULL,				{0} /*"4488543"*/},
	{PSU_PART_NUM_STR,		PERSISTENT_N, RESET_N,		NULL,				"123"},
	{PSU_FW_VER_STR,		PERSISTENT_N, RESET_N,		NULL,				{0} /*"1.0.1"*/},
	{PSU_AV_REQ_ST_1_STR,	PERSISTENT_N, RESET_N,		NULL,				"2"},
	{PSU_AV_REQ_ST_2_STR,	PERSISTENT_N, RESET_N,		NULL,				"3"},
	{PSU_AV_REQ_ST_3_STR,	PERSISTENT_N, RESET_N,		NULL,				"6"},
	{PSU_X_LOC_STR,			PERSISTENT_N, RESET_N,	    NULL,				"1"},
	{PSU_U_LOC_STR,			PERSISTENT_N, RESET_N,	    NULL,				"13"},
	{PSU_U_HEIGHT_STR,		PERSISTENT_N, RESET_N,	    NULL,				"3"},
};

static int init_attr(memdb_tbl_t *tb1, int size, memdb_integer node_id, int snap, ...)
{
	int i = 0;
	int rc = 0;
	result_t ret = RESULT_OK;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	va_list args;
	int length = 0;
	int64 error_code = 0;

	for (i = 0; i < size; i++) {
		ret = RESULT_OK;
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);

		error_code = libdb_attr_get_string(DB_RMM, node_id, tb1[i].key, value, WRAP_DB_MAX_VALUE_LEN, LOCK_ID_NULL);
		if (error_code != 0) {
			rmm_log(DBG, "%s:%d: error code:%d\n", __func__, __LINE__, error_code);
		}

		if (strlen(value) == 0) {
			/** PERSISTANT_Y: load all attribute
			* PERSISTANT_N: load non-persistant attribute only
			*/
			if (snap & tb1[i].snapshot_flag) {
				if (tb1[i].get_fn != NULL) {
					va_start(args, snap);
					ret = tb1[i].get_fn(tb1[i].key, value , WRAP_DB_MAX_VALUE_LEN, &args);
					va_end(args);
				} else {
					length = strlen(tb1[i].value);
					if (length >= WRAP_DB_MAX_VALUE_LEN)
						length = WRAP_DB_MAX_VALUE_LEN - 1;
					memcpy(value, tb1[i].value, length);
				}

				if (ret == RESULT_OK)
					if (libdb_attr_set_string(DB_RMM, node_id,
										tb1[i].key, 0x0, value,
										(tb1[i].snapshot_flag>>1), LOCK_ID_NULL) == -1) {
						rmm_log(ERROR, "db set attr %s fail\n", tb1[i].key);
						return -1;
					}
			}
		}
	}

	return 0;
}

static void set_color_tag(memdb_tbl_t *tbl, int size)
{
	int index = 0;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};

	for (index = 0; index < size; index++) {
		memcpy(value, tbl[index].value, WRAP_DB_MAX_VALUE_LEN);
		snprintf(tbl[index].value, WRAP_DB_MAX_VALUE_LEN, "%s%s", HARD_CODE_FLAG, value);
	}
}

void add_color_tag(void)
{
	set_color_tag(rack_attr_memdb,	sizeof(rack_attr_memdb)/sizeof(memdb_tbl_t));
	set_color_tag(mbp_attr_memdb,	sizeof(mbp_attr_memdb)/sizeof(memdb_tbl_t));
	set_color_tag(drawer_attr_memdb,	sizeof(drawer_attr_memdb)/sizeof(memdb_tbl_t));
	set_color_tag(tzone_attr_memdb,	sizeof(tzone_attr_memdb)/sizeof(memdb_tbl_t));
	set_color_tag(fan_attr_memdb,	sizeof(fan_attr_memdb)/sizeof(memdb_tbl_t));
	set_color_tag(pzone_attr_memdb,	sizeof(pzone_attr_memdb)/sizeof(memdb_tbl_t));
	set_color_tag(psu_attr_memdb,	sizeof(psu_attr_memdb)/sizeof(memdb_tbl_t));
}

void init_rack_attr(void)
{
	char ip_addr[WRAP_DB_MAX_VALUE_LEN] = {0};

	if (libutils_get_ip((char *)ip_addr) < 0) {
		memset(ip_addr, 0, WRAP_DB_MAX_VALUE_LEN);
		strncpy_safe(ip_addr, "x", sizeof(ip_addr), 1);
	}
	init_attr(rack_attr_memdb, sizeof(rack_attr_memdb)/sizeof(memdb_tbl_t), MC_TYPE_RMC, PERSISTENT_ALL, ip_addr);
}

void init_pzone_attr(const memdb_integer *node_id, int pzone_lid, int cm_lid, int snap)
{
	init_attr(pzone_attr_memdb, sizeof(pzone_attr_memdb)/sizeof(memdb_tbl_t), *node_id, snap, pzone_lid, cm_lid);
}

void init_tzone_attr(const memdb_integer *node_id, int tzone_lid, int cm_lid, int snap)
{
	init_attr(tzone_attr_memdb, sizeof(tzone_attr_memdb)/sizeof(memdb_tbl_t), *node_id, snap, tzone_lid, cm_lid);
}

void init_dzone_attr(const memdb_integer *node_id, int dzone_lid, int cm_lid, int snap)
{
	init_attr(dzone_attr_memdb, sizeof(dzone_attr_memdb)/sizeof(memdb_tbl_t), *node_id, snap, dzone_lid, cm_lid);
}

void init_fan_attr(const memdb_integer *node_id, int fan_lid, int tzone_lid, int cm_lid, int snap)
{
	init_attr(fan_attr_memdb, sizeof(fan_attr_memdb)/sizeof(memdb_tbl_t), *node_id, snap, *node_id, fan_lid, tzone_lid, cm_lid);
}

void init_psu_attr(const memdb_integer *node_id, int psu_lid, int pzone_lid, int cm_lid, int snap)
{
	init_attr(psu_attr_memdb, sizeof(psu_attr_memdb)/sizeof(memdb_tbl_t), *node_id, snap, node_id, psu_lid, pzone_lid, cm_lid);
}

void init_mbp_attr(const memdb_integer *node_id, int cm_lid, int snap)
{
	init_attr(mbp_attr_memdb, sizeof(mbp_attr_memdb)/sizeof(memdb_tbl_t), *node_id, snap, cm_lid);
}

void init_drawer_attr(const memdb_integer *node_id, int drawer_lid, int dz_lid, int cm_lid, int snap)
{
	init_attr(drawer_attr_memdb, sizeof(drawer_attr_memdb)/sizeof(memdb_tbl_t), *node_id, snap, node_id, drawer_lid, dz_lid, cm_lid);
}

static int reset_attr(memdb_tbl_t *tb1, int size, memdb_integer node_id, ...)
{
	int i = 0;
	result_t ret = RESULT_OK;
	char value[WRAP_DB_MAX_VALUE_LEN] = {0};
	va_list args;
	int length = 0;
	int64 error_code = 0;

	for (i = 0; i < size; i++) {
		if(tb1[i].reset_flag == RESET_N)
			continue;
		ret = RESULT_OK;
		memset(value, 0, WRAP_DB_MAX_VALUE_LEN);

		if (tb1[i].get_fn != NULL) {
			va_start(args, node_id);
			ret = tb1[i].get_fn(tb1[i].key, value , WRAP_DB_MAX_VALUE_LEN, &args);
			va_end(args);
		} else {
			length = strlen(tb1[i].value);
			if (length >= WRAP_DB_MAX_VALUE_LEN)
				length = WRAP_DB_MAX_VALUE_LEN - 1;
			memcpy(value, tb1[i].value, length);
		}

		if (ret == RESULT_OK)
			if (libdb_attr_set_string(DB_RMM, node_id,
								tb1[i].key, 0x0, value,
								(tb1[i].snapshot_flag>>1), LOCK_ID_NULL) == -1) {
				rmm_log(ERROR, "memdb set %s attr failed\n", tb1[i].key);
				return -1;
			}
	}

	return 0;
}

void reset_rack_attr(void)
{
	char ip_addr[WRAP_DB_MAX_VALUE_LEN] = {0};

	if (libutils_get_ip((char *)ip_addr) < 0) {
		memset(ip_addr, 0, WRAP_DB_MAX_VALUE_LEN);
		strncpy_safe(ip_addr, "x", sizeof(ip_addr), 1);
	}
	reset_attr(rack_attr_memdb, sizeof(rack_attr_memdb)/sizeof(memdb_tbl_t), MC_TYPE_RMC, ip_addr);
}

void reset_pzone_attr(const memdb_integer *node_id, int cm_lid, int pzone_lid)
{
	reset_attr(pzone_attr_memdb, sizeof(pzone_attr_memdb)/sizeof(memdb_tbl_t), *node_id, cm_lid, pzone_lid);
}

void reset_tzone_attr(const memdb_integer *node_id, int cm_lid, int tzone_lid)
{
	reset_attr(tzone_attr_memdb, sizeof(tzone_attr_memdb)/sizeof(memdb_tbl_t), *node_id, cm_lid, tzone_lid);
}

void reset_dzone_attr(const memdb_integer *node_id, int cm_lid, int dzone_lid)
{
	reset_attr(dzone_attr_memdb, sizeof(dzone_attr_memdb)/sizeof(memdb_tbl_t), *node_id, cm_lid, dzone_lid);
}

void reset_fan_attr(const memdb_integer *node_id, int cm_lid, int fan_lid, int tzone_lid)
{
	reset_attr(fan_attr_memdb, sizeof(fan_attr_memdb)/sizeof(memdb_tbl_t), *node_id, cm_lid, fan_lid, tzone_lid);
}

void reset_psu_attr(const memdb_integer *node_id, int cm_lid, int psu_lid, int pzone_lid)
{
	reset_attr(psu_attr_memdb, sizeof(psu_attr_memdb)/sizeof(memdb_tbl_t), *node_id, cm_lid, psu_lid, pzone_lid);
}

void reset_mbp_attr(const memdb_integer *node_id, int cm_lid)
{
	reset_attr(mbp_attr_memdb, sizeof(mbp_attr_memdb)/sizeof(memdb_tbl_t), *node_id, cm_lid);
}

void reset_drawer_attr(const memdb_integer *node_id, int cm_lid, int drawer_lid, int dz_lid)
{
	reset_attr(drawer_attr_memdb, sizeof(drawer_attr_memdb)/sizeof(memdb_tbl_t), *node_id, cm_lid, drawer_lid, dz_lid);
}
