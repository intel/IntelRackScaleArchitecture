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
 * @file subscriber.hpp
 *
 * @brief Abstract Subscriber interface
 * */

#ifndef AGENT_FRAMEWORK_EVENTING_SUBSCRIBER_HPP
#define AGENT_FRAMEWORK_EVENTING_SUBSCRIBER_HPP

namespace agent_framework {
namespace generic {

class EventMsg;

/*!
 * @brief Abstract class. Used to implement Publish-Subscribe pattern. To create
 * new Subscriber with own defined behavior just inherits from this class.
 *
 * One Subscriber can handle one Topic. To handle one Topic with different
 * behavior you need one class for each behavior.
 */
class Subscriber {
public:

    /*!
     * @brief Trivial class destructor
     */
    virtual ~Subscriber();

    /*!
     * @brief This method will be called when Subscriber is subscribed to some
     * Publisher object and Publisher called notify_all because of some event.
     *
     * To implement own behavior for an event, just inherits from this class,
     * implement this method and subscribe to some interesting Publisher.
     */
    virtual void notify(const EventMsg& msg) = 0;
};

} // namesapce generic
} // namespace agent_framework

#endif	/* AGENT_FRAMEWORK_EVENTING_SUBSCRIBER_HPP */

