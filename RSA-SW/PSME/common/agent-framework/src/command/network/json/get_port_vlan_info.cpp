/*!
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file command/network/json/get_port_vlan_info.cpp
 * @brief JSON command get port vlan information implementation
 * */

#include "get_port_vlan_info.hpp"
#include "agent-framework/command/network/get_port_vlan_info.hpp"
#include "uuid++.hh"

using namespace agent_framework::command;
using namespace agent_framework::command::network::json;

GetPortVlanInfo::GetPortVlanInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                "portIdentifier", jsonrpc::JSON_STRING,
                "vlanIdentifier", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetPortVlanInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        network::GetPortVlanInfo::Request request{};
        network::GetPortVlanInfo::Response response{};

        request.m_component = params["component"].asString();
        request.m_vlan_identifier = params["vlanIdentifier"].asString();
        request.m_port_identifier = params["portIdentifier"].asString();
        uuid id(request.m_component.c_str());

        command->execute(request, response);

        result["status"] = response.m_status.to_json();
        result["vlanId"] = response.m_vlan_id;
        result["vlanEnable"] = response.m_vlan_enable;
        result["tagged"] = response.m_tagged;
        result["ipv4address"] = response.m_ipv4address.to_json();
        result["ipv6address"] = response.m_ipv6address.to_json();
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

void GetPortVlanInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetPortVlanInfo> g;
