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

#include "agent-framework/command/storage/delete_iscsi_target.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "agent-framework/exceptions/exception.hpp"
#include "iscsi/manager.hpp"
#include "iscsi/response.hpp"
#include "iscsi/tgt/config/tgt_config.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;
using namespace agent::storage::iscsi::tgt::config;

/*! DeleteISCSITarget implementation */
class DeleteISCSITarget : public storage::DeleteISCSITarget {
public:
    DeleteISCSITarget() {}

    using storage::DeleteISCSITarget::execute;

    void execute(const Request& request, Response& response) {
        const auto target_uuid = request.get_target();
        const auto target_obj = ModuleManager::find_target(target_uuid).lock();
        if (!target_obj) {
            THROW(agent_framework::exceptions::InvalidParameters,
                  "rpc", "Target not found");
        }

        ::agent::storage::iscsi::tgt::Manager manager;
        auto tgt_response = manager.destroy_target(target_obj->get_target_id());

        if (!tgt_response.is_valid()) {
            THROW(agent_framework::exceptions::ISCSIError,
                  "rpc", "Cannot destroy target " +
            agent::storage::iscsi::tgt::Errors::get_error_str(
                                               tgt_response.get_error()));
        }
        remove_from_model(target_uuid);
        response.set_oem({});
    }


    ~DeleteISCSITarget();

private:
    void remove_from_model(const std::string& target_uuid) {
        for (const auto& module : ModuleManager::get_modules()) {
            for(auto& submodule : module->get_submodules()) {
                auto& target_manager = submodule->get_target_manager();
                for(const auto& target : target_manager.get_targets()) {
                    if (target && target->get_uuid() == target_uuid) {
                        remove_tgt_config_file(submodule,
                                               target->get_target_id());
                        target_manager.remove_target(target);
                        return;
                    }
                }
            }
        }
    }

    void remove_tgt_config_file(const SubmoduleUniquePtr& submodule,
                                const std::int32_t target_id) {
        const auto& iscsi_data = submodule->get_iscsi_data();
        TgtConfig tgtConfig(iscsi_data.get_configuration_path());
        try {
            tgtConfig.remove_target(target_id);
        } catch (const std::exception& ex) {
            log_error(GET_LOGGER("tgt"),
                    "Unable to remove TGT target config file: " << ex.what());
        }
    }
};

DeleteISCSITarget::~DeleteISCSITarget() {}


static Command::Register<DeleteISCSITarget> g("ConfigurationBased");
