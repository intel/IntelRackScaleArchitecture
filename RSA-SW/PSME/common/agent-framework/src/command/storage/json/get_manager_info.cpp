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
 * @file command/storage/json/get_manager_info.cpp
 *
 * @brief JSON command GetManagerInfo implementation
 * */

#include "get_manager_info.hpp"
#include "agent-framework/module/status.hpp"
#include "agent-framework/module/oem_data.hpp"
#include "agent-framework/logger_ext.hpp"

using namespace agent_framework::command::storage::json;

GetManagerInfo::GetManagerInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "component", jsonrpc::JSON_STRING,
                nullptr))
{

}

void GetManagerInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        storage::GetManagerInfo::Request request{};
        storage::GetManagerInfo::Response response{};

        request.m_component = params["component"].asString();

        command->execute(request, response);

        const auto& serial_console = response.m_serial_console;
        const auto& network_services = response.m_network_services;

        result["firmwareVersion"] = response.m_firmware_version;
        result["ipv4address"] = response.m_ipv4address;

        result["status"]["health"] = response.m_status.get_health();
        result["status"]["state"] = response.m_status.get_state();

        for (const auto& network_service: network_services) {
            Json::Value item;
            item["name"] = network_service.m_name;
            item["port"] = network_service.m_port;
            item["enabled"] = network_service.m_enabled;
            result["networkServices"].append(std::move(item));
        }

        result["serialConsole"]["signalType"] = serial_console.m_signal_type;
        result["serialConsole"]["bitrate"] = serial_console.m_bitrate;
        result["serialConsole"]["parity"] = serial_console.m_parity;
        result["serialConsole"]["dataBits"] = serial_console.m_data_bits;
        result["serialConsole"]["stopBits"] = serial_console.m_stop_bits;
        result["serialConsole"]["flowControl"] = serial_console.m_flow_control;
        result["serialConsole"]["pinOut"] = serial_console.m_pin_out;
        result["serialConsole"]["enabled"] = serial_console.m_enabled;

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

void GetManagerInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetManagerInfo> g;
