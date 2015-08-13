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

#include "agent-framework/command/storage/get_physical_drive_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! GetPhysicalDriveInfo implementation */
class GetPhysicalDriveInfo : public storage::GetPhysicalDriveInfo {
public:
    GetPhysicalDriveInfo() {}

    using storage::GetPhysicalDriveInfo::execute;

    void execute(const Request& request, Response& response) {
        auto drive = request.get_drive();

        auto hard_drive = ModuleManager::find_hard_drive(drive).lock();
        if (!hard_drive) {
            log_warning(GET_LOGGER("rpc"), "Drive drive=" << drive << " not found.");
            throw exception::NotFound();
        }

        // TODO hardcoded
        response.set_interface("SATA");
        response.set_capacity_gb(hard_drive->get_capacity_gb());
        response.set_type(hard_drive->get_type());
        response.set_rpm(hard_drive->get_rpm());
        response.set_fru_info(hard_drive->get_fru_info());
        response.set_status(hard_drive->get_status());
    }

    ~GetPhysicalDriveInfo();
};

GetPhysicalDriveInfo::~GetPhysicalDriveInfo() {}


static Command::Register<GetPhysicalDriveInfo> g("ConfigurationBased");
