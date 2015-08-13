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

#include "agent-framework/command/compute/get_compute_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::generic;
using namespace agent_framework::command;


class GetComputeInfo : public agent_framework::command::compute::GetComputeInfo {
public:
    GetComputeInfo() { }

    using agent_framework::command::compute::GetComputeInfo::execute;

    void execute(const Request& request, Response& response) {
        auto module = ModuleManager::get_module(request.get_compute());
        if (nullptr == module) {
            throw agent_framework::command::exception::NotFound();
        }

        response.set_thermal_zone("");
        response.set_power_zone("");
        response.set_status({"Enabled", "OK"});
        response.set_slot(module->get_slot());
        response.set_fru_info({"", "", "",""});
        response.set_oem({});
    }

    ~GetComputeInfo();
};

GetComputeInfo::~GetComputeInfo() { }

static Command::Register<GetComputeInfo> g("Intel");
