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

#include "agent-framework/command/network/get_components.hpp"

using namespace agent_framework;
using namespace agent_framework::command;

/*! Stubs GetComponents implementation */
class GetComponents : public command::network::GetComponents {
public:
    GetComponents() { }

     using agent_framework::command::network::GetComponents::execute;

    void execute(const Request&, Response& response) {
        std::vector<Response::Component> components{};
        components.push_back({"RSAFabricModule",
                              "863e4567-e87b-64d3-a489-987654300004",
                              {
                                  {"RSASwitch", "863e4567-e87b-64d3-a489-987654300001"},
                                  {"RSASwitch", "863e4567-e87b-64d3-a489-987654300002"},
                                  {"RSASwitch", "863e4567-e87b-64d3-a489-987654300003"}
                              }});
        response.set_components(components);
    }

    ~GetComponents();
};

GetComponents::~GetComponents() { }

static Command::Register<GetComponents> g("Stubs");
