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
 * @file  event_publisher.cpp
 *
 * @brief Event
 * */

#include "agent-framework/eventing/event_publisher.hpp"
#include "agent-framework/eventing/event_msg.hpp"

using namespace agent_framework::generic;

EventPublisher::EventPublisher():
    m_subscribers(),
    m_enable(true) {
    // Just to init subscribers and avoid gcc warnings.
}


EventPublisher::~EventPublisher() {
    m_subscribers.clear();
}


void EventPublisher::subscribe(Subscriber * subscriber) {
    // Ignores NULL and already subscribed.
    if(nullptr == subscriber) return;
    if(is_subscribed(subscriber)) return;

    m_subscribers.push_back(subscriber);
}

void EventPublisher::unsubscribe(Subscriber* subscriber) {
    // Ignores NULL and already unsubsribed.
    if(nullptr == subscriber) return;
    if(!is_subscribed(subscriber)) return;

    auto it = get_subscriber_position(subscriber);
    m_subscribers.erase(it);
}

bool EventPublisher::is_subscribed(Subscriber * subscriber) {
    return get_subscriber_position(subscriber) != m_subscribers.end() ? true:false;
}

std::vector<Subscriber*>::iterator EventPublisher::get_subscriber_position(Subscriber * subscriber)
{
    return std::find(m_subscribers.begin(), m_subscribers.end(), subscriber);
}


void EventPublisher::notify_all(const EventMsg& msg) {
    if (!get_enable()) { return; }

    for (auto it = m_subscribers.begin(); it != m_subscribers.end(); ++it) {
        (*it)->notify(msg);
    }
}

