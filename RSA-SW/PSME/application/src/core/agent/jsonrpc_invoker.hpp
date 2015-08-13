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
 * @file jsonrpc_invoker.hpp
 *
 * @brief JsonRPC invoker implementation
 * */

#ifndef PSME_JSONRPC_INVOKER_HPP
#define PSME_JSONRPC_INVOKER_HPP

#include "invoker.hpp"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <jsonrpccpp/client.h>
#include <mutex>
#include <memory>

namespace psme {
namespace core {
namespace agent {

/*! JsonRPC implementation of Invoker */
class JsonRpcInvoker final : public Invoker {
public:
    /*!
     * @brief Create JsonRpcInvoker object for given IPv4 address and port
     *
     * @param ipv4address agent IPv4 address
     * @param port agent port
     */
    explicit JsonRpcInvoker(const std::string& gami_id, const std::string& ipv4address, const int port);
    ~JsonRpcInvoker();

    /*! Implement invoker execute method for JsonRPC */
    void execute(const std::string& command,
                psme::core::dto::RequestDTO& request,
                psme::core::dto::ResponseDTO& response) override;

    /*!
     * @brief Gets connection status
     *
     * ConnectionStatus is a pair.
     * First element contains number of failed requests send to agent,
     * second element is number of seconds agent is unavailable.
     *
     * @return ConnectionStatus
     */
    ConnectionStatus get_connection_status() const override {
        return {m_unreachable_count, m_unreachable_seconds};
    }

private:
    /*!
     * @brief Create connection URL from IPv4 address and port
     *
     * @param ipv4address agent IPv4 address
     * @param port agent port
     *
     * @return  Connection URL
     */
    std::string make_connection_url(const std::string& ipv4address,
                                                        const int port) const;
    void update_connection_status(const int error_code);
    std::mutex m_mutex{};
    std::string m_gami_id;
    std::uint32_t m_unreachable_count{0};
    std::uint32_t m_unreachable_seconds{0};
    std::uint32_t m_time_begin{0};
    jsonrpc::HttpClient m_http_client;
    jsonrpc::Client m_client;

    static int g_request_id;
    static int get_request_id();
};
}
}
}

#endif /* PSME_JSONRPC_INVOKER_HPP */
