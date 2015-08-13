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
 * @file event_publisher.hpp
 *
 * @brief Implementation of Publisher interface to handle HotSwap feature events.
 * */

#ifndef AGENT_FRAMEWORK_EVENTING_EVENT_PUBLISHER_HPP
#define AGENT_FRAMEWORK_EVENTING_EVENT_PUBLISHER_HPP

// Standard library.
#include <vector>
#include <algorithm>
#include <functional>

// Project files.
#include "publisher.hpp"
#include "subscriber.hpp"

namespace agent_framework {
namespace generic {

class EventMsg;

/*!
 * @brief Implementation of Publisher interface to handle HotSwap feature.
 * This Publisher is also Observer for StateMachine feature to get notifications
 * about States changes. After it get notification about Transition
 * (State change) it decides to inform all subscribed object.
 * */
class EventPublisher: public agent_framework::generic::Publisher {
public:
    /*!
     * @brief Trivial class constructor.
     */
    EventPublisher();

    /*!
     * @brief Trivial class destructor.
     */
    virtual ~EventPublisher();

    virtual void subscribe(Subscriber * subscriber);
    virtual void unsubscribe(Subscriber * sub);
    virtual void notify_all(const EventMsg& msg);
    virtual bool is_subscribed(Subscriber * subscriber);

    /**
     * @brief Sets publisher enable
     *
     * @param enable status
     */
    void set_enable(const bool enable) {
        m_enable = enable;
    }

    /**
     * @brief Gets publisher enable
     *
     * @return status
     */
    bool get_enable() const {
        return m_enable;
    }
private:
    std::vector<Subscriber*> m_subscribers;

    /*!
     * @brief Returns iterator with position of Subscriber passed as an argument.
     *
     * @param[in] subscriber pointer to Subscriber object whose position you want to
     * check.
     *
     * @return iterator with position of Subscriber passed as an argument.
     * If passed Subscriber isn't subscribed will return iterator set to end.
     */
    std::vector<Subscriber*>::iterator get_subscriber_position(Subscriber * subscriber);

    /**
     * @brief Is publisher enable
     */
    bool m_enable;
};

} // namespace generic
} // namespace agent_framework

#endif	/* AGENT_FRAMEWORK_EVENTING_EVENT_PUBLISHER_HPP */

