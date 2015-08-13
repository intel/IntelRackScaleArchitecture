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

#include "agent-framework/command/storage/get_component_collection.hpp"
#include "agent-framework/module/module_manager.hpp"

using std::vector;

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! Dummy GetComponentCollection implementation */
class GetComponentCollection : public storage::GetComponentCollection {
public:
    GetComponentCollection() {}

    using storage::GetComponentCollection::execute;

    void execute(const Request&, Response& response) {

        const auto& modules = ModuleManager::get_modules();

        if (!modules.empty()) {
            const auto& module = modules.front();
            const auto& submodules = module->get_submodules();
            if (!submodules.empty()) {
                const auto& submodule = submodules.front();
                response.add_component(Response::Component(
                                           submodule->get_name(),
                                           submodule->get_type()));
            }
        }

        log_debug(GET_LOGGER("rpc"), "Getting component collection is successfully finished.");
    }

    ~GetComponentCollection();
};

GetComponentCollection::~GetComponentCollection() {}


static Command::Register<GetComponentCollection> g("ConfigurationBased");
