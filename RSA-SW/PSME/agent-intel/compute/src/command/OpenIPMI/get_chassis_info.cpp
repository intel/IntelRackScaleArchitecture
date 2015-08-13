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

#include "agent-framework/command/compute/get_chassis_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::generic;
using namespace agent_framework::command;

using agent_framework::command::exception::NotFound;

class GetChassisInfo : public agent_framework::command::compute::GetChassisInfo {
public:
    GetChassisInfo() { }

    using agent_framework::command::compute::GetChassisInfo::execute;

    void execute(const Request& request, Response& response) {
        auto chassis = ModuleManager::get_chassis();
        auto& chassis_uuid = request.get_chassis();
        if (nullptr == chassis || chassis->get_uuid() != chassis_uuid) {
            throw NotFound();
        }

        response.set_status({"Enabled", "OK"});
        response.set_type("Drawer");
        response.set_size(chassis->get_size());
        response.set_location_offset(chassis->get_location_offset());
        response.set_fru_info({ "", "", "", ""});
        response.set_oem({});

        response.add_collection({"ComputeZones", "ComputeZones", ""});
    }

    ~GetChassisInfo();

private:
};

GetChassisInfo::~GetChassisInfo() { }

static Command::Register<GetChassisInfo> g("Intel");
