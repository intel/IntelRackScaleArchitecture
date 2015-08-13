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

#include "agent_manager.hpp"
#include "jsonrpc_agent.hpp"
#include "logger/logger_factory.hpp"
#include <algorithm>

using namespace psme::core::agent;

std::once_flag AgentManager::m_once_flag{};

namespace {
AgentManager* g_agent_manager = nullptr;
}

AgentManager& AgentManager::get_instance() {
    std::call_once(m_once_flag, &AgentManager::init);
    return *g_agent_manager;
}

void AgentManager::cleanup() {
    if (g_agent_manager) {
        delete g_agent_manager;
        g_agent_manager = nullptr;
    }
}

void AgentManager::init() {
    g_agent_manager = new AgentManager;
}

AgentSharedPtr AgentManager::get_agent(const std::string& gami_id) const {
    std::lock_guard<std::mutex> lock{m_mutex};
    for (const auto& agent : m_agents) {
        if (agent->get_gami_id() == gami_id) {
            return agent;
        }
    }
    throw std::runtime_error("Cannot find agent for gamiId : '" + gami_id + "'");
}

void AgentManager::add_agent(AgentSharedPtr agent) {
    std::lock_guard<std::mutex> lock{m_mutex};
    m_agents.push_back(agent);
}

void AgentManager::remove_agent(const std::string& gami_id) {
    std::lock_guard<std::mutex> lock{m_mutex};
    auto comparer = [&gami_id](const AgentSharedPtr& agent) {
                        return agent->get_gami_id() == gami_id;
                    };

    m_agents.erase(std::remove_if(m_agents.begin(), m_agents.end(), comparer),
                   m_agents.end());
}

AgentManager::AgentsList AgentManager::get_agents_by_capability(
                                            const std::string& capability) {
    std::lock_guard<std::mutex> lock{m_mutex};

    AgentsList agents;
    for (const auto& agent : m_agents) {
        if (agent->has_capability(capability)) {
            agents.push_back(agent);
        }
    }
    return agents;
}

AgentManager::AgentsList AgentManager::get_agents() {
    std::lock_guard<std::mutex> lock{m_mutex};

    return m_agents;
}

AgentManager::AgentsList
AgentManager::remove_disconnected_agents(const std::uint32_t timeout) {
    std::lock_guard<std::mutex> lock{m_mutex};

    auto it = std::remove_if(m_agents.begin(), m_agents.end(),
            [&timeout](const AgentSharedPtr& a){
        auto unreachable_seconds = a->get_invoker().get_connection_status().second;
        if (unreachable_seconds > timeout) {
            log_warning(GET_LOGGER("rest"),
                    " Agent disconnected -> " << a->get_gami_id()
                      << " on " << a->get_ipv4address() << ":" << a->get_port());
            return true;
        }
        return false;
    });
    AgentsList agents(it, m_agents.end());
    m_agents.erase(it, m_agents.end());
    return agents;
}
