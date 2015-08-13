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
 * @file command/network/json/get_components.cpp
 *
 * @brief JSON command get components implementation
 * */

#include "get_components.hpp"

using namespace agent_framework;
using namespace agent_framework::command::network::json;

GetComponents::GetComponents() :
    CommandJson(Procedure(TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "component", jsonrpc::JSON_STRING,
                nullptr)) { }

void GetComponents::method(const Json::Value& params, Json::Value& result) {
    try {
        (void)params; // line added to avoid compiler error "unused-parameter"

        Command* command = get_command();

        network::GetComponents::Request request{};
        network::GetComponents::Response response{};

        request.m_component = params["component"].asString();

        command->execute(request, response);

        if (0 != response.m_components.size()) {
            result = m_add_components(response.m_components);
        }
        else {
            result = Json::arrayValue;
        }

    } catch (const command::exception::NotFound&) {
        /* @TODO: Move common exceptions to JSON command server */
        throw jsonrpc::JsonRpcException(-32602, "Component not found");
    } catch (...) {
        throw jsonrpc::JsonRpcException(-1, "JSON command error");
    }
}

Json::Value GetComponents::m_add_component(network::GetComponents::Response::Component & component) {
    Json::Value item;
    item["type"] = component.m_type;
    item["component"] = component.m_name;
    if (component.m_components.size() > 0) {
        item["components"] = m_add_components(component.m_components);
    }
    return item;
}

Json::Value GetComponents::m_add_components(std::vector<network::GetComponents::Response::Component> & components) {
    Json::Value json_components;
    Json::Value json_item;
    for (network::GetComponents::Response::Component component : components) {
        json_item = m_add_component(component);
        json_components.append(std::move(json_item));
    }
    return json_components;
}

void GetComponents::notification(const Json::Value&) { }

static CommandJson::Register<GetComponents> g;
