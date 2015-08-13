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

#include "add_logical_drive_dto.hpp"

using namespace psme::core::dto::storage;

AddLogicalDriveDTO::~AddLogicalDriveDTO(){}

AddLogicalDriveDTO::Request::~Request(){}

AddLogicalDriveDTO::Response::~Response(){}

const Json::Value AddLogicalDriveDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["type"] = get_type();
    json_request["capacityGB"] = get_capacity_gb();
    json_request["mode"] = get_mode();
    json_request["master"] = get_master();
    json_request["snapshot"] = get_snapshot();
    json_request["protected"] = get_protected();

    Json::Value drives(Json::arrayValue);
    for (const auto& drive : m_drives) {
       drives.append(drive);
    }

    json_request["drives"] = drives;
//    json_request["oem"] = get_oem_data();
    json_request["oem"] = Json::objectValue;

    return json_request;
}

void AddLogicalDriveDTO::Response::to_object(const Json::Value& response) {

    if (!response.isObject()) {
        return;
    }

    set_drive(response["drive"].asString());
    m_oem_data.to_object(response["oem"]);
}
