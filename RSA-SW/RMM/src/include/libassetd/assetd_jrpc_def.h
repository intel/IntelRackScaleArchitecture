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


#ifndef __ASSET_JRPC_DEF_H__
#define __ASSET_JRPC_DEF_H__

#include <stdio.h>

#define JRPC_NODE_TYPE				"node_type"
#define JRPC_PSU_CURRENT_OUT		"currentout"
#define JRPC_PSU_POWER_IN			"powerin"
#define JRPC_POS					"pos"
#define JRPC_CM_LID					"cm_lid"
#define JRPC_CM_ULOC				"cm_uloc"
#define JRPC_EVT_TYPE				"evt_type"
#define JRPC_MODULE_NAME			"module_name"
#define JRPC_IPADDRESS				"ipaddress"
#define JRPC_VER_HIGH				"verhigh"
#define JRPC_VER_LOW				"verlow"
#define JRPC_INFO					"info"
#define JRPC_PRESENT_INFO			"present"
#define JRPC_DRAWER_ALERT			"drawer_alert"
#define JRPC_NODE_ID				"node_id"
#define JRPC_UUID					"component"
#define JRPC_PARENT_UUID			"parent"
#define JRPC_PERI_ID				"peripheral_id"
#define JRPC_MBP_HOST_IP			"mbp_ip"

#define JRPC_TRAY_IDX				"tray_idx"
#define JRPC_TARGET_IDX				"target_idx"
#define JRPC_PSU_SERIAL				"psu_serial"
#define JRPC_PSU_MANUFACTURE		"psu_manufacture"
#define JRPC_PSU_MODEL				"psu_model"
#define JRPC_PSU_FW_VER				"psu_fw_ver"

#define JRPC_CM_NODE_ID				"cm_node_id"
#define JRPC_FAN_NODE_ID			"fan_node_id"
#define JRPC_FAN_PWM				"fan_pwm"


#define JRPC_PZ_LID					"pz_lid"
#define JRPC_PZ_ULOC				"pz_uloc"
#define JRPC_PSU_LID				"psu_lid"
#define JRPC_PSU_XLOC				"psu_xloc"
#define JRPC_PSU_ULOC				"psu_uloc"
#define JRPC_TZ_LID					"tz_lid"
#define JRPC_TZ_ULOC				"tz_uloc"
#define JRPC_DZ_LID					"dz_lid"
#define JRPC_DZ_ULOC				"dz_uloc"
#define JRPC_FAN_LID				"fan_lid"
#define JRPC_FAN_XLOC				"fan_xloc"
#define JRPC_FAN_ULOC				"fan_uloc"
#define JRPC_DRAWER_LID				"drawer_lid"
#define JRPC_DRAWER_ULOC			"drawer_uloc"


#define JRPC_FIELD_TYPE				"field_type"
#define JRPC_FIELD_INSTANCE			"field_instance"
#define JRPC_BYTE_NUM				"byte_num"
#define JRPC_DATA					"data"

#define JRPC_PWM					"fan_pwm"
#define JRPC_PSU_ENABLE_STATE		"psu_enable_state"

#define JRPC_GAMI_REG_NAME			"reg_name"
#define JRPC_GAMI_REG_IP			"reg_ip"
#define JRPC_GAMI_REG_PORT			"reg_port"
#define JRPC_GAMI_CAP_NAME			"name"
#define JRPC_GAM_CAP_UPDATE			"update"
#define JRPC_GAMI_REG_CAP_MBP		"cap_mbp"
#define JRPC_GAMI_REG_CAP_PSU		"cap_psu"
#define JRPC_GAMI_REG_CAP_FAN		"cap_fan"
#define JRPC_GAMI_REG_CAP_DRAWER	"cap_drawer"

#define JRPC_GAM_REG_ASSETMODULE	"asset_module"

#define JRPC_ACTION_NAME			"action"
#define JRPC_ACTION_ATTR			"attributes"
#define JRPC_ACTION_ATTR_NAME		"name"
#define JRPC_ACTION_VALUE			"value"
#define JRPC_ACTION_VALUE_STR		"string"
#define JRPC_ACTION_VALUE_NUM		"number"
#define JRPC_ACTION_VALUE_BOOL		"boolean"

#define JRPC_ACTION_MBP_UPDATE				"mbp_update"
#define JRPC_ACTION_MBP_UPDATE_HOST			"host"
#define JRPC_ACTION_MBP_UPDATE_DEV			"dev"
#define JRPC_ACTION_MBP_UPDATE_FILE			"file"

#define JRPC_ACTION_SET_ID_FIELD			"set_id_field"
#define JRPC_ACTION_GET_ID_FIELD			"get_id_field"
#define JRPC_ACTION_GET_THERMAL_SENSOR		"get_thermal_sensor"
#define JRPC_ACTION_GET_HEALTH_SENSOR		"get_health_sensor"
#define JRPC_ACTION_GET_TRAY_POWER			"get_tray_power"
#define JRPC_ACTION_GET_FAN_PWM				"get_fan_pwm_by_id"
#define JRPC_ACTION_SET_FAN_PWM				"set_fan_pwm"
#define JRPC_ACTION_HARD_RESET				"peripheral_hard_reset"
#define JRPC_ACTION_POST_PSU				"post_psu_by_node_id"




#endif
