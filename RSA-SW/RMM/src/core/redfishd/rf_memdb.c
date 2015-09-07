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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libjsonrpcapi/libjsonrpcapi.h"
#include "libutils/rack.h"
#include "libredfish/rf_types.h"
#include "libredfish/msg_reg.h"
#include "libredfish/mr_def.h"
#include "rf_memdb.h"
#include "librmmlog/rmmlog.h"

/**
 * @brief   max event action of each event source.
 *
 */
#define MAX_EVT_ACTION_NUM	5
#define MAX_EVT_SOURCE_NUM	10
#define MAX_RF_EVT_NUM		(MAX_EVT_ACTION_NUM*MAX_EVT_SOURCE_NUM)

typedef rf_node_msg_id_map_t		*p_rf_node_msg_id_map;
static rf_node_msg_id_map_t			g_rf_node_msg_id_map[MAX_RF_EVT_NUM];
static int32						node_msg_id_map_size;

/**
* @brief event source discription.
*
*/
struct evt_source_info {
	int8 prefix_msg[16];					//!< prefix messgae about the msg id str
	int32 node_id;							//!< node id for event source
	int32 is_evt_exist[MAX_EVT_ACTION_NUM];	//!< Initialize action for event source(such as change, add, remove and so on.).
};

/**
 * @brief map of msg tail and node id.
 *
 */
const id_evt_type_t rf_evt_msg[] = {
	{MC_REDFISH_CHANGE, "Change"},
	{MC_REDFISH_UPDATE, "Update"},
	{MC_REDFISH_ADD,    "Add"},
	{MC_REDFISH_REMOVE, "Remove"},
	{MC_REDFISH_ALERT,  "Alert"},
};

/**
 * @brief discription for each event source (such as rack, pzone, czone and so on).
 *
 */
struct evt_source_info evt_source_infos[] = {
		/* RACK node stands for RMM */				/*change, update, add, remove, alert */
		{ "MSGRack",	MC_REDFISH_RACK,			{1,       1,     0,      0,     0} },
		/* PZONE: Power zone */
		{ "MSGPZone",	MC_REDFISH_PZONE,			{1,       1,     1,      1,     0} },
		{ "MSGPsu",		MC_REDFISH_PSU_COLL,		{1,       1,     1,      1,     0} },

		/* TZONE: Thermal zone as known Cooling zone*/
		{ "MSGTZone",	MC_REDFISH_TZONE,			{1,       1,     1,      1,     0} },
		{ "MSGFan",		MC_REDFISH_FAN_COLL,		{1,       1,     1,      1,     0} },

		/* DRAWER: as known as Tray, sys,or Chassis.*/
		{ "MSGDrawer",	MC_REDFISH_DRAWER_COLL,		{1,       1,     1,      1,     1} },

		/* MBP: mangement backplane, as known as CM. */
		{ "MSGMbp",		MC_REDFISH_MBP_COLL,		{1,       1,     1,      1,     0} }
};

static memdb_integer get_rf_node_id(void)
{
	int32 output = 0;
	struct node_info *subnode = NULL;
	int32 subnode_num = 0;
	int32 retry = 3;
	memdb_integer node_id = 0;

	while (retry--) {
		subnode = libdb_list_node_by_type(DB_RMM, MC_REDFISH, MC_REDFISH, &subnode_num, NULL, LOCK_ID_NULL);
		if (subnode)
			break;
		sleep(1);
	}

	if (subnode_num != 1  || subnode == NULL) {
		libdb_free_node(subnode);
		rmm_log(ERROR, "failed to get subnode\n");
		return 0;
	}

	node_id = subnode[0].node_id;
	libdb_free_node(subnode);

	return node_id;
}

static void get_evt_action_msg_id_str(int8 *msg_id_str, const int8 *msg_prefix, const int8 *evt)
{
	snprintf(msg_id_str, MSG_ID_STR_LEN, "%s%s%s%s", RF_PRE_FIX_INFO, RF_PRE_VER_INFO, msg_prefix, evt);
}

/**
 * @brief Generate redfish event map cache when redfishd initialize.
 *
 */
static int32 gen_node_msg_id_map(p_rf_node_msg_id_map evt_types)
{
	int32 i = 0, j = 0;
	int32 nodenum = 0;
	int32 index = 0;
	int32 evt_source_num = 0;
	struct node_info *evt_source_nodeinfo = NULL, *rf_node = NULL;
	int32 action_num = 0;
	struct node_info *action_node_info;

	/*Get redfish event nid*/
	rf_node = libdb_list_node_by_type(DB_RMM, MC_REDFISH_EVENT, MC_REDFISH_EVENT, &nodenum, NULL, LOCK_ID_NULL);
	if (nodenum == 0)
		return 0;

	evt_source_nodeinfo = libdb_list_subnode(DB_RMM, rf_node[0].node_id, &evt_source_num, NULL, LOCK_ID_NULL);
	if (evt_source_num == 0)
		return 0;

	/*traverse all of event sources*/
	for (i = 0; i < evt_source_num; i++) {
		action_node_info = libdb_list_subnode(DB_RMM, evt_source_nodeinfo[i].node_id, &action_num, NULL, LOCK_ID_NULL);
		for (j = 0; j < action_num; j++) {
			/*Load event type information.*/
			evt_types[index].node_id = action_node_info[j].node_id;
			libdb_attr_get_string(DB_RMM, action_node_info[j].node_id, MSG_ID_KEY, evt_types[index].msg_id_str, MSG_ID_STR_LEN, LOCK_ID_NULL);
			index += 1;
		}
	}

	/*The index is total of event types count.*/
	return index;
}

/**
 * @brief Create event type node for each event(such as change, alert and so on).
 *
 */
static void create_rf_event_action_node(memdb_integer parent, uint32 type, int8 *msg_id_str)
{
	memdb_integer evt_type_node = libdb_create_node(DB_RMM, parent, type, SNAPSHOT_NEED, LOCK_ID_NULL);
	int32 rc = 0;

	rc = libdb_attr_set_string(DB_RMM, evt_type_node, MSG_ID_KEY, 0, msg_id_str, SNAPSHOT_NEED, LOCK_ID_NULL);
	if (rc == -1) {
		rmm_log(ERROR, "memdb set %s fail\n", msg_id_str);
	}
}

/**
 * @brief Create event source node for redfish.
 *
 */
static void create_rf_event_source_node(memdb_integer parent, struct evt_source_info info)
{
	memdb_integer nid = libdb_create_node(DB_RMM, parent, info.node_id, SNAPSHOT_NEED, LOCK_ID_NULL);
	int8 msg_id_str[MSG_ID_STR_LEN] = {0};
	int32 i = 0;

	for (i = 0;  i < MAX_EVT_ACTION_NUM; i++) {
		if (!info.is_evt_exist[i])
			continue;

		memset(msg_id_str, 0, MSG_ID_STR_LEN);
		get_evt_action_msg_id_str(msg_id_str, info.prefix_msg, rf_evt_msg[i].type_name);
		create_rf_event_action_node(nid, rf_evt_msg[i].node_id, msg_id_str);
	}
}

static memdb_integer create_memdb_rf_event_nodes(void)
{
	memdb_integer node_id = libdb_create_node(DB_RMM, MC_TYPE_RMC, MC_REDFISH, SNAPSHOT_NEED, LOCK_ID_NULL);
	int32 i = 0;

	node_id = libdb_create_node(DB_RMM, node_id, MC_REDFISH_EVENT, SNAPSHOT_NEED, LOCK_ID_NULL);
	for (i = 0; i < sizeof(evt_source_infos) / sizeof(struct evt_source_info); i++)
		create_rf_event_source_node(node_id, evt_source_infos[i]);
	return node_id;
}

static void get_listeners_by_nid(memdb_integer nid, int location_idx, struct dest_info *listeners)
{
	struct node_info *subnode = NULL;
	int32 subnode_num = 0;
	int32 mask = 0;
	int32 lds_idx = 0;
	int32 i = 0;
	struct dest_info *tmp;
	int32 is_listen_validate_flag = 1;
	int64 error_code = 0;

	subnode = libdb_list_subnode_by_type(DB_RMM, nid, MC_REDFISH_LISTENER, &subnode_num, NULL, LOCK_ID_NULL);
	for (i = 0; i < subnode_num; i++) {
		if (location_idx != INVAILD_IDX) {
			error_code = libdb_attr_get_int(DB_RMM, subnode[i].node_id, RF_EVENT_LISTENER_INDEXES_STR, &mask, LOCK_ID_NULL);
			if (error_code != 0)
				continue;
			if (location_idx < 1)
				continue;
			if ((mask & (1<<(location_idx - 1))) == 0)
				is_listen_validate_flag = 0;
			else
				is_listen_validate_flag = 1;
		} else
			is_listen_validate_flag = 1;

		if (is_listen_validate_flag == 1) {
			error_code = libdb_attr_get_string(DB_RMM, subnode[i].node_id, RF_EVENT_LISTENER_DEST_STR, listeners->dest, 256, LOCK_ID_NULL);
			if (error_code == 0) {
				tmp = (struct dest_info *)malloc(sizeof(struct dest_info));
				if (tmp == NULL)
					continue;
				listeners->pnext = tmp;
				listeners = tmp;
				listeners->pnext = NULL;
			}
		}
	}
}

void rf_memdb_event_node_init(void)
{
	libjsonrpcapi_init(JSONRPCINIT_MEMDB, 0);
	libdb_is_ready(DB_RMM, LOCK_ID_NULL, -1);
	memdb_integer nid = get_rf_node_id();

	if (nid == 0) {
		create_memdb_rf_event_nodes();
	}

	node_msg_id_map_size = gen_node_msg_id_map(g_rf_node_msg_id_map);
}

int32 rf_memdb_get_listeners(int8 *msg_id_str, int32 location_idx, struct dest_info *listener)
{
	int32 i = 0;

	for (i = 0; i < node_msg_id_map_size; i++) {

		if (memcmp(msg_id_str, g_rf_node_msg_id_map[i].msg_id_str, MSG_ID_STR_LEN) == 0) {
			get_listeners_by_nid(g_rf_node_msg_id_map[i].node_id, location_idx, listener);
			break;
		}
	}
	return 0;
}