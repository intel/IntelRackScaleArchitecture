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

#include "eventing_http_server.hpp"
#include "json/json.hpp"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <jsonrpccpp/common/specificationparser.h>
#include <iostream>

using namespace psme::app::eventing;
using namespace std;

namespace {

static constexpr unsigned short DEFAULT_EVENTING_PORT = 5567;

const char* GAMI_ID = "gami-id";

const char* get_gami_id(struct MHD_Connection* connection) {
    return MHD_lookup_connection_value(connection,
            MHD_HEADER_KIND,
            GAMI_ID);
}

void
add_gami_id_to_event_msg(const char* gami_id, std::stringstream& sstream) {
    Json::Value request{};
    sstream >> request;
    if (request.isMember("params")) {
        request["params"]["gamiId"] = !gami_id ? "" : gami_id;
        Json::FastWriter writer;
        sstream.str(writer.write(request));
    }
}
}

struct mhd_coninfo {
        struct MHD_PostProcessor *postprocessor{};
        MHD_Connection* connection{};
        stringstream request{};
        EventingHttpServer* server{};
        int code{};
};

EventingHttpServer::EventingHttpServer(const json::Value& config) :
    AbstractServerConnector(),
    m_config(config),
    m_port(DEFAULT_EVENTING_PORT),
    m_threads(10),
    m_running(false),
    m_path_sslcert(),
    m_path_sslkey(),
    m_sslcert(),
    m_sslkey(),
    m_daemon(nullptr)
{
    m_port = static_cast<unsigned short>(m_config["eventing"]["port"].as_uint());
}

IClientConnectionHandler*
EventingHttpServer::GetHandler(const std::string &url) {
    if (AbstractServerConnector::GetHandler() != nullptr)
        return AbstractServerConnector::GetHandler();
    auto it = this->urlhandler.find(url);
    if (it != this->urlhandler.end())
        return it->second;
    return nullptr;
}

bool EventingHttpServer::StartListening() {
    if (!this->m_running) {
        if (this->m_path_sslcert != "" && this->m_path_sslkey != "") {
            try {
                SpecificationParser::GetFileContent(this->m_path_sslcert,
                        this->m_sslcert);
                SpecificationParser::GetFileContent(this->m_path_sslkey,
                        this->m_sslkey);

                this->m_daemon = MHD_start_daemon(
                        MHD_USE_SSL | MHD_USE_SELECT_INTERNALLY,
                        static_cast<uint16_t> (this->m_port),
                        nullptr,
                        nullptr,
                        EventingHttpServer::callback,
                        this,
                        MHD_OPTION_HTTPS_MEM_KEY,
                        this->m_sslkey.c_str(),
                        MHD_OPTION_HTTPS_MEM_CERT,
                        this->m_sslcert.c_str(),
                        MHD_OPTION_THREAD_POOL_SIZE,
                        this->m_threads,
                        MHD_OPTION_END);
            } catch (JsonRpcException& ex) {
                (void) ex;
                return false;
            }
        } else {
            this->m_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                    static_cast<uint16_t> (this->m_port),
                    nullptr,
                    nullptr,
                    EventingHttpServer::callback,
                    this,
                    MHD_OPTION_THREAD_POOL_SIZE,
                    this->m_threads,
                    MHD_OPTION_END);
        }
        if (this->m_daemon != nullptr)
            this->m_running = true;

    }
    return this->m_running;
}

bool EventingHttpServer::StopListening() {
    if (this->m_running) {
        MHD_stop_daemon(this->m_daemon);
        this->m_running = false;
    }
    return true;
}

bool EventingHttpServer::SendResponse(const string& response, void* addInfo) {
    struct mhd_coninfo* client_connection =
        static_cast<struct mhd_coninfo*>(addInfo);
    struct MHD_Response *result = MHD_create_response_from_data(response.size(),
            const_cast<char*>(response.c_str()), 0, 1);

    MHD_add_response_header(result, "Content-Type", "application/json");
    MHD_add_response_header(result, "Access-Control-Allow-Origin", "*");

    int ret = MHD_queue_response(client_connection->connection,
            unsigned(client_connection->code),
            result);
    MHD_destroy_response(result);
    return ret == MHD_YES;
}

bool EventingHttpServer::SendOptionsResponse(void* addInfo) {
    struct mhd_coninfo* client_connection =
        static_cast<struct mhd_coninfo*>(addInfo);
    struct MHD_Response *result = MHD_create_response_from_data(0,
                                                                nullptr,
                                                                0,
                                                                1);

    MHD_add_response_header(result, "Allow", "POST, OPTIONS");
    MHD_add_response_header(result, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(result, "Access-Control-Allow-Headers",
                                            "origin, content-type, accept");
    MHD_add_response_header(result, "DAV", "1");

    int ret = MHD_queue_response(client_connection->connection,
            unsigned(client_connection->code),
            result);
    MHD_destroy_response(result);
    return ret == MHD_YES;
}

void EventingHttpServer::SetUrlHandler(const string &url,
                                        IClientConnectionHandler *handler_) {
    this->urlhandler[url] = handler_;
    this->SetHandler(nullptr);
}

int EventingHttpServer::callback(void *cls, MHD_Connection *connection,
        const char *url, const char *method, const char *version,
        const char *upload_data, size_t *upload_data_size, void **con_cls) {
    (void)version;

    if (check_client_connection(cls, con_cls, connection)) {
        return MHD_YES;
    }

    struct mhd_coninfo* client_connection =
        static_cast<struct mhd_coninfo*>(*con_cls);

    if (string("POST") == method) {
        if (check_post_data(upload_data,
                            upload_data_size,
                            client_connection)) {
            return MHD_YES;
        }
        handle_post_action(url, connection, client_connection);
    } else if (string("OPTIONS") == method) {
        client_connection->code = MHD_HTTP_OK;
        client_connection->server->SendOptionsResponse(client_connection);
	} else {
        client_connection->code = MHD_HTTP_METHOD_NOT_ALLOWED;
        client_connection->server->SendResponse("Not allowed HTTP Method",
                                                client_connection);
    }
    delete client_connection;

    return MHD_YES;
}


void EventingHttpServer::handle_post_action(const char* url,
                                    MHD_Connection* connection,
                                    struct mhd_coninfo* client_connection) {
    string response;
    IClientConnectionHandler* handler =
    client_connection->server->GetHandler(string(url));

    if (handler == nullptr) {
        client_connection->code = MHD_HTTP_INTERNAL_SERVER_ERROR;
        client_connection->server->SendResponse(
            "No client conneciton handler found", client_connection);
    } else {
        client_connection->code = MHD_HTTP_OK;
        add_gami_id_to_event_msg(get_gami_id(connection),
                                            client_connection->request);
        handler->HandleRequest(client_connection->request.str(), response);
        client_connection->server->SendResponse(response, client_connection);
    }
}

bool EventingHttpServer::check_client_connection(void* cls,
                                                void** con_cls,
                                                MHD_Connection* connection) {
    if (*con_cls == nullptr) {
        struct mhd_coninfo* client_connection = new mhd_coninfo;
        client_connection->connection = connection;
        client_connection->server = static_cast<EventingHttpServer*>(cls);
        *con_cls = client_connection;
        return true;
    }
    return false;
}

bool EventingHttpServer::check_post_data(const char* upload_data,
                                        std::size_t* upload_data_size,
                                    struct mhd_coninfo* client_connection) {

    if (*upload_data_size != 0) {
        client_connection->request.write(upload_data, long(*upload_data_size));
        *upload_data_size = 0;
        return true;
    }
    return false;
}
