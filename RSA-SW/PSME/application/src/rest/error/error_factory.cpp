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
#include "psme/rest/error/error_factory.hpp"
#include "psme/rest/error/server_error.hpp"

#include <exception>

using namespace psme::rest::error;

ServerError ErrorFactory::create_not_found_error() {
    ServerError server_error{HttpStatusCode::NOT_FOUND};
    server_error.set_code("InvalidEndpoint");
    server_error.set_message("Invalid endpoint in /rest/v1 namespace");
    return server_error;
}

ServerError ErrorFactory::create_malformed_uri_error() {
    ServerError server_error{HttpStatusCode::BAD_REQUEST};
    server_error.set_code("MalformedUri");
    server_error.set_message("Malformed URI");
    return server_error;
}

ServerError ErrorFactory::create_method_not_allowed_error() {
    ServerError server_error{HttpStatusCode::METHOD_NOT_ALLOWED};
    server_error.set_code("MethodNotAllowed");
    server_error.set_message("Method Not Allowed");
    return server_error;
}

ServerError ErrorFactory::create_invalid_payload_error() {
    ServerError server_error{HttpStatusCode::BAD_REQUEST};
    server_error.set_code("InvalidPayload");
    server_error.set_message("Request payload is invalid or missing");
    return server_error;
}
