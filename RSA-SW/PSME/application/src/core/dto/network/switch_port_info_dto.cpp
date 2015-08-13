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

#include "switch_port_info_dto.hpp"

using namespace psme::core::dto::network;

SwitchPortInfoDTO::~SwitchPortInfoDTO(){}

SwitchPortInfoDTO::Request::~Request(){}

SwitchPortInfoDTO::Response::~Response(){}

const Json::Value SwitchPortInfoDTO::Request::to_json() const {
    Json::Value json_request;
    json_request["component"] = get_component();
    json_request["portIdentifier"] = get_port_identifier();
    return json_request;
}

void SwitchPortInfoDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_port_identifier(response["portIdentifier"].asString());
        set_administrative_state(response["administrativeState"].asString());
        set_link_technology(response["linkTechnology"].asString());
        set_link_speed_gbps(response["linkSpeedGbps"].asUInt());
        set_max_speed_gbps(response["maxSpeedGbps"].asUInt());
        set_operational_state(response["operationalState"].asString());
        set_port_width(response["portWidth"].asUInt());
        set_frame_size(response["frameSize"].asUInt());
        set_autosense(response["autoSense"].asBool());
        set_is_management_port(response["isManagementPort"].asBool());
        set_last_error_code(response["lastErrorCode"].asUInt());
        set_error_cleared(response["errorCleared"].asBool());
        set_last_state_change_time(response["lastStateChangeTime"].asString());
        set_mac_address(response["macAddress"].asString());
        set_vlan_enable(response["vlanEnable"].asBool());

        m_status.to_object(response["status"]);
        m_ipv4_address.to_object(response["ipv4Address"]);
        m_ipv6_address.to_object(response["ipv6Address"]);
        m_neighbor_info.to_object(response["neighborInfo"]);
        m_oem.to_object(response["oem"]);
    }
}
