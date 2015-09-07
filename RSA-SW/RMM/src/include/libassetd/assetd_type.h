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


#ifndef __ASSET_DIRVER_TYPE_H__
#define __ASSET_DIRVER_TYPE_H__

#include <stdio.h>
#include "libjsonrpc/jsonrpc_err.h"
#include "libjsonrpcapi/libjsonrpcapi.h"


enum AD_EVT_TYPE {
	REG_APP,
	REQ_QUERY,
	ADD_EVT,
	REMOVE_EVT,
	CHANGE_EVT,
	DO_ACTION,
	#if 0
	GET_PSU_SERIAL,
	GET_PSU_MANUFACTURE,
	GET_PSU_MODEL,
	GET_PSU_FW_VER,
	GET_FAN_PWM_BY_ID,
	SET_FAN_PWM,
	RESET_HARD_RESET,
	POST_PSU_BY_NODE_ID,
	SET_ID_FIELD,
	GET_ID_FIELD,
	GET_THERMAL_SENSOR,
	GET_HEALTH_SENSOR,
	GET_TRAY_POWER,
	#endif
	RMM_FACTORY_RESET,
	UART_SWITCH,
	MAX_EVT
};

static cmd_map_t am_cmd_map[] = {
			{REG_APP, "on_register"},
			{REQ_QUERY, "on_reg_query"},
			{ADD_EVT, "on_added"},
			{REMOVE_EVT, "on_removed"},
			{CHANGE_EVT, "on_changed"},
			{DO_ACTION, "do_action"},
			#if 0
			{GET_FAN_PWM_BY_ID, "get_fan_pwm_by_id"},
			{SET_FAN_PWM, "set_fan_pwm"},
			{RESET_HARD_RESET, "hard_reset"},
			{POST_PSU_BY_NODE_ID, "post_psu_by_node_id"},
			{SET_ID_FIELD, "set_id_field"},
			{GET_ID_FIELD, "get_id_field"},
			{GET_THERMAL_SENSOR, "get_thermal_sensor"},
			{GET_HEALTH_SENSOR, "get_health_sensor"},
			{GET_TRAY_POWER, "get_tray_power"},
			#endif
			{UART_SWITCH, "uart_switch"},
			{RMM_FACTORY_RESET, "rmm_factory_reset"}
			};

enum MODULE_TYPE {
	CM_TYPE,
	PZ_TYPE,
	TZ_TYPE,
	DZ_TYPE,
	PSU_TYPE,
	FAN_TYPE,
	DRAWER_TYPE,
	MAX_TYPE,
};


static char *module_name[MAX_TYPE] = {
	[CM_TYPE]   = "CM",
	[PZ_TYPE]   = "PZone",
	[TZ_TYPE]   = "TZone",
	[DZ_TYPE]   = "DZone",
	[PSU_TYPE]  = "PSU",
	[FAN_TYPE]  = "FAN",
	[DRAWER_TYPE]  = "DRAWER"
};

#endif
