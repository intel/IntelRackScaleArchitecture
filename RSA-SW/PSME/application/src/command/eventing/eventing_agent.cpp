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

#include "eventing_agent.hpp"
#include <ostream>

using namespace psme::command::eventing;

const char EventingAgent::TAG[] = "updateComponentState";

EventingAgent::~EventingAgent(){}

EventingAgent::Request::~Request() { }

EventingAgent::Response::~Response() { }

namespace psme {
namespace command {
namespace eventing {
    std::ostream& operator<<(std::ostream& out, const EventingAgent::Request& r) {
        out << "[gami_id=" << r.get_gami_id()
            << " component=" << r.get_id()
            << " state=" << r.get_state()
            << " transition=" << r.get_transition() << "]";
        return out;
    }
}
}
}
