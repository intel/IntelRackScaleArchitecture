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

#include "agent-framework/command/compute/get_drive_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

using std::out_of_range;

/*! GetDriveInfo implementation */
class GetDriveInfo : public compute::GetDriveInfo {
public:
    GetDriveInfo() {}

    using compute::GetDriveInfo::execute;

    void execute(const Request& request, Response& response) {
        auto drive_id = request.get_drive();
        auto component_uuid = request.get_component();
        auto controller_id = request.get_controller();

        auto submodule = ModuleManager::get_submodule(component_uuid);
        auto submodule_is_absent = nullptr == submodule || !submodule->is_present();
        if (submodule_is_absent) {
            log_warning(GET_LOGGER("rpc"), "Component uuid=" << component_uuid << " not found.");
            throw exception::NotFound();
        }

        if (!element_exists(submodule->get_storage_controllers(), controller_id)) {
            log_warning(GET_LOGGER("rpc"), "StorageController controller=" << controller_id << " not found.");
            throw exception::NotFound();
        }

        auto& storage_controller = submodule->get_storage_controllers().at(controller_id);
        if (!element_exists(storage_controller->get_hard_drives(), drive_id)) {
            log_warning(GET_LOGGER("rpc"), "Drive drive=" << drive_id << " not found.");
            throw exception::NotFound();
        }

        auto& hard_drive = storage_controller->get_hard_drives().at(drive_id);

        response.set_type(hard_drive->get_type());
        response.set_status(hard_drive->get_status());
        response.set_interface(hard_drive->get_interface());
        response.set_fru_info(hard_drive->get_fru_info());
        response.set_capacity_gb(hard_drive->get_capacity_gb());
        response.set_rpm(hard_drive->get_rpm());
    }

    template<class Container>
    bool element_exists(const Container& container, unsigned int index) const {
        try {
            container.at(index);
        } catch (const out_of_range&) { return false; }

        return true;
    }

    ~GetDriveInfo();
};

GetDriveInfo::~GetDriveInfo() {}


static Command::Register<GetDriveInfo> g("Intel");
