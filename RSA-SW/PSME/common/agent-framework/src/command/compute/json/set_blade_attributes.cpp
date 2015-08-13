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
 * @file command/compute/json/set_blade_attributes.cpp
 *
 * @brief JSON command set blade attributes implementation
 * */

#include "set_blade_attributes.hpp"

using namespace agent_framework::command;
using namespace agent_framework::command::compute::json;

SetBladeAttributes::SetBladeAttributes() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                "bootOverride", jsonrpc::JSON_STRING,
                "bootOverrideTarget", jsonrpc::JSON_STRING,
                "powerState", jsonrpc::JSON_STRING,
                "oem", jsonrpc::JSON_OBJECT,
                nullptr)) { }

void SetBladeAttributes::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::SetBladeAttributes::Request request{};
        compute::SetBladeAttributes::Response response{};

        request.m_component = params["component"].asString();
        request.m_boot_override = params["bootOverride"].asString();
        request.m_boot_override_target = params["bootOverrideTarget"].asString();
        request.m_power_state = params["powerState"].asString();

        command->execute(request, response);

        result = Json::objectValue;

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void SetBladeAttributes::notification(const Json::Value&) { }

static CommandJson::Register<SetBladeAttributes> g;
