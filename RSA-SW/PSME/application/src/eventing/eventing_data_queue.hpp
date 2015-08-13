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

#ifndef PSME_FW_BUILD_EVENTING_DATA_QUEUE_HPP
#define PSME_FW_BUILD_EVENTING_DATA_QUEUE_HPP

#include "command/eventing/eventing_agent.hpp"
#include "event_queue.hpp"

/*! PSME namespace */
namespace psme {
/*! Application namespace */
namespace app {
/*! Eventing namespace */
namespace eventing {

using namespace psme::command::eventing;

/*! Store eventing messages to processing */
class EventingDataQueue :
                        public generic::EventQueue<EventingAgent::Request> {

public:
    /*!
     * @brief Singleton pattern. Gets eveting data instance that contains event
     * messages
     *
     * @return Global EventingDataQueue object
     */
    static EventingDataQueue* get_instance();


    /*!
     * @brief Cleanup EventingDataQueue object
     */
    static void cleanup();

private:
    explicit EventingDataQueue() = default;
    EventingDataQueue(const EventingDataQueue&) = delete;
    EventingDataQueue& operator=(const EventingDataQueue&) = delete;
    EventingDataQueue(EventingDataQueue&&) = delete;
    EventingDataQueue& operator=(EventingDataQueue&&) = delete;
};

}
}
}

#endif //PSME_FW_BUILD_EVENTING_DATA_QUEUE_HPP
