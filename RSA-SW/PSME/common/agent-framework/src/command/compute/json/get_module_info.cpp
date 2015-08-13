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
 * @file command/compute/json/get_module_info.cpp
 *
 * @brief JSON command get module info implementation
 * */

#include "get_module_info.hpp"
#include "agent-framework/logger_ext.hpp"

using namespace agent_framework::command::compute::json;

GetModuleInfo::GetModuleInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "component", jsonrpc::JSON_STRING,
                nullptr))
{

}

void GetModuleInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetModuleInfo::Request request{};
        compute::GetModuleInfo::Response response{};

        request.m_component = params["component"].asString();

        command->execute(request, response);

        result["status"] = response.m_status.to_json();
        result["bladeCount"] = response.m_blade_count;
        result["switchCount"] = response.m_switch_count;
        result["slot"] = response.m_slot;
        result["fruInfo"] = response.m_fru_info.to_json();
        result["oem"] = response.m_oem_data.to_json();

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (const std::exception& e) {
        log_error(GET_LOGGER("command"), e.what());
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetModuleInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetModuleInfo> g;
