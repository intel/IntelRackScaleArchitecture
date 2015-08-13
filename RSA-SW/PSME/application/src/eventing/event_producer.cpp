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
 *
 * */

#include "event_producer.hpp"
#include "core/service/compute_service.hpp"
#include "core/agent/agent_manager.hpp"
#include "core/agent/agent_unreachable.hpp"
#include "eventing/eventing_data_queue.hpp"
#include "logger_ext.hpp"

#include <algorithm>
#include <chrono>

using namespace psme::app::eventing;

using psme::command::eventing::EventingAgent;
using psme::core::agent::AgentManager;
using psme::core::agent::AgentUnreachable;
using psme::core::agent::AgentSharedPtr;
using psme::core::service::ComputeService;
using std::string;

namespace {

UuidSet poll_uuids(const AgentSharedPtr& agent) {
    log_debug(GET_LOGGER("rest"), " Polling " << agent->get_gami_id());

    ComputeService service{agent};

    UuidSet polled_uuids;
    // Spec diverged for Storage agent
    // we use different command to query components
    if (agent->has_capability("Storage")) {
        auto result = service.get_component_collection();
        if (result.is_valid()) {
            for (const auto& component : result.get_components()) {
                polled_uuids.emplace(component.get_component());
            }
        }
    } else {
        auto result = service.get_components();
        if (result.is_valid()) {
            for (const auto& component : result.get_components()) {
                polled_uuids.emplace(component.get_name());
            }
        }
    }
    return polled_uuids;
}

UuidSet uuids_intersection(const UuidSet& first, const UuidSet& second) {
    UuidSet intersection;
    std::set_intersection(first.begin(), first.end(),
                          second.begin(), second.end(),
                          std::inserter(intersection, intersection.end()));
    return intersection;
}

UuidSet uuids_difference(const UuidSet& first, const UuidSet& second) {
    UuidSet diff;
    std::set_difference(first.begin(), first.end(),
                        second.begin(), second.end(),
                        std::inserter(diff, diff.end()));
    return diff;
}

void produce_add_events(const std::string& gami_id, const UuidSet& uuids) {
    using psme::command::eventing::EventingAgent;
    for (const auto& uuid: uuids) {
        EventingAgent::Request request(gami_id, uuid, "Enabled", "DISCOVERY_UP");
        psme::app::eventing::EventingDataQueue::get_instance()->
                        push_back(request);
    }
}

void produce_remove_events(const std::string& gami_id, const UuidSet& uuids) {
    using psme::command::eventing::EventingAgent;
    for (const auto& uuid: uuids) {
        EventingAgent::Request request(gami_id, uuid, "Absent", "EXTRACTION");
        psme::app::eventing::EventingDataQueue::get_instance()->
                        push_back(request);
    }
}

void produce_update_events(const std::string& gami_id, const UuidSet& uuids) {
    using psme::command::eventing::EventingAgent;
    for (const auto& uuid: uuids) {
        EventingAgent::Request request(gami_id, uuid, "Enabled", "UPDATE");
        psme::app::eventing::EventingDataQueue::get_instance()->
                        push_back(request);
    }
}
}

EventProducer::EventProducer(const ::json::Value& config)
    : m_config(config), m_running(false), m_thread(), m_stored_uuids() { }

void EventProducer::start() {
    if (!m_config["eventing"]["enabled"].as_bool()) {
        if (!m_running) {
            m_running = true;
            m_thread = std::thread(&EventProducer::produce, this);
        }
    }
}

void EventProducer::stop() {
    if (m_running) {
        m_running = false;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }
}

void EventProducer::produce() {
    static const auto SLEEP_TIME =
                    m_config["eventing"]["poll-interval-sec"].as_uint();

    while (m_running) {
        do_poll();
        do_clean();
        // rest a little
        std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME));
    }
}

const UuidSet& EventProducer::get_stored_uuids(const string& gami_id) {
    const auto it = m_stored_uuids.find(gami_id);
    if (it == m_stored_uuids.end()) {
        return m_stored_uuids.emplace(gami_id, UuidSet{}).first->second;
    } else {
        return it->second;
    }
}

void EventProducer::do_poll() {
    using psme::core::service::ComputeService;
    using psme::core::dto::ComponentDTO;

    const auto agents = AgentManager::get_instance().get_agents();

    log_debug(GET_LOGGER("rest"),
            " Number of connected agents: " << agents.size());

    for (const auto& agent : agents) {
        const auto& gami_id = agent->get_gami_id();
        try {
            auto polled_uuids = poll_uuids(agent);
            const auto& stored_uuids = get_stored_uuids(gami_id);

            auto common_uuids = uuids_intersection(stored_uuids, polled_uuids);

            // components to remove
            auto uuids_to_remove = uuids_difference(stored_uuids, common_uuids);
            produce_remove_events(gami_id, uuids_to_remove);

            // components to add
            auto uuids_to_add = uuids_difference(polled_uuids, common_uuids);
            produce_add_events(gami_id, uuids_to_add);

            // components to update
            produce_update_events(gami_id, common_uuids);

            // update stored uuids
            m_stored_uuids[gami_id] = std::move(polled_uuids);
        } catch (...) {
            log_error(GET_LOGGER("rest"),
                " Error occured while polling agent " << gami_id);
        }
    }
}

void EventProducer::do_clean() noexcept {
    static const auto TIMEOUT =
                    m_config["eventing"]["poll-interval-sec"].as_uint() * 2;
    // clean stored uuids of disconnected agents
    auto disconnected_agents =
            AgentManager::get_instance().remove_disconnected_agents(TIMEOUT);

    for (const auto& agent : disconnected_agents) {
        auto it = m_stored_uuids.find(agent->get_gami_id());
        if (it != m_stored_uuids.end()) {
            produce_remove_events(agent->get_gami_id(), it->second);
            m_stored_uuids.erase(it);
        }
    }
}
