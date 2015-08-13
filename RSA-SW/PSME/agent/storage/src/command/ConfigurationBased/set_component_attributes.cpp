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

#include "agent-framework/command/storage/set_component_attributes.hpp"

#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! SetComponentAttributes implementation */
class SetComponentAttributes : public storage::SetComponentAttributes {
public:
    SetComponentAttributes() {}

    using storage::SetComponentAttributes::execute;

    void execute(const Request& request, Response&) {
        const auto& uuid = request.get_component();

        auto logical_drive = ModuleManager::find_logical_drive(uuid).lock();
        if (logical_drive) {
            // @TODO: Implement setting logical drive attributes
            return;
        }

        auto target = ModuleManager::find_target(uuid).lock();
        if (target) {
            // @TODO: Implement setting target attributes
            return;
        }

        log_warning(GET_LOGGER("rpc"), "Component uuid=" << uuid << " not found.");
        throw exception::NotFound();
    }

    ~SetComponentAttributes();
};

SetComponentAttributes::~SetComponentAttributes() {}

static Command::Register<SetComponentAttributes> g("ConfigurationBased");
