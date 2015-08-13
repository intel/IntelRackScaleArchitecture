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

#include "logical_drive_info_dto.hpp"

using namespace psme::core::dto::storage;

LogicalDriveInfoDTO::~LogicalDriveInfoDTO(){}

LogicalDriveInfoDTO::Request::~Request(){}

LogicalDriveInfoDTO::Response::~Response(){}

const Json::Value LogicalDriveInfoDTO::Request::to_json() const {
    Json::Value json_request;

    json_request["drive"] = get_drive();

    return json_request;
}

void LogicalDriveInfoDTO::Response::to_object(const Json::Value& response) {

    if (!response.isObject()) {
        return;
    }

    set_type(response["type"].asString());
    set_image(response["image"].asString());
    set_mode(response["mode"].asString());
    set_master(response["master"].asString());
    set_bootable(response["bootable"].asBool());
    set_protected(response["protected"].asBool());
    set_capacity_gb(response["capacityGB"].asUInt());
    set_snapshot(response["snapshot"].asBool());

    m_oem_data.to_object(response["oem"]);
    m_status.to_object(response["status"]);
    m_collections.to_object(response["collections"]);
}
