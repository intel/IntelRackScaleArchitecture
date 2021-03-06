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

#include "set_switch_port_attributes_dto.hpp"

using namespace psme::core::dto::network;

SetSwitchPortAttributesDTO::~SetSwitchPortAttributesDTO(){}

SetSwitchPortAttributesDTO::Request::~Request(){}

SetSwitchPortAttributesDTO::Response::~Response(){}

const Json::Value SetSwitchPortAttributesDTO::Request::to_json() const {
    Json::Value json_request;
    json_request["component"] = get_component();
    json_request["portIdentifier"] = get_port_identifier();
    json_request["linkSpeedGbps"] = get_link_speed_gbps();
    json_request["administrativeState"] = get_administrative_state();
    json_request["autoSense"] = get_auto_sense();
    json_request["frameSize"] = get_frame_size();
    json_request["oem"] = get_oem().to_json();
    return json_request;
}

void SetSwitchPortAttributesDTO::Response::to_object(const Json::Value&) {
}
