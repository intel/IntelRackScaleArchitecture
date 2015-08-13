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

#include "set_component_attributes.hpp"

using namespace agent_framework;
using namespace agent_framework::command::storage::json;

SetComponentAttributes::SetComponentAttributes() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "component", jsonrpc::JSON_STRING,
                "attributes", jsonrpc::JSON_ARRAY,
                nullptr)) {}

void
SetComponentAttributes::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        storage::SetComponentAttributes::Request request{};
        storage::SetComponentAttributes::Response response{};

        request.m_component = params["component"].asString();

        if (params["attributes"].isArray()) {
            for (const auto& attribute_json : params["attributes"]) {
                m_add_attribute(request, attribute_json);
            }
        }

        command->execute(request, response);

        result = Json::objectValue;

    } catch (const command::exception::NotFound&) {
        /* @TODO: Common exceptions */
        throw jsonrpc::JsonRpcException(-32602, "Invalid parameter(s).");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-32603, "Internal JSON-RPC error.");
    }
}

void SetComponentAttributes::m_add_attribute(
    storage::SetComponentAttributes::Request& request,
    const Json::Value& attribute_json) {

    generic::Attribute attribute{};
    attribute.from_json(attribute_json);

    request.m_attributes.push_back(attribute);
}

void SetComponentAttributes::notification(const Json::Value& /*params*/) { }

static CommandJson::Register<SetComponentAttributes> g;
