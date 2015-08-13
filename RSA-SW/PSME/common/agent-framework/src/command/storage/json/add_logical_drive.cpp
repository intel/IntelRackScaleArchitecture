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
 * @file command/storage/json/add_logical_drive.cpp
 *
 * @brief JSON command AddLogicalDrive
 * */

#include "add_logical_drive.hpp"

using namespace agent_framework::command::storage::json;

AddLogicalDrive::AddLogicalDrive() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "type", jsonrpc::JSON_STRING,
                "capacityGB", jsonrpc::JSON_INTEGER,
                "mode", jsonrpc::JSON_STRING,
                "master", jsonrpc::JSON_STRING,
                "snapshot", jsonrpc::JSON_BOOLEAN,
                "protected", jsonrpc::JSON_BOOLEAN,
                "drives", jsonrpc::JSON_ARRAY,
                "oem", jsonrpc::JSON_OBJECT,
                nullptr)) { }

void AddLogicalDrive::method(const Json::Value& params, Json::Value& result) {
        Command* command = get_command();

        storage::AddLogicalDrive::Request request{};
        storage::AddLogicalDrive::Response response{};

        request.m_type = params["type"].asString();
        request.m_capacityGB = params["capacityGB"].asDouble();
        request.m_mode = params["mode"].asString();
        request.m_master = params["master"].asString();
        request.m_snapshot = params["snapshot"].asBool();
        request.m_protected = params["protected"].asBool();

        for (const auto& drive_json : params["drives"]) {
            request.m_drives.emplace_back(drive_json.asString());
        }

        command->execute(request, response);

        result["drive"] = response.m_drive;
        result["oem"] = response.m_oem.to_json();
}

void AddLogicalDrive::notification(const Json::Value&) { }

static CommandJson::Register<AddLogicalDrive> g;
