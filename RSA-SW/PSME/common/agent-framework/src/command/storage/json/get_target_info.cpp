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

#include "get_target_info.hpp"

using namespace agent_framework::command::storage::json;

GetTargetInfo::GetTargetInfo() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "target", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetTargetInfo::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        storage::GetTargetInfo::Request request{};
        storage::GetTargetInfo::Response response{};

        request.m_target = params["target"].asString();

        command->execute(request, response);

        const auto& target_data  = response.m_target_data;

        result["status"] = target_data.get_status().to_json();

        Json::Value luns(Json::arrayValue);
        for (const auto& lun : target_data.get_target_lun()) {
            Json::Value lun_json;
            lun_json["LUN"] = static_cast<unsigned>(lun.get_lun_id());
            lun_json["logicalDrive"] = lun.get_logical_drive_uuid();
            luns.append(std::move(lun_json));
        }
        result["targetLUN"] = luns;
        result["targetIQN"] = target_data.get_target_iqn();
        result["status"] = target_data.get_status().to_json();
        result["targetAddress"] = target_data.get_target_address();
        result["targetPort"] = target_data.get_target_port();
        result["initiatorIQN"] = target_data.get_initiator_iqn();
        result["oem"] = target_data.get_oem_data().to_json();

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602,
                "GetTargetInfo command execute error");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetTargetInfo::notification(const Json::Value&) { }

static CommandJson::Register<GetTargetInfo> g;
