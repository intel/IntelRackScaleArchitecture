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
 * @file command/registration/definition/register_agent.cpp
 * @brief RegisterAgent command definition
 * */

#include "logger_ext.hpp"
#include "tag.hpp"
#include "command/registration/register_agent.hpp"
#include "configuration/configuration.hpp"
#include "core/agent/jsonrpc_agent.hpp"
#include "core/agent/agent_manager.hpp"
#include "uuid++.hh"

using configuration::Configuration;

using namespace psme;
using namespace psme::command;
using namespace psme::app;

namespace {
const char DEFAULT_EVENTING_ADDRESS[] = "localhost";
const int DEFAULT_EVENTING_PORT = 5567;
const char GAMI_API_VERSION[] = "1.0.0";
}

/*! Register agent implementation */
class RegisterAgent : public registration::RegisterAgent {
public:
    RegisterAgent() :
        registration::RegisterAgent(registration::definition::TAG) { }

    using registration::RegisterAgent::execute;

    void execute(const Request& request, Response& response) {
        if (!request.is_valid()) {
            throw command::exception::InvalidArgument();
        }

        auto gami_id = register_agent(request);
        set_response(gami_id, response);
    }

    ~RegisterAgent();
private:
    std::string generate_id() {
        uuid client_id;
        client_id.make(UUID_MAKE_V1);
        return std::string(client_id.string());
    }

    void set_response(const std::string& gami_id, Response& response) {
        std::string eventing_address = DEFAULT_EVENTING_ADDRESS;
        int eventing_port = DEFAULT_EVENTING_PORT;

        const json::Value& configuration =
                                    Configuration::get_instance().to_json();
        try {
            eventing_address = configuration["eventing"]["address"].as_char();
            eventing_port = configuration["eventing"]["port"].as_int();
        } catch (const json::Value::Exception& e) {
            log_error(LOGUSR, "Cannot read setting " << e.what());
        }

        response.set_ipv4address(eventing_address);
        response.set_version(GAMI_API_VERSION);
        response.set_port(eventing_port);
        response.set_gami_id(gami_id);
    }

private:
    std::string register_agent(const Request& request) {
        auto& agent_manager = psme::core::agent::AgentManager::get_instance();

        auto agents = agent_manager.get_agents();
        bool conflict = false;

        for (const auto& agent: agents) {
            // check if ip:port is not already taken by other agent
            if ( agent->get_ipv4address() == request.get_ipv4address()
                 && agent->get_port() == request.get_port() ) {
                conflict = true;
            }
            // is it reconnect ?
            if ( conflict && agent->get_version() == GAMI_API_VERSION
                 && agent->get_vendor() == request.get_vendor()
                 && agent->get_capabilities() == request.get_capabilities() ) {
                return agent->get_gami_id();
            }
        }

        if (conflict) {
            throw jsonrpc::JsonRpcException(-32602,
                    "Other agent is already connected on "
                    + request.get_ipv4address()
                    + ":" + std::to_string(request.get_port()));
        }

        std::string gami_id = generate_id();

        auto agent = std::make_shared<psme::core::agent::JsonRpcAgent>(
                gami_id,
                request.get_ipv4address(),
                request.get_port());

        agent->m_version = request.get_version();
        agent->m_vendor = request.get_vendor();
        agent->m_capabilities = request.get_capabilities();

        agent_manager.add_agent(agent);
        return agent->get_gami_id();
    }
};

RegisterAgent::~RegisterAgent() { }

static Command::Register<RegisterAgent> g;
