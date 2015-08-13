/*!
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
 *
 * @file http_status_code.hpp
 *
 * @brief Declaration of HTTP status codes
 * */
#ifndef PSME_REST_HTTP_HTTPSTATUSCODES_H
#define PSME_REST_HTTP_HTTPSTATUSCODES_H

namespace psme {
namespace rest {
namespace http {

/*!
 * @enum HttpStatusCode
 * @brief HTTP status codes as defined in RFC2616-sec10
 * and additional status codes as defined in RFC6585.
 *
 * @var OK
 * Standard response for successful HTTP requests.
 *
 * @var CREATED
 * The request has been fulfilled and resulted in a new resource being created.
 *
 * @var ACCEPTED
 * The request has been accepted for processing,
 * but the processing has not been completed.
 *
 * @var NO_CONTENT
 * The server successfully processed the request,
 * but is not returning any content.
 *
 * @var RESET_CONTENT
 * The server has fulfilled the request and the user agent SHOULD reset the
 * document view which caused the request to be sent.
 *
 * @var PARTIAL_CONTENT
 * The server has fulfilled the partial GET request for the resource.
 *
 * @var MOVED_PERMANENTLY
 * The requested resource has been assigned a new permanent URI and any future
 * references to this resource SHOULD use one of the returned URIs.
 *
 * @var FOUND
 * The requested resource resides temporarily under a different URI.
 *
 * @var SEE_OTHER
 * The response to the request can be found under a different URI and SHOULD be
 * retrieved using a GET method on that resource.
 *
 * @var NOT_MODIFIED
 * Indicates that the resource has not been modified.
 *
 * @var USE_PROXY
 * The requested resource is only available through a proxy, whose address is
 * provided in the response.
 *
 * @var TEMPORARY_REDIRECT
 * In this case, the request should be repeated with another URI, however,
 * future requests should still use the original URI.
 *
 * @var BAD_REQUEST
 * The server cannot or will not process the request due to something that is
 * perceived to be a client error.
 *
 * @var UNAUTHORIZED
 * The request requires user authentication.
 *
 * @var PAYMENT_REQUIRED
 * Reserved for future use.
 *
 * @var FORBIDDEN
 * The server understood the request, but is refusing to fulfill it.
 *
 * @var NOT_FOUND
 * The requested resource could not be found but may be available
 * again in the future.
 *
 * @var METHOD_NOT_ALLOWED
 * The method specified in the Request-Line is not allowed for the resource
 * identified by the Request-URI.
 *
 * @var NOT_ACCEPTABLE
 * The requested resource is only capable of generating content not acceptable
 * according to the Accept headers sent in the request.
 *
 * @var PROXY_AUTHENTICATION_REQUIRED
 * The client must first authenticate itself with the proxy.
 *
 * @var REQUEST_TIMEOUT
 * The client did not produce a request within the time that
 * the server was prepared to wait.
 *
 * @var CONFLICT
 * The request could not be completed due to a conflict with the current
 * state of the resource.
 *
 * @var GONE
 * The requested resource is no longer available at the server and
 * no forwarding address is known.
 *
 * @var LENGTH_REQUIRED
 * The server refuses to accept the request without a defined Content-Length.
 *
 * @var PRECONDITION_FAILED
 * The precondition given in one or more of the request-header fields
 * evaluated to false when it was tested on the server.
 *
 * @var REQUEST_ENTITY_TOO_LARGE
 * The server is refusing to process a request because the request entity is
 * larger than the server is willing or able to process.
 *
 * @var REQUEST_URI_TOO_LONG
 * The server is refusing to service the request because the Request-URI is
 * longer than the server is willing to interpret.
 *
 * @var UNSUPPORTED_MEDIA_TYPE
 * The server is refusing to service the request because the entity of
 * the request is in a format not supported by the requested resource for
 * the requested method.
 *
 * @var REQUESTED_RANGE_NOT_SATISFIABLE
 * The client has asked for a portion of the file (byte serving), but
 * the server cannot supply that portion.
 *
 * @var EXPECTATION_FAILED
 * The server cannot meet the requirements of the Expect request-header field.
 *
 * @var PRECONDITION_REQUIRED
 * The origin server requires the request to be conditional.
 *
 * @var INTERNAL_SERVER_ERROR
 * The server encountered an unexpected condition which prevented it
 * from fulfilling the request.
 *
 * @var NOT_IMPLEMENTED
 * The server does not support the functionality required to fulfill
 * the request.
 *
 * @var BAD_GATEWAY
 * The server, while acting as a gateway or proxy, received an invalid response
 * from the upstream server it accessed in attempting to fulfill the request.
 *
 * @var SERVICE_UNAVAILABLE
 * The server is currently unable to handle the request due to a temporary
 * overloading or maintenance of the server.
 *
 * @var GATEWAY_TIMEOUT
 * The server was acting as a gateway or proxy and did not receive a timely
 * response from the upstream server.
 *
 * @var HTTP_VERSION_NOT_SUPPORTED
 * The server does not support, or refuses to support, the HTTP protocol
 * version that was used in the request message.
 *
 * */
enum HttpStatusCode {
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    TEMPORARY_REDIRECT = 307,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    REQUEST_ENTITY_TOO_LARGE = 413,
    REQUEST_URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    PRECONDITION_REQUIRED = 428,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

}
}
}

#endif	/* PSME_REST_HTTP_HTTPSTATUSCODES_H */
