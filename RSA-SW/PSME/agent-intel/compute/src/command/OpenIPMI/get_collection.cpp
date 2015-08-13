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
#include "compute_collections.hpp"

#include "agent-framework/command/compute/get_collection.hpp"
#include "agent-framework/module/module_manager.hpp"

using std::vector;

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! GetCollection implementation */
class GetCollection : public compute::GetCollection {
public:
    GetCollection() {}

    using compute::GetCollection::execute;

    void execute(const Request& request, Response& response) {

        auto uuid = request.get_component();
        auto collection_name = request.get_name();

        auto chassis = ModuleManager::get_chassis();
        if (nullptr != chassis && chassis->get_uuid() == uuid) {
            m_add_collection(chassis, collection_name, response);
            return;
        }

        log_warning(GET_LOGGER("rpc"), "Component uuid=" << uuid << " not found.");
        throw exception::NotFound();
    }

    ~GetCollection();

private:

    void m_add_collection(const Chassis* component, const string& collection_name,
                          Response& response) {
        if (ComputeCollections::COMPUTE_ZONES_NAME ==
            collection_name) {
            m_add_compute_zones(component, response);
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

    void m_add_compute_zones(const Chassis* component, Response& response) {
        const auto& compute_zones = component->get_compute_zones();
        for (const auto& compute_zone : compute_zones) {
            response.add_subcomponent(
                Response::Subcomponent(compute_zone->get_uuid()));
        }
    }
};

GetCollection::~GetCollection() {}

static Command::Register<GetCollection> g("Intel");
