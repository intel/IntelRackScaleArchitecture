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

#include "psme/rest/http/server.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "logger/logger_factory.hpp"
#include "psme/rest/error/server_error.hpp"
#include "psme/rest/error/server_exception.hpp"
#include "psme/rest/error/error_factory.hpp"

using namespace psme::rest::http;
using psme::rest::error::ServerError;
using psme::rest::error::ServerException;
using psme::rest::error::ErrorFactory;

namespace {

void default_method(const Request&, Response& response) {
    auto error = ErrorFactory::create_method_not_allowed_error();
    response.set_reply(error.get_http_status_code(), error.as_string());
}

}

Server::Server(const string& url) :
    m_url(url),
    m_method_callback{{
            default_method,
            default_method,
            default_method,
            default_method,
            default_method,
            default_method}} { }

Server::~Server() { }

void Server::call(const Method method, const Request& request,
        Response& response) {
    try {
        m_method_callback.at(method)(request, response);
    } catch (const std::out_of_range&) {
        default_method(request, response);
    } catch (const ServerException& e) {
        const ServerError& error = e.get_error();
        response.set_reply(error.get_http_status_code(), error.as_string());
    } catch (...) {
        ServerError internal_server_error;
        response.set_reply(internal_server_error.get_http_status_code(),
                internal_server_error.as_string());
    }
    log_debug(GET_LOGGER("rest"), "\nRequest: " <<
    "\n\tURL: " << request.get_url() <<
    "\n\tMSG: " << request.get_body());
    log_debug(GET_LOGGER("rest"), "\nResponse: " <<
    "\n\tSTATUS: " << response.get_status() <<
    "\n\tMSG: " << response.get_body());
}

void Server::support(MethodCallback method_callback) {
    if (nullptr == method_callback) {
        method_callback = default_method;
    }

    for (auto& method : m_method_callback) {
        method = method_callback;
    }
}

void Server::support(const Method method,
        MethodCallback method_callback) {
    if (nullptr == method_callback) {
        method_callback = default_method;
    }

    if (method < MAX_METHODS) {
        m_method_callback[method] = method_callback;
    }
}

Request::Request(const string& url, const string& message) :
    m_url(url),
    m_body(message),
    m_headers() {}

void Request::add_header(const std::string& key, const std::string& value) {
    m_headers.add_header(key, value);
}

Response::Response() : m_status(HttpStatusCode::NOT_FOUND),
        m_body(), m_mime(JSON_MIME), m_headers() {}

void Response::set_reply(const std::uint32_t status,
        const string& message, const char* mime) {
    m_status = status;
    m_body = message;
    m_mime = mime;
}

void Response::send_redirect(const string& location) {
    m_status = HttpStatusCode::TEMPORARY_REDIRECT;
    add_header(HttpHeaders::LOCATION, location);
}

void Response::add_header(const std::string& key, const std::string& value) {
    m_headers.add_header(key, value);
}
