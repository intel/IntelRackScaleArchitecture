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

#include "switch_ports_id_dto.hpp"
#include "logger/logger_factory.hpp"

using namespace psme::core::dto::network;

const Json::Value SwitchPortsIdDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["component"] = get_component();

    return json_request;
}

void SwitchPortsIdDTO::Response::to_object(const Json::Value& response) {
    const auto& port_identifiers = response["portIdentifiers"];
    if (port_identifiers.isArray()) {
        for (const auto& val : port_identifiers) {
            PortIdentifier port_identifier{val["Id"].asString()};
            add_port_identifier(std::move(port_identifier));
        }
    }
    else {
        log_error(GET_LOGGER("core"), "Invalid GetSwitchPortsId response.");
    }
}

SwitchPortsIdDTO::~SwitchPortsIdDTO(){}

SwitchPortsIdDTO::Response::~Response(){}

SwitchPortsIdDTO::Request::~Request(){}
