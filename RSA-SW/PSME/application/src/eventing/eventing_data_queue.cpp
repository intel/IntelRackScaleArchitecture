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

#include "eventing_data_queue.hpp"

using namespace psme::app::eventing;

namespace {

EventingDataQueue* g_eventing_data_mediator = nullptr;

}

EventingDataQueue* EventingDataQueue::get_instance() {
    if (!g_eventing_data_mediator) {
        g_eventing_data_mediator = new EventingDataQueue;
    }
    return g_eventing_data_mediator;
}

void EventingDataQueue::cleanup() {
    if (g_eventing_data_mediator) {
        delete g_eventing_data_mediator;
    }
}
