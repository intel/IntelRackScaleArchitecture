/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 * @file command/network/json/get_switch_port_info.cpp
 *
 * @brief JSON command get switch port information implementation
 * */

#include "get_switch_port_info.hpp"
#include "agent-framework/command/network/get_switch_port_info.hpp"
#include "uuid++.hh"

using namespace agent_framework::command;
using namespace agent_framework::command::network::json;

GetSwitchPortInfo::GetSwitchPortInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                "portIdentifier", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetSwitchPortInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        network::GetSwitchPortInfo::Request request{};
        network::GetSwitchPortInfo::Response response{};

        request.m_component = params["component"].asString();
        request.m_port_identifier = params["portIdentifier"].asString();
        uuid id(request.m_component.c_str());

        command->execute(request, response);

        result["status"] = response.m_status.to_json();
        result["portIdentifier"] = response.m_port_identifier;
        result["linkTechnology"] = response.m_link_technology;
        result["linkSpeedGbps"] = response.m_link_speed;
        result["maxSpeedGbps"] = response.m_max_speed;
        result["operationalState"] = response.m_operational_state;
        result["administrativeState"] = response.m_administrative_state;
        result["portWidth"] = response.m_port_width;
        result["frameSize"] = response.m_frame_size;
        result["autoSense"] = response.m_autosense;
        result["isManagementPort"] = response.m_is_management_port;
        result["lastErrorCode"] = response.m_last_error_code;
        result["errorCleared"] = response.m_error_cleared;
        result["lastStateChangeTime"] = response.m_last_state_change_time;
        result["macAddress"] = response.m_mac_address;
        result["ipv4address"] = response.m_ipv4address.to_json();
        result["ipv6address"] = response.m_ipv6address.to_json();
        result["neighborInfo"] = response.m_neighbor_info.to_json();
        result["vlanEnable"] = response.m_vlan_enable;
        result["oem"] = response.m_oem_data.to_json();

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (const uuid_error_t&) {
        throw jsonrpc::JsonRpcException(-32602, "Invalid UUID format");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetSwitchPortInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetSwitchPortInfo> g;
