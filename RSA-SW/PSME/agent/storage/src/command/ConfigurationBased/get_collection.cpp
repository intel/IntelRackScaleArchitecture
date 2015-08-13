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
#include "storage_services_collections.hpp"

#include "agent-framework/command/storage/get_collection.hpp"
#include "agent-framework/module/module_manager.hpp"

using std::vector;

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! GetCollection implementation */
class GetCollection : public storage::GetCollection {
public:
    GetCollection() {}

    using storage::GetCollection::execute;

    void execute(const Request& request, Response& response) {

        auto uuid = request.get_component();
        auto collection_name = request.get_name();

        auto submodule = ModuleManager::get_submodule(uuid);
        if (nullptr != submodule) {
            m_add_collection(submodule, collection_name, response);
            return;
        }

        auto logical_drive = ModuleManager::find_logical_drive(uuid).lock();
        if (logical_drive) {
            m_add_collection(logical_drive.get(), collection_name, response);
            return;
        }

        auto target = ModuleManager::find_target(uuid).lock();
        if (target) {
            m_add_collection(target.get(), collection_name, response);
            return;
        }

        log_warning(GET_LOGGER("rpc"), "Component uuid=" << uuid << " not found.");
        throw exception::NotFound();
    }

    ~GetCollection();

private:

    template<typename T>
    void m_add_collection(const T* component, const string& collection_name,
                          Response& response) {
        if (StorageServicesCollections::PHYSICAL_DRIVES_NAME ==
            collection_name) {
            m_add_physical_drives(component, response);
        }
        else if (StorageServicesCollections::LOGICAL_DRIVES_NAME ==
                 collection_name) {
            m_add_logical_drives(component, response);
        }
        else if (StorageServicesCollections::ISCSI_TARGETS_NAME ==
                 collection_name) {
            m_add_iscsi_targets(component, response);
        }
        else {
            log_warning(GET_LOGGER("rpc"),
                        "Collection " << collection_name << " not found.");
            throw exception::NotFound();
        }

        log_debug(GET_LOGGER("rpc"), "Getting collection "
                  << collection_name
                  << " is successfully finished.");
    }

    template<typename T>
    void m_add_physical_drives(const T* component, Response& response) {
        const auto& hard_drives = component->get_hard_drives();
        for (const auto& drive : hard_drives) {
            response.add_subcomponent(
                Response::Subcomponent(drive->get_uuid()));
        }
    }

    template<typename T>
    void m_add_logical_drives(const T* component, Response& response) {
        for (const auto& logical_drive : component->get_logical_drives()) {
            response.add_subcomponent(
                Response::Subcomponent(logical_drive->get_uuid()));
        }
    }

    template<typename T>
    void m_add_iscsi_targets(const T* component, Response& response) {
        for (const auto& target : component->get_targets()) {
            response.add_subcomponent(
                Response::Subcomponent(target->get_uuid()));
        }
    }
};

GetCollection::~GetCollection() {}

template<>
void GetCollection::m_add_iscsi_targets(const LogicalDrive*, Response&) {}

template<>
void GetCollection::m_add_iscsi_targets(const Target*, Response&) {}

static Command::Register<GetCollection> g("ConfigurationBased");
