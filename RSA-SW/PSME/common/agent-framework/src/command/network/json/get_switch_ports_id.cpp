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
 * @file command/network/json/get_switch_ports_id.cpp
 * @brief JSON command get switch ports id information implementation
 * */

#include "get_switch_ports_id.hpp"
#include "agent-framework/command/network/get_switch_ports_id.hpp"
#include "uuid++.hh"

using namespace agent_framework::command;
using namespace agent_framework::command::network::json;

GetSwitchPortsId::GetSwitchPortsId() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetSwitchPortsId::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        network::GetSwitchPortsId::Request request{};
        network::GetSwitchPortsId::Response response{};

        request.m_component = params["component"].asString();
        uuid id(request.m_component.c_str());

        command->execute(request, response);

        Json::Value json_ports_id;
        for (auto& port_id : response.m_ports_id) {
            Json::Value json_port_id;
            json_port_id["Id"] = port_id;
            json_ports_id.append(json_port_id);
        }

        result["portIdentifiers"] = json_ports_id;

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (const uuid_error_t&) {
        throw jsonrpc::JsonRpcException(-32602, "Invalid UUID format");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetSwitchPortsId::notification(const Json::Value&) { }

static CommandJson::Register<GetSwitchPortsId> g;
