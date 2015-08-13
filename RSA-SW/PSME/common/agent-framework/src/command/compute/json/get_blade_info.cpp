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
 * @file command/compute/json/get_blade_info.cpp
 *
 * @brief JSON command get blade information implementation
 * */

#include "get_blade_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::command::compute::json;

GetBladeInfo::GetBladeInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_OBJECT,
                "component", jsonrpc::JSON_STRING,
                nullptr)) { }

namespace {
    Json::Value
    boot_override_supported_to_json(
           compute::GetBladeInfo::Response::BootSupportedVec& boot_supported) {
        Json::Value json_obj(Json::arrayValue);
        for (const auto& bs : boot_supported) {
            json_obj.append(bs);
        }
        return json_obj;
    }
}

void GetBladeInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetBladeInfo::Request request{};
        compute::GetBladeInfo::Response response{};

        request.m_component = params["component"].asString();

        command->execute(request, response);

        result["status"] = response.m_status.to_json();
        result["biosVersion"] = response.m_bios_version;
        result["bootOverride"] = response.m_boot_override;
        result["bootOverrideTarget"] = response.m_boot_override_target;
        result["bootOverrideSupported"] =
          boot_override_supported_to_json(response.m_boot_override_supported);
        result["processorCount"] = response.m_processor_count;
        result["dimmCount"] = response.m_dimm_count;
        result["controllerCount"] = response.m_controller_count;
        result["nicCount"] = response.m_nic_count;
        result["powerState"] = response.m_power_state;
        result["slot"] = response.m_slot;
        result["fruInfo"] = response.m_fru_info.to_json();
        result["oem"] = response.m_oem_data.to_json();

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetBladeInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetBladeInfo> g;
