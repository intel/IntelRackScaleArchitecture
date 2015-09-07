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


#ifndef __CFG_DEF__
#define __CFG_DEF__

#define PROC_RMM				"rmm"
#define PROC_RESTD				"restd"
#define PROC_MEMDBD				"memdbd"
#define PROC_IPMI_MODULE		"ipmi_module"
#define PROC_RMM_LOGD			"rmmlogd"
#define PROC_UPGRADED			"upgraded"
#define PROC_DB_SYNCD			"db_syncd"
#define PROC_REDFISHD			"redfishd"
#define PROC_SNMP_SUBAGENTD		"snmp_subagentd"
#define PROC_REGISTERD          "registerd"
#define PROC_ASSETD             "assetd"
#define PROC_ASSET_MODULE       "asset_module"
#define PROC_SENSORD			"sensord"
#define PROC_RMM_KEEPERD		"rmm_keeperd"
#define PROC_AUTO_TEST			"auto_test"

#define ATTR_VM_HEADER			"vm" 
#define ATTR_VM_ROOT_PASSWORD	"root_password"

#define ATTR_PORT     			"Port"
#define ATTR_JRPC_PORT     		"JrpcPort"
#define ATTR_LOG_MODULES    	"LogModules"
#define ATTR_TTY_INFOS    		"TTYInfos"
#define ATTR_MAX_LINES			"MaxNumberOfRecords"
#define ATTR_LOG_LEVEL			"Level"
#define ATTR_RMCP_USERNAME		"UserName"
#define ATTR_RMCP_PASSWORD		"Password"
#define ATTR_SYNC_DB_VER_PORT	"SyncDbVerPort"
#define ATTR_KEEPER_DDB_PORT	"KeeperDdbPort"
#define ATTR_SNAPSHOT_SYNC_PORT	"SnapshotSyncPort"
#define ATTR_REST_PREFIX     	"restful_prefix"

#define ATTR_RACK_PLATFORM		"Platform"
#define ATTR_RACK_PLATFORM_BDCA	"BDC-A"
#define ATTR_RACK_PLATFORM_BDCR	"BDC-R"

#define ATTR_IPMID_SERIAL_BAUDRATE 		"SerialBaudRate"
#define ATTR_IPMID_SERIAL_DATABITS 		"SerialDataBits"
#define ATTR_IPMID_SERIAL_STOPBITS 		"SerialStopBits"
#define ATTR_IPMID_SERIAL_PARITY 		"SerialParity"
#define ATTR_IPMID_SERIAL_FLOWCONTROL 	"SerialFlowControl"

#define ATTR_IPMI_RMCP_CLIENT_PORT 		"RmcpClientPort"
#define ATTR_IPMI_IPMB_UDP_PORT 		"IpmbPort"
#define ATTR_IPMI_I2CD_UDP_PORT 		"I2cdPort"
#define ATTR_IPMI_LISTEN_PORT			"IpmiListenPort"
#define ATTR_IPMI_COOLING_CTRL_PORT		"IpmiCoolingCtrlPort"
#define ATTR_IPMI_POWER_MGMT_PORT		"IpmiPowerMgmtPort"
#define ATTR_IPMI_ASSET_MODULE_PORT		"IpmiAssetModulePort"
#define ATTR_IPMI_SENSOR_MONITOR_PORT 	"IpmiSensorMonitorPort"
#define ATTR_IPMI_TEST_PORT				"IpmiTestPort"
#define ATTR_IPMI_UART_TGT_CFG_PORT		"IpmiUartTgtCfgPort"
#define ATTR_IPMI_WEBSOCKET_PORT		"IpmiWebSocketPort"
#define ATTR_IPMI_JSON_RPC_SERVER_PORT	"IpmiJRpcServerPort"
#define ATTR_IPMI_JSON_RPC_IPMID_PORT	"IpmiJRpcIpmidPort"
#define ATTR_IPMI_JSON_RPC_SNMP_SUBAGNETD_PORT	"IpmiJRpcSnmpSubAgentdPort"

#endif



