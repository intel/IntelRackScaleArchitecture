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
 * @file jsonrpc_agent.hpp
 *
 * @brief JsonRPC Agent implementation
 * */

#ifndef PSME_JSONRPC_AGENT_HPP
#define PSME_JSONRPC_AGENT_HPP

#include "agent.hpp"
#include "jsonrpc_invoker.hpp"

/*! Forwared declaration */
class RegisterAgent;

namespace psme {
namespace core {
namespace agent {

/*! JsonRPC implementation of agent */
class JsonRpcAgent final : public Agent {
public:
    /*!
     * @brief JsonRpcAgent constructon
     *
     * @param gami_id agent id from request
     * @param ipv4address agent IPv4 address from request
     * @param port agent port from request
     */
    JsonRpcAgent(const std::string& gami_id, const std::string& ipv4address,
                                                                    int port);
    ~JsonRpcAgent();

    /*!
     * @brief Return specified invoker for JsonRpcAgent
     *
     * @return Invoker object
     */
    Invoker& get_invoker() override;

private:
    friend class ::RegisterAgent;
    JsonRpcInvoker m_invoker;
};

}
}
}
#endif /* PSME_JSONRPC_AGENT_HPP */
