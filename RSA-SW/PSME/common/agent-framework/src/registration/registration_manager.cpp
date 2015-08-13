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

#include "agent-framework/registration/registration_manager.hpp"

#include "configuration/configuration.hpp"

using configuration::Configuration;

using namespace std;
using namespace agent_framework::generic;

/*! Default registration interval */
constexpr const RegistrationManager::Seconds
RegistrationManager::DEFAULT_REGISTRATION_INTERVAL;

/*! Registration server IP */
constexpr const char
RegistrationManager::DEFAULT_REGISTRATION_IPV4[];

RegistrationManager::~RegistrationManager() {
    if (m_is_running) {
        m_stop = true;
        if (m_thread.joinable()) { m_thread.join(); }
    }
}

void RegistrationManager::register_agent(RegistrationData& reg_data) {
    log_info(GET_LOGGER("registration"), "Trying to register Agent...");

    Json::Value result;
    const auto& configuration = Configuration::get_instance().to_json();

    if (configuration["registration"]["ipv4"].is_string()) {
        m_ipv4 = configuration["registration"]["ipv4"].as_string();
        log_debug(GET_LOGGER("registration"),
                "Registration IPv4 loaded from configuration");
    }
    log_debug(GET_LOGGER("registration"), "Registration IPv4: " << m_ipv4);

    if (configuration["registration"]["port"].is_uint()) {
        m_port = Port(configuration["registration"]["port"].as_uint());
        log_debug(GET_LOGGER("registration"),
                "Registration port loaded from configuration");
    }
    log_debug(GET_LOGGER("registration"), "Registration port: " << m_port);

    if (configuration["registration"]["interval"].is_uint()) {
        m_interval = Seconds(configuration["registration"]["interval"].as_uint());
        log_debug(GET_LOGGER("registration"),
                "Registration interval loaded from configuration");
    }
    log_debug(GET_LOGGER("registration"),
            "Registration interval: " << m_interval.count());

    /* Build url */
    m_url = "http://" + m_ipv4 + ":" + std::to_string(unsigned(m_port));

    jsonrpc::HttpClient httpclient(m_url);
    RegistrationClient client(httpclient);

    for (;;) {
        try {
            result = client.attach();
            reg_data.set_gami_id(result["gamiId"].asString());
            reg_data.set_app_rpc_server_ip(result["ipv4address"].asString());
            reg_data.set_app_rpc_server_port(result["port"].asInt());
            log_info(GET_LOGGER("registration"), "Agent has been registered.");

            result = client.heart_beat();
            m_heart_beat = Seconds(result["timestamp"].asUInt());
            if (m_interval < Seconds(result["minDelay"].asUInt())) {
                m_interval = Seconds(result["minDelay"].asUInt());
            }
            break;
        }
        catch (jsonrpc::JsonRpcException & e) {
            log_error(GET_LOGGER("registration"),
                    "Cannot register to server " << m_url << "...");
            log_debug(GET_LOGGER("registration"), e.what());
        }
        this_thread::sleep_for(m_interval);
    }

    m_state = State::MONITOR;

    if (!m_is_running) {
        m_stop = false;
        m_thread = std::thread{&RegistrationManager::task, this};
        m_is_running = true;
    }
}

void RegistrationManager::unregister_agent() { }

void RegistrationManager::task() {
    Json::Value result;
    Seconds heart_beat;

    // Create client instance
    jsonrpc::HttpClient httpclient(m_url);
    RegistrationClient client(httpclient);

    do {
        this_thread::sleep_for(m_interval);
        try {
            switch (m_state) {
            case State::MONITOR:
                result = client.heart_beat();
                heart_beat = Seconds(result["timestamp"].asUInt());
                if (m_interval < Seconds(result["minDelay"].asUInt())) {
                    m_interval = Seconds(result["minDelay"].asUInt());
                }
                if (heart_beat < m_heart_beat) {
                    m_state = State::REGISTER;
                }
                break;
            case State::REGISTER:
            case State::UNDEFINED:
            default:
                result = client.attach();
                log_info(GET_LOGGER("registration"),
                        "Agent has been registered.");
                result = client.heart_beat();
                m_heart_beat = Seconds(result["timestamp"].asUInt());
                if (m_interval < Seconds(result["minDelay"].asUInt())) {
                    m_interval = Seconds(result["minDelay"].asUInt());
                }
                m_state = State::MONITOR;
                break;
            }
        }
        catch (jsonrpc::JsonRpcException & e) {
            m_state = State::UNDEFINED;
            log_error(GET_LOGGER("registration"),
                    "Cannot register to server " << m_url << "...");
            log_debug(GET_LOGGER("registration"), e.what());
        }
    } while (!m_stop);
}
