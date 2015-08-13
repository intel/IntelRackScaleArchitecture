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
#include "psme/rest/http/psme_server_factory.hpp"
#include "psme/rest/http/server.hpp"
#include "psme/rest/node/tree_manager.hpp"
#include "psme/rest/http/microhttpd.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "json/json.hpp"
#include "psme/rest/error/server_exception.hpp"

using namespace psme::rest::http;
using namespace psme::rest::node;
using namespace psme::rest::error;

static PsmeServerFactory* g_psme_server_factory = nullptr;

PsmeServerFactory& PsmeServerFactory::get_instance() {
    if (nullptr == g_psme_server_factory) {
        g_psme_server_factory = new PsmeServerFactory;
    }
    return *g_psme_server_factory;
}

Server* PsmeServerFactory::create_server(const std::string& url,
        TreeManager& tree_manager) {

    Server* server = new MicroHttpd(url);

    server->support(Server::Method::GET,
            [&tree_manager](const Request& request, Response & response) {
                tree_manager.get(request, response);
            });
    server->support(Server::Method::DELETE,
            [&tree_manager](const Request& request, Response & response) {
                tree_manager.del(request, response);
            });
    server->support(Server::Method::POST,
            [&tree_manager](const Request& request, Response & response) {
                tree_manager.post(request, response);
            });
    server->support(Server::Method::PUT,
            [&tree_manager](const Request& request, Response & response) {
                tree_manager.put(request, response);
            });
    server->support(Server::Method::PATCH,
            [&tree_manager](const Request& request, Response & response) {
                tree_manager.patch(request, response);
            });
    server->support(Server::Method::HEAD,
            [&tree_manager](const Request& request, Response & response) {
                tree_manager.head(request, response);
            });

    return server;
}
