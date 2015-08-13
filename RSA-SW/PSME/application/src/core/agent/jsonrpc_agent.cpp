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

#include "jsonrpc_agent.hpp"

using namespace psme::core::agent;

JsonRpcAgent::JsonRpcAgent( const std::string& gami_id,
                            const std::string& ipv4address,
                            int port)
    : Agent{gami_id, ipv4address, port}, m_invoker{gami_id, ipv4address, port} { }

JsonRpcAgent::~JsonRpcAgent() {}

Invoker& JsonRpcAgent::get_invoker() {
    return m_invoker;
}
