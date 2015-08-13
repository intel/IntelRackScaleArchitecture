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
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/utils/enum_chars.hpp"

using namespace psme::rest::http;

namespace psme {
namespace rest {
namespace utils {

template<>
void get_enum_chars_array<HttpStatusCode>(EnumChars<HttpStatusCode>*& ptr, size_t& size) {
    static EnumChars<HttpStatusCode> g_http_status_code_chars[] = {
        { HttpStatusCode::OK, "OK"},
        { HttpStatusCode::CREATED, "Created"},
        { HttpStatusCode::ACCEPTED, "Accepted"},
        { HttpStatusCode::NO_CONTENT, "No Content"},
        { HttpStatusCode::RESET_CONTENT, "Reset Content"},
        { HttpStatusCode::PARTIAL_CONTENT, "Partial Content"},
        { HttpStatusCode::MOVED_PERMANENTLY, "Moved Permanently"},
        { HttpStatusCode::FOUND, "Found"},
        { HttpStatusCode::SEE_OTHER, "See Other"},
        { HttpStatusCode::NOT_MODIFIED, "Not Modified"},
        { HttpStatusCode::USE_PROXY, "Use Proxy"},
        { HttpStatusCode::TEMPORARY_REDIRECT, "Temporary Redirect"},
        { HttpStatusCode::BAD_REQUEST, "Bad Request"},
        { HttpStatusCode::UNAUTHORIZED, "Unauthorized"},
        { HttpStatusCode::PAYMENT_REQUIRED, "Payment Required"},
        { HttpStatusCode::FORBIDDEN, "Forbidden"},
        { HttpStatusCode::NOT_FOUND, "Not Found"},
        { HttpStatusCode::METHOD_NOT_ALLOWED, "Method Not Allowed"},
        { HttpStatusCode::NOT_ACCEPTABLE, "Not Acceptable"},
        { HttpStatusCode::PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"},
        { HttpStatusCode::REQUEST_TIMEOUT, "Request Timeout"},
        { HttpStatusCode::CONFLICT, "Conflict"},
        { HttpStatusCode::GONE, "Gone"},
        { HttpStatusCode::LENGTH_REQUIRED, "Length Required"},
        { HttpStatusCode::PRECONDITION_FAILED, "Precondition Failed"},
        { HttpStatusCode::REQUEST_ENTITY_TOO_LARGE, "Request Entity Too Large"},
        { HttpStatusCode::REQUEST_URI_TOO_LONG, "Request-URI Too Long"},
        { HttpStatusCode::UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type"},
        { HttpStatusCode::REQUESTED_RANGE_NOT_SATISFIABLE, "Requested Range Not Satisfiable"},
        { HttpStatusCode::EXPECTATION_FAILED, "Expectation Failed"},
        { HttpStatusCode::PRECONDITION_REQUIRED, "Precondition Required"},
        { HttpStatusCode::INTERNAL_SERVER_ERROR, "Internal Server Error"},
        { HttpStatusCode::NOT_IMPLEMENTED, "Not Implemented"},
        { HttpStatusCode::BAD_GATEWAY, "Bad Gateway"},
        { HttpStatusCode::SERVICE_UNAVAILABLE, "Service Unavailable"},
        { HttpStatusCode::GATEWAY_TIMEOUT, "Gateway Timeout"},
        { HttpStatusCode::HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"}
    };
    ptr = g_http_status_code_chars;
    size = array_size(g_http_status_code_chars);
}

}
}
}


