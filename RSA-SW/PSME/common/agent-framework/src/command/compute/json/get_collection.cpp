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

#include "get_collection.hpp"

using namespace agent_framework;
using namespace agent_framework::command::compute::json;

GetCollection::GetCollection() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "component", jsonrpc::JSON_STRING,
                "name", jsonrpc::JSON_STRING,
                nullptr)) {}

void GetCollection::method(const Json::Value& params, Json::Value& result) {
    try {
        Command* command = get_command();

        compute::GetCollection::Request request{};
        compute::GetCollection::Response response{};

        request.m_component = params["component"].asString();
        request.m_name = params["name"].asString();

        command->execute(request, response);

        result = Json::arrayValue;
        for (const auto& subcomponent : response.m_subcomponents) {
            result.append(m_add_subcomponent(subcomponent));
        }

    } catch (const command::exception::NotFound&) {
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetCollection::notification(const Json::Value& /*params*/) { }

Json::Value GetCollection::m_add_subcomponent(
    const compute::GetCollection::Response::Subcomponent& subcomponent) {

    Json::Value item;
    item["subcomponent"] = subcomponent.m_subcomponent;

    return item;
}

static CommandJson::Register<GetCollection> g;
