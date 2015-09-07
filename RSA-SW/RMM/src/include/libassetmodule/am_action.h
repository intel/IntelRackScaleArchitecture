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


#ifndef __LIB_ASSET_MODULE_ACTION_H__
#define __LIB_ASSET_MODULE_ACTION_H__

#include <stdio.h>
#include "libjsonrpc/jsonrpc_err.h"
#include "libjsonrpcapi/libjsonrpcapi.h"


typedef struct _action_st {
	int  action_id;
	char *action_name;
} action_map_t;

enum AM_ACTIONS {
	AMACTION_GET_FAN_PWM_BY_ID,
	AMACTION_SET_FAN_PWM,
	AMACTION_RESET_HARD_RESET,
	AMACTION_POST_PSU_BY_NODE_ID,
	AMACTION_SET_ID_FIELD,
	AMACTION_GET_ID_FIELD,
	AMACTION_GET_THERMAL_SENSOR,
	AMACTION_GET_HEALTH_SENSOR,
	AMACTION_GET_TRAY_POWER,
	AMACTION_MBP_FW_UPDATE,
	AMACTION_MBP_UART_SWITCH,
	MAX_ACTION
};


static action_map_t am_action_map[] = {
	{AMACTION_GET_FAN_PWM_BY_ID, "get_fan_pwm_by_id"},
	{AMACTION_SET_FAN_PWM, "set_fan_pwm"},
	{AMACTION_RESET_HARD_RESET, "hard_reset"},
	{AMACTION_POST_PSU_BY_NODE_ID, "post_psu_by_node_id"},
	{AMACTION_SET_ID_FIELD, "set_id_field"},
	{AMACTION_GET_ID_FIELD, "get_id_field"},
	{AMACTION_GET_THERMAL_SENSOR, "get_thermal_sensor"},
	{AMACTION_GET_HEALTH_SENSOR, "get_health_sensor"},
	{AMACTION_GET_TRAY_POWER, "get_tray_power"},
	{AMACTION_MBP_FW_UPDATE, "mbp_fw_update"},
	{AMACTION_MBP_UART_SWITCH, "uart_switch"}
	};


#endif

