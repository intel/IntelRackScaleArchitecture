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
 * @file command/network/json/get_remote_switch_info.cpp
 * @brief JSON command get remote switch info information implementation
 * */

#include "get_remote_switch_info.hpp"
#include "agent-framework/command/network/get_remote_switch_info.hpp"
#include "uuid++.hh"

using namespace agent_framework::command;
using namespace agent_framework::command::network::json;

GetRemoteSwitchInfo::GetRemoteSwitchInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                "switchIdentifier", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetRemoteSwitchInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        network::GetRemoteSwitchInfo::Request request{};
        network::GetRemoteSwitchInfo::Response response{};

        request.m_component = params["component"].asString();
        request.m_switch_id = params["switchIdentifier"].asString();
        uuid id(request.m_component.c_str());

        command->execute(request, response);

        result["macAddress"] = response.m_mac_address;
        Json::Value json_next_hops;
        for (const auto& next_hop : response.m_next_hops) {
            Json::Value json_next_hop;
            json_next_hop["metric"] = next_hop.get_metric();
            json_next_hop["portIdentifier"] = next_hop.get_port_identifier();
            json_next_hop["ipv4Address"] = next_hop.get_ipv4_address();
            json_next_hop["ipv6Address"] = next_hop.get_ipv6_address();
            json_next_hops.append(json_next_hop);
        }
        result["nextHop"] = json_next_hops;

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (const uuid_error_t&) {
        throw jsonrpc::JsonRpcException(-32602, "Invalid UUID format");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetRemoteSwitchInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetRemoteSwitchInfo> g;
