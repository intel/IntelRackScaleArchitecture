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
 * */

#include "add_iscsi_target.hpp"

using namespace agent_framework::command::storage::json;

AddISCSITarget::AddISCSITarget() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "targetIQN", jsonrpc::JSON_STRING,
                "initiatorIQN", jsonrpc::JSON_STRING,
                "targetLUN", jsonrpc::JSON_ARRAY,
                "oem", jsonrpc::JSON_OBJECT,
                nullptr)) { }

void AddISCSITarget::method(const Json::Value& params, Json::Value& result) {
        Command* command = get_command();

        storage::AddISCSITarget::Request request{};
        storage::AddISCSITarget::Response response{};

        for (const auto& target_lun : params["targetLUN"]) {
            request.m_target_luns.push_back({
                       target_lun["logicalDrive"].asString(),
                       target_lun["LUN"].asUInt64()
                    });
        }
        request.m_target_iqn = params["targetIQN"].asString();
        request.m_initiator_iqn = params["initiatorIQN"].asString();

        command->execute(request, response);

        result["target"] = response.m_target;
        result["targetAddress"] = response.m_target_address;
        result["targetPort"] = response.m_target_port;
        result["oem"] = response.m_oem_data.to_json();
}

void AddISCSITarget::notification(const Json::Value&) { }

static CommandJson::Register<AddISCSITarget> g;
