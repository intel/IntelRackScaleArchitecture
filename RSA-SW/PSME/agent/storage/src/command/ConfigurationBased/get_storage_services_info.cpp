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

#include "agent-framework/command/storage/get_storage_services_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

using std::out_of_range;

/*! GetStorageServicesInfo implementation */
class GetStorageServicesInfo : public storage::GetStorageServicesInfo {
public:

    GetStorageServicesInfo() {}

    using storage::GetStorageServicesInfo::execute;

    void execute(const Request& request, Response& response) {
        auto services = request.get_services();

        auto submodule = ModuleManager::get_submodule(services);
        if (nullptr == submodule) {
            log_warning(GET_LOGGER("rpc"), "Services uuid="
                        << services << " not found.");
            throw exception::NotFound();
        }

        response.add_collection(
            Collection::CollectionEntry(
                StorageServicesCollections::PHYSICAL_DRIVES_NAME,
                StorageServicesCollections::PHYSICAL_DRIVES_TYPE));
        response.add_collection(
            Collection::CollectionEntry(
                StorageServicesCollections::LOGICAL_DRIVES_NAME,
                StorageServicesCollections::LOGICAL_DRIVES_TYPE));
        response.add_collection(
            Collection::CollectionEntry(
                StorageServicesCollections::ISCSI_TARGETS_NAME,
                StorageServicesCollections::ISCSI_TARGETS_TYPE));

        response.set_status({"Enabled", "OK"});
    }

    ~GetStorageServicesInfo();
};

GetStorageServicesInfo::~GetStorageServicesInfo() {}


static Command::Register<GetStorageServicesInfo> g("ConfigurationBased");
