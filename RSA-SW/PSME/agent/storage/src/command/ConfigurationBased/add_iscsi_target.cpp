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

#include "agent-framework/command/storage/add_iscsi_target.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "agent-framework/module/iscsi_data.hpp"
#include "agent-framework/exceptions/exception.hpp"
#include "iscsi/manager.hpp"
#include "iscsi/response.hpp"
#include "iscsi/tgt/config/tgt_config.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;
using namespace agent::storage::iscsi::tgt::config;

/*! AddISCSITarget implementation */
class AddISCSITarget : public storage::AddISCSITarget {
public:
    AddISCSITarget() {}

    using storage::AddISCSITarget::execute;

    void execute(const Request& request, Response& response) {
        if (ModuleManager::get_modules().empty()) {
            THROW(agent_framework::exceptions::InvalidParameters,
                 "rpc", "Module not found!");
        }

        auto& module = ModuleManager::get_modules().front();

        if (module->get_submodules().empty()) {
            THROW(agent_framework::exceptions::InvalidParameters,
                 "rpc", "Submodule not found!");
        }

        auto& submodule = module->get_submodules().front();
        auto& target_manager = submodule->get_target_manager();
        const auto target_id = target_manager.get_new_target_id();
        agent::storage::iscsi::tgt::Manager manager;

        create_target(manager, target_id, request.get_target_iqn());
        set_initiator_iqn(manager, target_id, request.get_initiator_iqn());

        const auto& iscsi_data = submodule->get_iscsi_data();
        auto target = create_target_obj(target_id,
                                        request,
                                        iscsi_data);
        if (!create_luns(manager, target, request)) {
            delete_target(manager, target_id);
            THROW(agent_framework::exceptions::ISCSIError,
                  "rpc", "Create luns internal error");
        }

        target_manager.add_target(target);

        TgtConfig tgtConfig(iscsi_data.get_configuration_path());
        try {
            tgtConfig.add_target(target);
        }
        catch (std::exception& ex) {
            log_error(GET_LOGGER("tgt"), "Unable to create TGT target config file: " << ex.what());
        }

        response.set_target(target->get_uuid());
        response.set_target_address(target->get_target_address());
        response.set_target_port(target->get_target_port());
        response.set_oem_data({});
    }

    void create_target(agent::storage::iscsi::tgt::Manager& manager,
                                const std::int32_t target_id,
                                const std::string& target_iqn) {
        auto target_res = manager.create_target(target_id, target_iqn);
        if (!target_res.is_valid()) {
            THROW(agent_framework::exceptions::ISCSIError,
                  "rpc",
                  agent::storage::iscsi::tgt::Errors::get_error_str(
                                               target_res.get_error()));
        }
    }

    void delete_target(agent::storage::iscsi::tgt::Manager& manager,
                                        const std::int32_t target_id) {
        auto response = manager.destroy_target(target_id);

        if (!response.is_valid()) {
            log_error(GET_LOGGER("rpc"), "Destroy target error: " <<
            agent::storage::iscsi::tgt::Errors::get_error_str(
                                               response.get_error()));
        }
    }

    void set_initiator_iqn(agent::storage::iscsi::tgt::Manager& manager,
                                const std::int32_t target_id,
                                const std::string& initiator) {
        if (initiator.empty()) {
            log_debug(GET_LOGGER("rpc"), "No target initiator set");
            return;
        }
        agent::storage::iscsi::tgt::Manager::OptionMapper options;
        options.emplace(std::make_pair("initiator-address", initiator));
        auto response = manager.bind_target(target_id, options);
        if (!response.is_valid()) {
            log_error(GET_LOGGER("rpc"), "Cannot bind initiator to target " <<
            agent::storage::iscsi::tgt::Errors::get_error_str(
                                                        response.get_error()));
        }
    }

    bool create_luns(agent::storage::iscsi::tgt::Manager& manager,
                                                    TargetSharedPtr& target,
                                                    const Request& request) {
        for (const auto& target_lun : request.get_target_luns()) {
            auto drive =
                    ModuleManager::find_logical_drive(target_lun.get_drive());
            auto drive_ptr = drive.lock();
            if (!drive_ptr) {
                log_error(GET_LOGGER("rpc"), "Invalid logical drive uuid");
                return false;
            }

            auto lun_res = manager.create_lun(target->get_target_id(),
                                              target_lun.get_lun(),
                                            drive_ptr->get_device_path());
            if (!lun_res.is_valid()) {
                log_error(GET_LOGGER("rpc"), "Create lun error: " <<
                agent::storage::iscsi::tgt::Errors::get_error_str(
                                                    lun_res.get_error()));
                return false;
            }
            target->add_target_lun(create_lun_obj(
                        target_lun.get_lun(),
                        drive_ptr));

            target->add_logical_drive(drive_ptr);
        }
        return true;
    }

    Target::Lun
    create_lun_obj(const std::uint64_t lun,
                   const LogicalDrive::LogicalDriveSharedPtr& drive) {
        Target::Lun lun_obj;
        lun_obj.set_lun_id(lun);
        lun_obj.set_device_path(drive->get_device_path());
        lun_obj.set_logical_drive_uuid(drive->get_uuid());
        return lun_obj;
    }

    Target::TargetSharedPtr
    create_target_obj(const int32_t target_id,
                      const Request& request,
                      const IscsiData& iscsi_data) {
        auto target = std::make_shared<Target>();
        target->set_target_id(target_id);
        target->set_target_iqn(request.get_target_iqn());
        target->set_initiator_iqn(request.get_initiator_iqn());
        target->set_status({"Enabled", "OK"});
        target->set_target_address(iscsi_data.get_portal_ip());
        target->set_target_port(iscsi_data.get_portal_port());
        return target;
    }

    ~AddISCSITarget();
};

AddISCSITarget::~AddISCSITarget() {}


static Command::Register<AddISCSITarget> g("ConfigurationBased");
