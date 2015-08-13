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

#include "logger_ext.hpp"
#include "tag.hpp"
#include "command/eventing/eventing_agent.hpp"
#include "eventing/eventing_data_queue.hpp"

using namespace psme;
using namespace psme::command;
using namespace psme::app;

/*! EventingAgent implementation */
class EventingAgent : public psme::command::eventing::EventingAgent {
public:
    EventingAgent() : psme::command::eventing::EventingAgent(
            psme::command::eventing::definition::TAG) { }

    using psme::command::eventing::EventingAgent::execute;

    void execute(const Request& request, Response& response) {
        (void) response;
        psme::app::eventing::EventingDataQueue::get_instance()->
                                                     push_back(request);
    }

    ~EventingAgent();
};

EventingAgent::~EventingAgent() { }

static Command::Register<EventingAgent> g;
