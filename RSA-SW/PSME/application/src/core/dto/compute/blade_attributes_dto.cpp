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

#include "blade_attributes_dto.hpp"

using namespace psme::core::dto::compute;

BladeAttributesDTO::~BladeAttributesDTO(){}

BladeAttributesDTO::Request::~Request(){}

BladeAttributesDTO::Response::~Response(){}

const Json::Value BladeAttributesDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["component"] = get_component();
    json_request["bootOverride"] = get_boot_override();
    json_request["bootOverrideTarget"] = get_boot_override_target();
    json_request["powerState"] = get_power_state();
    json_request["oem"] = get_oem_data().to_json();

    return json_request;
}

void BladeAttributesDTO::Response::to_object(const Json::Value&) {

}
