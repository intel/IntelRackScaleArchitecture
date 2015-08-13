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
 * @file command/network/json/set_switch_port_attributes.cpp
 * @brief JSON command SetSwitchPortAttributes implementation
 * */

#include "set_switch_port_attributes.hpp"
#include "agent-framework/command/network/set_switch_port_attributes.hpp"
#include "uuid++.hh"

using namespace agent_framework::command;
using namespace agent_framework::command::network::json;

SetSwitchPortAttributes::SetSwitchPortAttributes() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                "portIdentifier", jsonrpc::JSON_STRING,
                "linkSpeedGbps", jsonrpc::JSON_INTEGER,
                "administrativeState", jsonrpc::JSON_STRING,
                "frameSize", jsonrpc::JSON_INTEGER,
                "autoSense", jsonrpc::JSON_BOOLEAN,
                nullptr)) { }

void SetSwitchPortAttributes::method(const Json::Value& params, Json::Value& result) {
    (void)result;
    try {
        Command* command = get_command();

        network::SetSwitchPortAttributes::Request request{};
        network::SetSwitchPortAttributes::Response response{};

        request.m_component = params["component"].asString();
        request.m_port_identifier = params["portIdentifier"].asString();
        request.m_link_speed_gbps = params["linkSpeedGbps"].asUInt();
        request.m_administrative_state = params["administrativeState"].asString();
        request.m_frame_size = params["frameSize"].asUInt();
        request.m_auto_sense = params["autoSense"].asBool();
        uuid id(request.m_component.c_str());

        command->execute(request, response);

        result["status"] = response.m_status;
    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (const uuid_error_t&) {
        throw jsonrpc::JsonRpcException(-32602, "Invalid UUID format");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void SetSwitchPortAttributes::notification(const Json::Value&) { }

static CommandJson::Register<SetSwitchPortAttributes> g;
