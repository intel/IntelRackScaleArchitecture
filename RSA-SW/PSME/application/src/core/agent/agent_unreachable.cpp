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
*/

#include "agent_unreachable.hpp"

using namespace psme::core::agent;


AgentUnreachable::AgentUnreachable(const std::string& gami_id,
                                   std::uint32_t unreachable_count,
                                   std::uint32_t unreachable_seconds)
    : m_gami_id(gami_id), m_unreachable_count{unreachable_count},
      m_unreachable_seconds{unreachable_seconds} { }

AgentUnreachable::~AgentUnreachable() noexcept {}

const char* AgentUnreachable::what() const noexcept {
    return "Could not connect to agent ";
}
