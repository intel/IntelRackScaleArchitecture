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

#include "agent-framework/command/compute/get_module_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::generic;
using namespace agent_framework::command;

using agent_framework::command::exception::NotFound;

/*! GetModuleInfo implementation */
class GetModuleInfo : public compute::GetModuleInfo {
public:
    GetModuleInfo() { }

    using compute::GetModuleInfo::execute;

    void execute(const Request& request, Response& response) {
        auto module = ModuleManager::get_module(request.get_component());

        if (nullptr == module) {
            throw NotFound();
        }

        response.set_status({module->get_state_name(), "OK"});

        const auto& submodules = module->get_submodules();
        response.set_slot(module->get_slot());
        response.set_blade_count(static_cast<unsigned>(submodules.size()));
        response.set_fru_info({ "", module->get_manufacturer_name(),"", ""});
        response.set_oem({});
    }

    ~GetModuleInfo();
};

GetModuleInfo::~GetModuleInfo() { }

static Command::Register<GetModuleInfo> g("Intel");
