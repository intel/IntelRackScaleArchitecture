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

#include "agent-framework/command/compute/get_storage_controller_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! GetStorageControllerInfo implementation */
class GetStorageControllerInfo : public compute::GetStorageControllerInfo {
public:
    GetStorageControllerInfo() {}

    using compute::GetStorageControllerInfo::execute;

    void execute(const Request& request, Response& response) {
        using std::out_of_range;

        auto component_uuid = request.get_component();
        auto controller_id = request.get_controller();

        auto submodule = ModuleManager::get_submodule(component_uuid);
        auto submodule_is_absent = submodule == nullptr || !submodule->is_present();
        if (submodule_is_absent) {
            log_warning(GET_LOGGER("rpc"), "Component uuid=" << component_uuid << " not found.");
            throw exception::NotFound();
        }

        auto& storage_controllers = submodule->get_storage_controllers();

        try {
            auto& controller = storage_controllers.at(controller_id);
            response.set_drive_count(controller->get_drive_count());
            response.set_interface(controller->get_interface());
            response.set_status(controller->get_status());
            response.set_fru_info(controller->get_fru_info());
        } catch (const out_of_range&) {
            log_warning(GET_LOGGER("rpc"), "Controller controller=" << controller_id << " not found.");
            throw exception::NotFound();
        }
    }


    ~GetStorageControllerInfo();
};

GetStorageControllerInfo::~GetStorageControllerInfo() {}

static Command::Register<GetStorageControllerInfo> g("Intel");
