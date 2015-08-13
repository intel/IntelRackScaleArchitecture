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
 * @file event_client.hpp
 *
 * @brief Eventing Client
 * */

#ifndef AGENT_FRAMEWORK_EVENTING_EVENT_CLIENT_HPP
#define AGENT_FRAMEWORK_EVENTING_EVENT_CLIENT_HPP

#include "subscriber.hpp"
#include "event_queue.hpp"
#include "event_msg.hpp"
#include "agent-framework/logger_ext.hpp"
#include "agent-framework/registration/registration_data.hpp"

#include <jsonrpccpp/client/connectors/httpclient.h>

#include <thread>
#include <memory>

/*! AGENT_FRAMEWORK namespace */
namespace agent_framework {
/*! Generic namespace */
namespace generic {

/*! Event client send notification to AGENT_FRAMEWORK Application */
class EventClient : public Subscriber {
public:

    /*!
     * Event Client class constructor.
     * */
    explicit EventClient(const RegistrationData& registration_data) :
        Subscriber{},
        m_registration_data{registration_data},
        m_thread{},
        m_running{false},
        m_msg_queue{} {}

    ~EventClient();

    /*! Start Event Client */
    void start() {
        if (!m_running) {
            m_running = true;
            m_thread = std::thread(&EventClient::m_task, this);
        }
    }

    /*! Stop Event Client */
    void stop() {
        if (m_running) {
            m_running = false;
            if (m_thread.joinable()) {
                m_thread.join();
            }
        }
    }

    /*!
     * @brief Method handle notification from system
     *
     * @param[in] msg Event message object
     * */
    void notify(const EventMsg& msg) override;

private:
    RegistrationData m_registration_data;
    std::thread m_thread;
    volatile bool m_running;
    EventQueue<EventMsg> m_msg_queue;

    void m_task();
};

/*! Eventing Client unique pointer. */
using EventClientUniquePtr = std::unique_ptr<EventClient>;

}
}
#endif /* AGENT_FRAMEWORK_EVENTING_EVENT_CLIENT_HPP */
