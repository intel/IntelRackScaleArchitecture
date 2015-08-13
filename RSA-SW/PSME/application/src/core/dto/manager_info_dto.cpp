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
 * */

#include "manager_info_dto.hpp"

using namespace psme::core::dto;

ManagerInfoDTO::~ManagerInfoDTO(){}

ManagerInfoDTO::Request::~Request(){}

ManagerInfoDTO::Response::~Response(){}

const Json::Value ManagerInfoDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["component"] = get_component_id();

    return json_request;
}

void ManagerInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_firmware_version(response["firmwareVersion"].asString());
        set_ipv4_address(response["ipv4address"].asString());
        set_network_services(create_network_services(
                                                response["networkServices"]));
        set_serial_console(create_serial_console(response["serialConsole"]));
    }
}

NetworkService
ManagerInfoDTO::Response::create_network_service(const Json::Value& response) {
    NetworkService service;
    service.set_name(response["name"].asString());
    service.set_port(response["port"].asUInt());
    service.set_enabled(response["enabled"].asBool());
    return service;
}

NetworkServices
ManagerInfoDTO::Response::create_network_services(const Json::Value& response) {
    if (response.isArray()) {
        NetworkServices services;
        for (const auto& service_data : response) {
            services.push_back(create_network_service(service_data));
        }
        return services;
    }
    return {};
}

SerialConsole
ManagerInfoDTO::Response::create_serial_console(const Json::Value& response) {
    if (response.isObject()) {
        SerialConsole serial_console;
        serial_console.set_signal_type(response["signalType"].asString());
        serial_console.set_bitrate(response["bitrate"].asUInt());
        serial_console.set_parity(response["parity"].asString());
        serial_console.set_data_bits(response["dataBits"].asUInt());
        serial_console.set_stop_bits(response["stopBits"].asUInt());
        serial_console.set_flow_control(response["flowControl"].asString());
        serial_console.set_pin_out(response["pinOut"].asString());
        serial_console.set_enabled(response["enabled"].asBool());
        return serial_console;
    }
    return {};
}

SerialConsole::~SerialConsole(){}
