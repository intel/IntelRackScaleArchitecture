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


#ifndef __LIBUTILS_RMM_JSON_H__
#define __LIBUTILS_RMM_JSON_H__

#define RMM_JSON_ID				"id"
#define RMM_JSON_UUID				"uuid"
#define RMM_JSON_NAME				"name"
#define RMM_JSON_HREF				"links"
#define RMM_JSON_RACkUUID			"rackUUID"

#define RMM_JSON_POD				"pod"
#define RMM_JSON_RACK				"rack"
#define RMM_JSON_XLOC				"xLocation"
#define RMM_JSON_ULOC				"uLocation"
#define RMM_JSON_UHEIGHT			"uHeight"

#define RMM_JSON_LOC				"location"
#define RMM_JSON_ZONE_ID			"zone_id"
#define RMM_JSON_RACK_UNITS			"rackUnits"
#define RMM_JSON_RACK_LOC			"rackLocation"
#define RMM_JSON_PRESENCE			"presence"
#define RMM_JSON_DESC				"description"
#define RMM_JSON_CREATED_DATE			"createdDate"
#define RMM_JSON_UPDATED_DATE			"updatedDate"
#define RMM_JSON_ASSET_TAG			"assetTag"
#define RMM_JSON_STATE				"enabledState"

#define RMM_JSON_PROPERTY_NAME			"propertyName"
#define RMM_JSON_ALLOWABLE_VALUES		"allowableValues"
#define RMM_JSON_CAPABILITIES			"capabilities"
#define RMM_JSON_AVALIBLE_ACTIONS		"availableActions"
#define RMM_JSON_RESET_TYPE			"resetType"

#define RMM_JSON_RESET				"reset"
#define RMM_JSON_RESETED			"reseted"
#define RMM_JSON_RESET_BIG			"Reset"
#define RMM_JSON_RESETED_BIG			"Reseted"

#define RMM_JSON_SETUARTTARGET			"setUartTarget"
#define RMM_JSON_RESET_MODE_SERVICE		"service"
#define RMM_JSON_RESET_MODE_RMM			"rmm"
#define RMM_JSON_RESULT				"result"
#define RMM_JSON_LOAD_FACTORY_DEFAULT		"loadFactoryDefault"

#define RMM_JSON_RESET_SOFT_STR			"soft"
#define RMM_JSON_RESET_HARD_STR			"hard"

#define RMM_JSON_SERIAL_NUM			"serialNumber"
#define RMM_JSON_MANUFACTURE			"manufacture"
#define RMM_JSON_MODEL_NUM			"modelNumber"
#define RMM_JSON_PART_NUM			"partNumber"
#define RMM_JSON_FIRMWARE_VER			"firmwareRevision"
#define RMM_JSON_OEM				"oem"
#define RMM_JSON_MODIFY				"modified"
#define RMM_JSON_TYPE				"type"

#define RMM_JSON_ACTION				"action"
#define RMM_JSON_DEST				"destination"
#define RMM_JSON_PROTOCOL			"protocol"
#define RMM_JSON_PORT				"port"

#define RMM_JSON_HW_ADDR			"MBPHWAddress"
#define RMM_JSON_IP_ADDR			"MBPIPAddress"
#define RMM_JSON_FRU_INFO			"FRUInfo"
#define RMM_JSON_FRU_RESULT			"result"
#define RMM_JSON_REQ_ENABLED_STATE		"requestEnabledState"
#define RMM_JSON_API_VER			"apiVersion"
#define RMM_JSON_PLATFORM			"platform"

/*log*/
#define RMM_JSON_COUNT				"count"
#define RMM_JSON_LEN				"length"
#define RMM_JSON_LOG_INFO			"loginfo"

/*RACK*/
#define RMM_JSON_RACK_ID			"rack_id"
#define RMM_JSON_POD_DCUID			"podDCUID"
#define RMM_JSON_RACK_PUID			"rackPUID"
#define RMM_JSON_PODM_ADDR			"PODMAddress"
#define RMM_JSON_GEO_TAG			"geoTag"
#define RMM_JSON_RACK_DCUID			"rackDCUID"
#define RMM_JSON_SYSTEM_PRESENCE		"systemPresence"
#define RMM_JSON_MBP_PRESENCE			"MBPPresence"
#define RMM_JSON_RMM_PRESENT			"RMMPresent"
#define RMM_JSON_DISAGGR			"rackSupportsDisaggregatedPowerCooling"
#define RMM_JSON_COMPONENT_TYPE			"componentType"
#define RMM_JSON_IMAGE				"image"

/*DRAWER*/
#define RMM_JSON_DRAWERS			"drawers"
#define RMM_JSON_DRAWER_ID			"drawer_id"
#define RMM_JSON_DRAWER_IPADDRESS		"TMCIPAddress"
#define RMM_JSON_DRAWER_RUID			"trayRUID"
#define RMM_JSON_DRAWER_PUID			"trayPUID"

#define RMM_JSON_DRAWER_STATE			"thermalState"
#define RMM_JSON_DRAWER_POWER_CONSUMED_WATTS	"powerConsumedWatts"
#define RMM_JSON_DRAWER_PRESENT_TEMP		"presentTemperature"
#define RMM_JSON_DRAWER_POWER_AND_THERMAL	"powerAndThermal"
#define RMM_JSON_MAX_DRAWER_SUPPORTED		"maxDrawerSupported"

/*Power Zone*/
#define RMM_JSON_POWER_ZONES			"powerzones"
#define RMM_JSON_POWER_ZONE_NAME		"name"
#define RMM_JSON_PSU_NAME			"name"
#define RMM_JSON_PZONE_ID			"powerzone_id"
#define RMM_JSON_PSU_ID				"psu_id"
#define RMM_JSON_POWER_INPUT			"presentPowerInput"
#define RMM_JSON_POWER_OUTPUT			"presentPowerOutput"
#define RMM_JSON_POWER_MAX_RATED_CAPACITY	"maxRatedPowerCapacity"
#define RMM_JSON_MAX_PSUS_SUPPORT		"maxPSUsSupported"
#define RMM_JSON_PSU_NUMBER_OF_PRESENT		"numberOfPSUsPresent"
#define RMM_JSON_PSUS				"psus"
#define RMM_JSON_REDUNDANCY_SET			"redundancySet"
#define RMM_JSON_HEALTH_CODE			"healthCode"
#define RMM_JSON_STATUS				"status"
#define RMM_JSON_PSU_ENABLE_STATE		"state"

/*thermalzones*/
#define RMM_JSON_TZONE_NAME			"name"
#define RMM_JSON_TZONE_ID			"thermalzone_id"
#define RMM_JSON_FAN_NAME			"name"
#define RMM_JSON_FAN_ID				"fan_id"
#define RMM_JSON_THERMAL_ZONES			"thermalzones"
#define RMM_JSON_POLICY				"policy"
#define RMM_JSON_PRESENT_TEMP			"presentTemperature"
#define RMM_JSON_OUT_LET_TEMP			"outletTemperature"
#define RMM_JSON_AIRFLOW			"volumetricAirflow"
#define RMM_JSON_MAX_FANS_SUPPORTED		"maxFansSupported"
#define RMM_JSON_NUM_OF_FANS_PRESENT		"numberOfFansPresent"
#define RMM_JSON_FANS				"fans"
#define RMM_JSON_ENABLE_STATE			"enabledState"
#define RMM_JSON_FAN_ENABLE_STATE		"state"
#define RMM_JSON_HEALTH_STATE			"healthState"
#define RMM_JSON_DESIRED_SPEED			"desiredSpeed"
#define RMM_JSON_TACHOMETER_READING		"presentTachMeterReading"
#define RMM_JSON_METRE_STATE			"meterState"
#define RMM_JSON_LOWER_NON_CRITICAL		"lowerNonCritical"
#define RMM_JSON_UPPER_NON_CRITICAL		"upperNonCritical"
#define RMM_JSON_LOWER_CRITICAL			"lowerCritical"
#define RMM_JSON_UPPER_CRITICAL			"upperCritical"
#define RMM_JSON_THRESHOLD			"tachMeterThreshold"
#define RMM_JSON_SPEED_UNIT			"speedUnit"
#define RMM_JSON_UNIT_TYPE			"unitType"
#define RMM_JSON_REQ_STATE_CHANGE		"requestStateChange"
#define RMM_JSON_SET_DESIRED_SPEED		"setDesiredSpeed"
#define RMM_JSON_DESIRED_VALUE			"desiredValue"
#define RMM_JSON_RPM				"RPM"
#define RMM_JSON_PWM				"PWM"
#define RMM_JSON_SET_DESIRED_SPEED		"setDesiredSpeed"

/*events*/
#define RMM_JSON_EVENTS				"rf_event"

/*MZone*/
#define RMM_JSON_MBPS				"mbps"
#define RMM_JSON_MBP_ID				"mbp_id"
#define RMM_JSON_MBP_MBPID			"MBPID"
#define RMM_JSON_FW_STATE			"firmwareState"
#define RMM_JSON_TARGET_TRAY			"targetTray"
#define RMM_JSON_TARGET_INDEX			"targetIndex"

#define RMM_JSON_UPDATING			"updating"
#define RMM_JSON_UPDATE				"update"
#define RMM_JSON_UPDATE_BIG			"Update"
#define RMM_JSON_UPDATED			"updated"
#define RMM_JSON_UPDATED_BIG			"Updated"

/*Redfish*/
#define RMM_JSON_RF_EVT_TYPES			"eventTypes"
#define RMM_JSON_RF_EVT				"event"
#define RMM_JSON_RF_LISTENER			"listener"
#define RMM_JSON_RF_SELF			"self"
#define RMM_JSON_RF_SUBSCRIPTION		"subscription"
#define RMM_JSON_RF_LINKS			"links"
#endif
