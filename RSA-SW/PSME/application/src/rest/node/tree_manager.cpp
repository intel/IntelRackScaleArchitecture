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

#include "psme/rest/node/tree_manager.hpp"
#include "psme/rest/node/node.hpp"
#include "psme/rest/node/crud/root.hpp"
#include "psme/rest/node/crud/rest.hpp"
#include "psme/rest/node/crud/version.hpp"
#include "psme/rest/node/crud/drawers.hpp"
#include "psme/rest/node/crud/managers.hpp"
#include "psme/rest/node/crud/services.hpp"
#include "psme/rest/resource/resource.hpp"
#include "core/agent/agent_manager.hpp"
#include "eventing/eventing_data_queue.hpp"
#include "logger_ext.hpp"
#include "bits/algorithmfwd.h"
#include "psme/rest/http/server.hpp"
#include "psme/rest/node/builders/drawer_node_builder.hpp"
#include "psme/rest/node/builders/compute_node_builder.hpp"
#include "psme/rest/node/builders/network_node_builder.hpp"
#include "psme/rest/node/builders/storage_node_builder.hpp"

#include "json/json.hpp"

#include <atomic>
#include <set>
#include <mutex>
#include <thread>

using namespace psme::rest::node;
using namespace psme::rest::resource;
using psme::app::eventing::EventingDataQueue;
using psme::command::eventing::EventingAgent;
using psme::core::agent::AgentManager;

namespace {

NodeSharedPtr
build_root(const json::Value& config) {

    auto root = std::make_shared<Root>();
    auto rest = std::make_shared<Rest>();
    root->add_node(rest);
    auto version = std::make_shared<Version>();
    version->handle_service_uuid(config);
    rest->add_node(version);
    auto drawers = std::make_shared<Drawers>();
    NodeBuilder::link_nodes(
        LinkType::COMPOSITION, Resource::CHASSIS, *version, drawers);
    auto managers = std::make_shared<Managers>();
    managers->set_odata_context(config);
    NodeBuilder::link_nodes(
            LinkType::COMPOSITION, Managers::TYPE, *version, managers);
    auto services = std::make_shared<Services>();
    NodeBuilder::link_nodes(
        LinkType::COMPOSITION, Services::TYPE, *version, services);

    return root;
}

}

class TreeManager::EventBasedImpl {
public:
    EventBasedImpl(const json::Value& config);

    ~EventBasedImpl();

    /*!
     * @brief Start thread updating tree
     */
    void start() {
        if (!m_running) {
            m_running = true;
            m_thread = std::thread(&EventBasedImpl::m_handle_events, this);
        }
    }

    /*!
     * @brief Stop thread updating tree
     */
    void stop() {
        if (m_running) {
            m_running = false;
            if (m_thread.joinable()) {
                m_thread.join();
            }
        }
    }

    void get(const Request& request, Response & response) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto& node = m_root->get_node_by_id(request.get_url());
        node.get(request, response);
    }

    void del(const Request& request, Response & response) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto& node = m_root->get_node_by_id(request.get_url());
        node.del(request, response);
    }

    void put(const Request& request, Response & response) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto& node = m_root->get_node_by_id(request.get_url());
        node.put(request, response);
    }

    void post(const Request& request, Response & response) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto& node = m_root->get_node_by_id(request.get_url());
        node.post(request, response);
    }

    void patch(const Request& request, Response & response) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto& node = m_root->get_node_by_id(request.get_url());
        node.patch(request, response);
    }

    void head(const Request& request, Response & response) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto& node = m_root->get_node_by_id(request.get_url());
        node.head(request, response);
    }

private:
    EventBasedImpl(const EventBasedImpl&) = delete;
    EventBasedImpl& operator=(const EventBasedImpl&) = delete;

    /*! @brief Enumerates event types */
    enum class EventType {
        ADD,
        REMOVE,
        UPDATE,
        UNKNOWN
    };

    EventType get_event_type(const EventingAgent::Request& event);

    /*!
     * @brief Updates tree in separate thread based on events.
     **/
    void m_handle_events();

    void handle_add_event(const EventingAgent::Request& event);
    void handle_remove_event(const EventingAgent::Request& event);
    void handle_update_event(const EventingAgent::Request& event);

    NodeBuilderUPtr create_node_builder(AgentSharedPtr agent);

private:
    const json::Value& m_config;
    /*! @brief Root of managed tree. */
    NodeSharedPtr m_root;
    std::thread m_thread;
    std::atomic<bool> m_running;
    // todo: compare with shared_mutex
    std::mutex m_mutex;
};

TreeManager::EventBasedImpl::EventBasedImpl(const json::Value& config)
    : m_config(config),
      m_root(build_root(config)),
      m_thread(), m_running(false), m_mutex() {

    DrawerNodeBuilder builder(m_config);
    auto nodes_to_link = builder.build_nodes(*m_root, "RSA Drawer");
    for (auto& link : nodes_to_link) {
        NodeBuilder::link_nodes(link.m_link_type,
                                link.m_first_link_name,
                                link.m_first,
                                link.m_second,
                                link.m_second_link_name);
    }
}

TreeManager::EventBasedImpl::~EventBasedImpl() {
    stop();
}

void
TreeManager::EventBasedImpl::m_handle_events() {
    static const std::size_t QUEUE_WAIT_TIME = 1000;

    while (m_running) {
        if (const auto event =
                EventingDataQueue::get_instance()->wait_for_and_pop(
                std::chrono::milliseconds(QUEUE_WAIT_TIME))) {
            try {
                auto event_type = get_event_type(*event);

                switch (event_type) {
                    case EventType::ADD:
                        handle_add_event(*event);
                        break;
                    case EventType::REMOVE:
                        handle_remove_event(*event);
                        break;
                    case EventType::UPDATE:
                        handle_update_event(*event);
                        break;
                    case EventType::UNKNOWN:
                    default:
                        log_debug(GET_LOGGER("rest"), " UNKNOWN event: " << *event);
                        break;
                };
            } catch (...) {
                log_error(GET_LOGGER("rest"),
                        " Exception occured when processing event: " << *event);
            }
        }
    }
}

TreeManager::EventBasedImpl::EventType
TreeManager::EventBasedImpl::get_event_type(const EventingAgent::Request& msg) {
    if ( 0 == msg.get_state().compare("Enabled")
         && (0 == msg.get_transition().compare("DISCOVERY_UP")
             || 0 == msg.get_transition().compare("CAME_UP"))) {
        return EventType::ADD;
    }

    if ( 0 == msg.get_state().compare("Absent")
         && 0 == msg.get_transition().compare("EXTRACTION")) {
        return EventType::REMOVE;
    }

    if ( 0 == msg.get_state().compare("Enabled")
         && (0 == msg.get_transition().compare("UPDATE"))) {
        return EventType::UPDATE;
    }

    return EventType::UNKNOWN;
}

void
TreeManager::EventBasedImpl::handle_remove_event(const EventingAgent::Request& event) {
    log_debug(GET_LOGGER("rest"), " Remove event handler");

    const string& component_id = event.get_id();
    auto* found = m_root->find_if([&component_id](const Node & node) {
        return 0 == node.get_uuid().compare(component_id);
    });

    if (nullptr != found) {
        // exclusive access for tree structure update
        std::lock_guard<std::mutex> lock(m_mutex);

        // remove managers of node and it's children
        found->for_each([this](Node& n) {
            for (const auto& link : n.get_links()) {
                if (link.m_node->get_type() == Manager::TYPE) {
                    m_root->erase(*(link.m_node));
                }
            }
        });
        // remove node
        m_root->erase(*found);
    }
    else {
        log_warning(GET_LOGGER("rest"), " Remove event handler: component ["
                + component_id + "] NOT FOUND in tree.");
    }
}

NodeBuilderUPtr
TreeManager::EventBasedImpl::create_node_builder(AgentSharedPtr agent) {
    if (agent->has_capability("Compute")) {
        return NodeBuilderUPtr(new ComputeNodeBuilder(agent));
    }
    else if (agent->has_capability("Network")) {
        return NodeBuilderUPtr(new NetworkNodeBuilder(agent));
    }
    else if (agent->has_capability("Storage")) {
        return NodeBuilderUPtr(new StorageNodeBuilder(agent));
    }
    throw std::runtime_error("Unknown agent type.");
}

void
TreeManager::EventBasedImpl::handle_add_event(const EventingAgent::Request& event) {
    log_debug(GET_LOGGER("rest"), " Add event handler");

    const string& component_id = event.get_id();

    auto agent = AgentManager::get_instance().get_agent(event.get_gami_id());
    auto node_builder = create_node_builder(agent);

    // nodes discovery
    auto nodes_to_link = node_builder->build_nodes(*m_root, component_id);

    if (!nodes_to_link.empty()) {
        // exclusive access for tree structure update
        std::lock_guard<std::mutex> lock(m_mutex);

        // create links between nodes
        for (auto& link : nodes_to_link) {
            NodeBuilder::link_nodes(link.m_link_type,
                                    link.m_first_link_name,
                                    link.m_first,
                                    link.m_second,
                                    link.m_second_link_name);
        }
    }
}

void
TreeManager::EventBasedImpl::handle_update_event(const EventingAgent::Request& event) {
    log_debug(GET_LOGGER("rest"), " Update event handler");

    auto agent = AgentManager::get_instance().get_agent(event.get_gami_id());
    // todo: compute and network agent.
    if (agent->has_capability("Storage")) {
        handle_add_event(event);
    }
}

TreeManager::TreeManager(const json::Value& config)
    : m_impl(new TreeManager::EventBasedImpl(config)) { }

TreeManager::~TreeManager() { }

void
TreeManager::start() {
    m_impl->start();
}

void
TreeManager::stop() {
    m_impl->stop();
}

void
TreeManager::get(const Request& request, Response & response) {
    m_impl->get(request, response);
}

void
TreeManager::del(const Request& request, Response & response) {
    m_impl->del(request, response);
}

void
TreeManager::put(const Request& request, Response & response) {
    m_impl->put(request, response);
}

void
TreeManager::post(const Request& request, Response & response) {
    m_impl->post(request, response);
}

void
TreeManager::patch(const Request& request, Response & response) {
    m_impl->patch(request, response);
}

void
TreeManager::head(const Request& request, Response & response) {
    m_impl->head(request, response);
}
