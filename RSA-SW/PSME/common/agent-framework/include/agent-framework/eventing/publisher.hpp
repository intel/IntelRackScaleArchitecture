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
 * @file publisher.hpp
 *
 * @brief Abstract Publisher interface
 * */

#ifndef AGENT_FRAMEWORK_EVENTING_PUBLISHER_HPP
#define AGENT_FRAMEWORK_EVENTING_PUBLISHER_HPP

// Standard library.
#include <functional>

// Project files.
#include "subscriber.hpp"

namespace agent_framework {
namespace generic {

class EventMsg;

/*!
 * @brief Abstract class. Used to implement Publish-Subscribe pattern. To create
 * new Publisher for own topics just inherits from this class.
 * One Publisher can handle one Topic. Each deriviated class handles different
 * topic.
 */
class Publisher {
public:

    /*!
     * @brief Trivial class destructor
     */
    virtual ~Publisher();

    /*!
     * @brief Subscribes an object to Publisher. After that Publisher has
     * a reference to Subscriber object to call notify.
     *
     * References passed as arguments should be valid. nullptr will be ignored.
     *
     * @param[in] subscriber pointer to Subscriber object you want to subscribe.
     */
    virtual void subscribe(Subscriber * subscriber) = 0;

    /*!
     * @brief Unsubscribe an object from Publisher. After that Publisher has no
     * reference to Subscriber object. Notify won't be called.
     *
     * References passed as arguments should be valid. nullptr will be ignored.
     *
     * @param[in] subscriber pointer to Subscriber object you want to
     * unsubscribe.
     */
    virtual void unsubscribe(Subscriber * subscriber) = 0;

    /*!
     * @brief Triggers a call to notify method on all subscribed objects.
     * This method be called to inform subscribers about some event.
     */
    virtual void notify_all(const EventMsg& msg) = 0;

    /*!
     * @brief Checks if Subscriber object which is passed as a parameter is
     * already subscribed to this Publisher.
     *
     * @param[in] subscriber pointer to Subscriber object you want to test.
     *
     * @return true if passed Subscriber is already subscribed, otherwise false.
     */
    virtual bool is_subscribed(Subscriber * subscriber) = 0;
};

} // namespace generic
} // namespace agent_framework

#endif	/* AGENT_FRAMEWORK_EVENTING_PUBLISHER_HPP */
