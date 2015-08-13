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

#include "port_vlan_info_dto.hpp"

using namespace psme::core::dto::network;

PortVlanInfoDTO::~PortVlanInfoDTO(){}

PortVlanInfoDTO::Request::~Request(){}

PortVlanInfoDTO::Response::~Response(){}

const Json::Value PortVlanInfoDTO::Request::to_json() const {
    Json::Value json_request;
    json_request["component"] = get_component();
    json_request["portIdentifier"] = get_port_identifier();
    json_request["vlanIdentifier"] = get_vlan_identifier();
    return json_request;
}

void PortVlanInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_vlan_id(response["vlanId"].asUInt());
        set_vlan_enable(response["vlanEnable"].asBool());
        set_tagged(response["tagged"].asBool());

        m_status.to_object(response["status"]);
        m_ipv4_address.to_object(response["ipv4Address"]);
        m_ipv6_address.to_object(response["ipv6Address"]);
        m_oem.to_object(response["oem"]);
    }
}
