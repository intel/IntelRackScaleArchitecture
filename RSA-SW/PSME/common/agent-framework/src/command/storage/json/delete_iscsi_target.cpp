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

#include "delete_iscsi_target.hpp"

using namespace agent_framework::command::storage::json;

DeleteISCSITarget::DeleteISCSITarget() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "target", jsonrpc::JSON_STRING,
                nullptr)) { }

void DeleteISCSITarget::method(const Json::Value& params, Json::Value& result) {
        Command* command = get_command();

        storage::DeleteISCSITarget::Request request{};
        storage::DeleteISCSITarget::Response response{};

        request.m_target = params["target"].asString();

        command->execute(request, response);

        result["oem"] = response.m_oem_data.to_json();
}

void DeleteISCSITarget::notification(const Json::Value&) { }

static CommandJson::Register<DeleteISCSITarget> g;
