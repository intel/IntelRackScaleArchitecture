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

#include "agent-framework/eventing/event_client.hpp"
#include "agent-framework/client/generic_client.hpp"

using namespace agent_framework::generic;

namespace {
const char* GAMI_ID = "gami-id";
const std::size_t QUEUE_WAIT_TIME = 1000;
}

EventClient::~EventClient() {
    stop();
}

void EventClient::m_task() {
    std::string url{};

    log_info(GET_LOGGER("eventing"), "Starting RPC Client thread...");

    url = "http://" + m_registration_data.get_app_rpc_server_ip() + ":" +
    std::to_string(m_registration_data.get_app_rpc_server_port());

    log_debug(GET_LOGGER("eventing"), "Event Client connect to: " << url);

    jsonrpc::HttpClient httpclient(url);
    httpclient.AddHeader(GAMI_ID, m_registration_data.get_gami_id());
    GenericClient client(httpclient);

    log_debug(GET_LOGGER("eventing"), "RPC Client has been initialized.");

    while (m_running) {
        if (const auto msg = m_msg_queue.wait_for_and_pop(
                                            std::chrono::milliseconds(
                                                QUEUE_WAIT_TIME))) {

            auto msg_json = msg->to_json();

             try {
                 client.CallNotification("updateComponentState",
                                                            msg->to_json());
             } catch (const jsonrpc::JsonRpcException& e) {
                 log_debug(GET_LOGGER("eventing"), "Event exception " << e.what());
             }
        }
    }

    log_debug(GET_LOGGER("eventing"), "RPC Client thread is stopped.");
}

void EventClient::notify(const EventMsg& msg) {
    m_msg_queue.push_back(msg);
}
