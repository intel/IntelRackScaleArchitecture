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
*/

#include "service_factory.hpp"
#include "compute_service.hpp"
#include "storage_service.hpp"
#include "network_service.hpp"
#include "core/agent/agent_manager.hpp"

#include <exception>

using namespace psme::core::service;

namespace {
constexpr char COMPUTE[] = "Compute";
}

ComputeService ServiceFactory::create_compute(const std::string& gami_id) {
    const auto agent = psme::core::agent::AgentManager::get_instance().
                                                            get_agent(gami_id);
    if (agent) {
        return ComputeService{agent};
    }

    throw std::runtime_error("Agent not found for given id!");
}

StorageService ServiceFactory::create_storage(const std::string& gami_id) {
    const auto agent = psme::core::agent::AgentManager::get_instance().
        get_agent(gami_id);
    if (agent) {
        return StorageService{agent};
    }

    throw std::runtime_error("Agent not found for given id!");
}

NetworkService ServiceFactory::create_network(const std::string& gami_id) {
    const auto agent = psme::core::agent::AgentManager::get_instance().
        get_agent(gami_id);
    if (agent) {
        return NetworkService{agent};
    }

    throw std::runtime_error("Agent not found for given id!");
}

std::vector<ComputeService> ServiceFactory::get_all_computes() {
    std::vector<ComputeService> computes;
    const auto agents = psme::core::agent::AgentManager::get_instance().
                                get_agents_by_capability(std::string(COMPUTE));
    for (const auto& agent : agents) {
        computes.emplace_back(agent);
    }
    return computes;
}
