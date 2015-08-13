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

#include "jsonrpc_invoker.hpp"
#include "agent_unreachable.hpp"
#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "logger/logger_factory.hpp"

#include <thread>
#include <chrono>

using namespace psme::core::agent;

int JsonRpcInvoker::g_request_id{0};

int JsonRpcInvoker::get_request_id() {
    return g_request_id++;
}

JsonRpcInvoker::JsonRpcInvoker(const std::string& gami_id, const std::string& ipv4address, const int port) :
    Invoker{},
    m_gami_id(gami_id),
    m_http_client{make_connection_url(ipv4address, port)},
    m_client{m_http_client} {
}

JsonRpcInvoker:: ~JsonRpcInvoker() {}

void JsonRpcInvoker::execute(const std::string& command,
                            psme::core::dto::RequestDTO& request,
                            psme::core::dto::ResponseDTO& response) {

    request.set_id(JsonRpcInvoker::get_request_id());
    Json::Value json_request = request.to_json();
    Json::Value json_response;
    try {
        std::lock_guard<std::mutex> lock{m_mutex};

        json_response = m_client.CallMethod(command, json_request);

        update_connection_status(0);

        Json::FastWriter writer;
        log_debug(GET_LOGGER("core"),
                            "JsonRpcInvoker call method '" << command <<  "'"
                            << " json_request: " << writer.write(json_request)
                            << " json_response: " << writer.write(json_response));
    } catch (const jsonrpc::JsonRpcException& e) {
        log_error(GET_LOGGER("core"),
                            "JsonRpcInvoker call method '" << command <<  "'"
                            << " json_request: " << json_request.toStyledString()
                            << " json_response: " << json_response.toStyledString()
                            << " error: (" << e.GetCode() << ")" << e.GetMessage());
        update_connection_status(e.GetCode());
        psme::core::dto::Error error(e.GetCode(), e.GetMessage(), e.GetData());
        response.set_error(std::move(error));
    }
    response.to_object(json_response);
}

std::string
JsonRpcInvoker::make_connection_url(const std::string& ipv4address,
                                                        const int port) const {
    return "http://" + ipv4address + ":" + std::to_string(port);
}

namespace {
    std::uint32_t time_now() {
        using namespace std::chrono;
        auto time_clock = steady_clock::now().time_since_epoch();
        return std::uint32_t(duration_cast<seconds>(time_clock).count());
    }
}

void JsonRpcInvoker::update_connection_status(const int error_code) {
    if (psme::core::dto::Error::CONNECTION_ERROR == error_code) {
        m_unreachable_count++;
        if (m_unreachable_count == 1) {
            m_time_begin = time_now();
        } else {
            m_unreachable_seconds = time_now() - m_time_begin;
        }
        throw AgentUnreachable(m_gami_id, m_unreachable_count, m_unreachable_seconds);
    }
    m_unreachable_count = 0;
}
