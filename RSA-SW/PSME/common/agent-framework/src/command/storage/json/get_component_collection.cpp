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

#include "get_component_collection.hpp"

using namespace agent_framework;
using namespace agent_framework::command::storage::json;

GetComponentCollection::GetComponentCollection() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                nullptr)) {}

void GetComponentCollection::method(const Json::Value&,
                                    Json::Value& result) {
    try {
        Command* command = get_command();

        storage::GetComponentCollection::Request request{};
        storage::GetComponentCollection::Response response{};

        command->execute(request, response);

        result = response.m_components.size() ?
            m_add_components(response.m_components) : Json::arrayValue;
    } catch (const command::exception::NotFound&) {
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

void GetComponentCollection::notification(const Json::Value& /*params*/) { }

Json::Value GetComponentCollection::m_add_component(
    storage::GetComponentCollection::Response::Component& component) {
    Json::Value item;
    item["component"] = component.m_component;
    item["type"] = component.m_type;

    return item;
}

Json::Value GetComponentCollection::m_add_components(
    std::vector<storage::GetComponentCollection::Response::Component>& components) {
    Json::Value json_components;
    Json::Value json_item;

    for (auto component : components) {
        json_item = m_add_component(component);
        json_components.append(std::move(json_item));
    }

    return json_components;
}

static CommandJson::Register<GetComponentCollection> g;
