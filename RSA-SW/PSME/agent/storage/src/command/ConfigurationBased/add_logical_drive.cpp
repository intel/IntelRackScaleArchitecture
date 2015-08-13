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

#include "agent-framework/command/storage/add_logical_drive.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "agent-framework/action/task_runner.hpp"
#include "agent-framework/exceptions/exception.hpp"
#include "lvm/lvm_api.hpp"
#include "lvm/lvm_create_data.hpp"
#include "lvm/lvm_clone_task.hpp"

using namespace agent_framework::action;
using namespace agent_framework::command;
using namespace agent_framework::generic;
using namespace agent::storage::lvm;

/*! AddLogicalDrive implementation */
class AddLogicalDrive : public storage::AddLogicalDrive {
public:
    AddLogicalDrive() {}

    using storage::AddLogicalDrive::execute;

    void execute(const Request& request, Response& response) {
        const auto master_drive =
                ModuleManager::find_logical_drive(request.get_master()).lock();

        if (!master_drive) {
            THROW(agent_framework::exceptions::InvalidParameters,
                  "rpc", "Master logical drive not found");
        }
        log_debug(GET_LOGGER("rpc"),
                        "Master drive name = " << master_drive->get_name() );

        /*
         * volume group is taken from masters volume group in current implementation
         * instead of from request
         */
        LogicalDriveSharedPtr volume_group = master_drive->get_volume_group();
        if (nullptr == volume_group) {
            THROW(agent_framework::exceptions::LvmError,
                  "rpc", "No volume group for master drive");
        }

        const std::string& volume_group_name {volume_group->get_name()};
        auto logical_drive = LogicalDrive::make_logical_drive();
        const std::string& name{logical_drive->get_uuid()};
        Status lvm_status{};
        lvm_status.set_health("OK");
        LvmAPI lvm_api{};
        if (request.is_snapshot()) {
            lvm_api.create_snapshot(volume_group_name.c_str(),
                                                  master_drive->get_name().c_str(),
                                                  name.c_str(),
                                                  request.get_capacity_bytes());
            lvm_status.set_state("Enabled");
        } else {
            lvm_api.create_clone(volume_group_name.c_str(),
                                                  master_drive->get_name().c_str(),
                                                  name.c_str(),
                                                  request.get_capacity_bytes());
            lvm_status.set_state("Starting");

            LvmCreateData create_data{};
            create_data.set_uuid(logical_drive->get_uuid());
            create_data.set_volume_group(volume_group_name);
            create_data.set_create_name(logical_drive->get_uuid());
            create_data.set_size(request.get_capacity_bytes());
            create_data.set_logical_volume(master_drive->get_name());

            TaskRunner::get_instance().run(LvmCloneTask{create_data});
        }

        // add entry in Volume group logical drives list
        logical_drive->set_bootable(false);
        logical_drive->set_protected(request.is_protected());
        logical_drive->set_capacity_gb(request.get_capacity_gb());
        logical_drive->set_type(request.get_type());
        logical_drive->set_volume_group(volume_group);
        logical_drive->set_status(lvm_status);
        logical_drive->set_name(name);
        logical_drive->set_mode(request.get_mode());
        logical_drive->set_device_path("/dev/" + volume_group_name + "/" + name);
        logical_drive->set_master(request.get_master());
        logical_drive->set_snapshot(request.is_snapshot());

        volume_group->add_logical_drive(logical_drive);
        response.set_drive_uuid(logical_drive->get_uuid());
        response.set_oem({});

        log_debug(GET_LOGGER("rpc"),
            "Logical Volume " << logical_drive->get_uuid() << " created." );
    }


    ~AddLogicalDrive();
};

AddLogicalDrive::~AddLogicalDrive() {}


static Command::Register<AddLogicalDrive> g("ConfigurationBased");
