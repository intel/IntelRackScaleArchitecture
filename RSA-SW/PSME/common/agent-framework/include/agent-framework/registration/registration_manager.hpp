/*!
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
 *
 * @file registration_manager.hpp
 * @brief Registration Manager
 * */

#ifndef AGENT_FRAMEWORK_REGISTRATION_REGISTRATION_MANAGER_HPP
#define AGENT_FRAMEWORK_REGISTRATION_REGISTRATION_MANAGER_HPP

#include "agent-framework/registration/registration_client.hpp"
#include "agent-framework/registration/registration_data.hpp"
#include "agent-framework/logger_ext.hpp"

#include <jsonrpccpp/client/connectors/httpclient.h>

#include <thread>
#include <chrono>
#include <atomic>

/*! AGENT_FRAMEWORK namespace */
namespace agent_framework {
/*! Generic namespace */
namespace generic {

/*! Registration class */
class RegistrationManager {
public:
    using Port = std::uint16_t;
    using Seconds = std::chrono::seconds;

    enum class State {
        UNDEFINED,
        MONITOR,
        REGISTER
    };

    /*! Default registration interval */
    static constexpr const Seconds DEFAULT_REGISTRATION_INTERVAL{3};

    /*! Registration server IP */
    static constexpr const char DEFAULT_REGISTRATION_IPV4[]{"localhost"};

    /*! Registration server port */
    static constexpr const Port DEFAULT_REGISTRATION_PORT{8383};

    /*! Defualt constructor. */
    RegistrationManager() { }

    /*! Registers Agent to AGENT_FRAMEWORK REST Server. */
    void register_agent(RegistrationData& reg_data);

    /*! Unregisters AGENT_FRAMEWORK Agent */
    void unregister_agent();

    virtual ~RegistrationManager();

private:
    volatile std::atomic<bool> m_stop{false};
    volatile std::atomic<bool> m_is_running{false};
    std::thread m_thread{};

    std::string m_url{};
    Port m_port{DEFAULT_REGISTRATION_PORT};
    std::string m_ipv4{DEFAULT_REGISTRATION_IPV4};
    Seconds m_interval{DEFAULT_REGISTRATION_INTERVAL};
    Seconds m_heart_beat{0};
    State m_state{State::UNDEFINED};

    void task();
};

}
}

#endif /* AGENT_FRAMEWORK_REGISTRATION_REGISTARTION_MANAGER_HPP */
