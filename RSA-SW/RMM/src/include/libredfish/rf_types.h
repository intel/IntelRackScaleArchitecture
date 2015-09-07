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


#ifndef __LIBUTILS_RF_TYPE_H__
#define __LIBUTILS_RF_TYPE_H__

#define RF_MSG_MAX_LEN					256
#define MAX_MODULE_NAME_LEN				128
#define MAX_FUNC_NAME_LEN				128
#define BUF_SIZE						25600
#define MAX_RF_EVT_MSG_LOG_COUNT		1024
#define MAX_IP_LEN						64

#define RF_EVENT_TYPE_ADD_STR			"add"
#define RF_EVENT_TYPE_DELETE_STR		"remove"
#define RF_EVENT_TYPE_UPDATE_STR		"update"
#define RF_EVENT_TYPE_CHANGE_STR		"change"
#define RF_EVENT_TYPE_ALERT_STR			"alert"

#define RF_EVENT_LISTENER_DEST_STR		"dest"
#define RF_EVENT_LISTENER_TYPE_STR		"type"
#define RF_EVENT_LISTENER_PROTOCOL_STR	"protocol"
#define RF_EVENT_LISTENER_INDEXES_STR	"indexes"

#define RF_EVENT_TYPE_ADD_BIT			0x01
#define RF_EVENT_TYPE_DELETE_BIT		(RF_EVENT_TYPE_ADD_BIT << 1)
#define RF_EVENT_TYPE_UPDATE_BIT		(RF_EVENT_TYPE_ADD_BIT << 2)
#define RF_EVENT_TYPE_CHANGE_BIT		(RF_EVENT_TYPE_ADD_BIT << 3)
#define RF_EVENT_TYPE_ALERT_BIT			(RF_EVENT_TYPE_ADD_BIT << 4)

#define RF_EVENT_MAX_MSG_ARG_NUM		8
#define RF_EVENT_MAX_CAP_NUM			4
#define RF_EVENT_MAX_TYPE_NUM			5
#define RF_EVENT_MAX_LISTENER_NUM		16

#define RF_EVENT_MASK_ALL				0xFFFFFFFF

#define RF_EVENT_URL_FMT_HDR			"http://%s:%d"
#define RF_EVENT_RACK_FMT				"/rack/event"
#define RF_EVENT_DRAWER_COLL_FMT		"/rack/drawers/event"
#define RF_EVENT_DRAWER_FMT				"/rack/drawers/%s/event"
#define RF_EVENT_MBP_COLL_FMT			"/rack/mbps/event"
#define RF_EVENT_MBP_FMT				"/rack/mbps/%d/event"
#define RF_EVENT_PZONE_COLL_FMT			"/rack/powerzones/event"
#define RF_EVENT_PZONE_FMT				"/rack/powerzones/%d/event"
#define RF_EVENT_PSU_COLL_FMT			"/rack/powerzones/%d/psus/event"
#define RF_EVENT_PSU_FMT				"/rack/powerzones/%d/psus/%d/event"
#define RF_EVENT_CZONE_COLL_FMT			"/rack/thermalzones/event"
#define RF_EVENT_CZONE_FMT				"/rack/thermalzones/%d/event"
#define RF_EVENT_FAN_COLL_FMT			"/rack/thermalzones/%d/fans/event"
#define RF_EVENT_FAN_FMT				"/rack/thermalzones/%d/fans/%d/event"

union ip{
	char c[4]; 
	long int src;
} IP;

/*redfish log define*/
enum RF_EVT {
	RF_MSG_REQ			= 0,
	RF_GET_LOG_BY_LIMIT	= 1,
	RF_SET_TRAP_IP		= 2
};

enum SNMP_TRAP
{
	SNMP_TRAP_ADD 		= 0,
	SNMP_TRAP_REMOVE	= 1
};

struct rf_log_req_info {
	int		type;
	union {
		struct {
			int		msg_sn;
			int		is_event;
			char	str_args[RF_MSG_MAX_LEN];
			int		level;
			char	module_name[MAX_MODULE_NAME_LEN];
			char	func_name[MAX_FUNC_NAME_LEN];
		} fmt1;
		struct {
			int count;
		} fmt2;
		struct {
			int is_remove;
			char ip[MAX_IP_LEN];
			int port;
		} fmt3;
	} data;
};

typedef struct rf_log_desc {
	int count;
	int length;
} rf_log_dsc_t;

typedef struct rf_msg_info {
	rf_log_dsc_t dsc;
	char data[RF_MSG_MAX_LEN * MAX_RF_EVT_MSG_LOG_COUNT];
} rf_msg_info_t;
/*redfish log define end*/

typedef struct listener_dest {
	int		idx_mask;
	char	type[32];
	char	dest[128];
	char	protocol[32];
} listener_dest_t;

typedef struct rf_base {
	char oem[64];
	char name[64];
	char modify_date[128];
	char type[32];
	char description[256];
} rf_base_t;

typedef struct rf_status {
	char state[32];
	char health_rollup[16];
	char health[16];
	char oem[32];
} rf_status_t;

typedef struct rf_link {
	char subscription[RF_EVENT_MAX_LISTENER_NUM][256];
	char self[256];
} rf_link_t;

typedef struct rf_capability {
	char event_type[32];
	int  event_id;
	char event_timestamp[128];
	char severity[32];
	char message[256];
	int  msg_id;
	char msg_args[RF_EVENT_MAX_MSG_ARG_NUM][32];
	char origin_cond[64];
} rf_capability_t;

typedef struct rf_av_action {
	char action[64];
	rf_capability_t cap[RF_EVENT_MAX_CAP_NUM];
} rf_av_action_t;

typedef struct rf_evt_svc {
	rf_base_t   base;
	rf_link_t       links;
/*	rf_status_t     status;             */
/*	int             retry_attemp;       */
/*	int             retry_interval;     */
/*	char            removal_action[32]; */
/*	int             removal_interval;   */
	char            event_type[RF_EVENT_MAX_TYPE_NUM][32];
/*	rf_av_action_t  av_action; */
} rf_evt_svc_t;

typedef struct rf_log_svc {
	rf_base_t   base;
	char        self_link[256];
	int         MaxNumberOfRecords;
	char        OverWritePolicy[32];
} rf_log_svc_t;

typedef struct id_evt_type {
	int		node_id;
	char	type_name[64];
} id_evt_type_t;

#endif
