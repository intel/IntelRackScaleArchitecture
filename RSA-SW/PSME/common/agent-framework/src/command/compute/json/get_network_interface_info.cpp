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
 * @file command/compute/json/get_network_interface_info.cpp
 *
 * @brief JSON command get network interface information implementation
 * */

#include "get_network_interface_info.hpp"

using namespace agent_framework::command::compute::json;

GetNetworkInterfaceInfo::GetNetworkInterfaceInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                "interface", jsonrpc::JSON_INTEGER,
                nullptr)) { }

void GetNetworkInterfaceInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetNetworkInterfaceInfo::Request request{};
        compute::GetNetworkInterfaceInfo::Response response{};

        request.m_component = params["component"].asString();
        request.m_interface = params["interface"].asUInt();

        command->execute(request, response);

        result["frameSize"] = response.m_frame_size;
        result["linkTechnology"] = response.m_link_technology;
        result["speedGbps"] = response.m_speed_gbps;
        result["autoSense"] = response.m_autosense;
        result["macAddress"] = response.m_mac_address;
        result["vlanEnable"] = response.m_vlan_enable;
        result["vlanCount"] = response.m_vlan_count;

        result["ipv4Address"] = response.m_ipv4_address.to_json();
        result["ipv6Address"] = response.m_ipv6_address.to_json();
        result["neighborInfo"] = response.m_neighbor_info.to_json();

        result["status"]["health"] = response.m_status.get_health();
        result["status"]["state"] = response.m_status.get_state();

        result["oem"] = response.m_oem->to_json();

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetNetworkInterfaceInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetNetworkInterfaceInfo> g;
