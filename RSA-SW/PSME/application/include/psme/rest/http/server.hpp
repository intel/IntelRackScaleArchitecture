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
 * @file server.hpp
 *
 * @brief Declaration of Server, Request and Response classes
 * */

#ifndef PSME_REST_HTTP_SERVER_HPP
#define PSME_REST_HTTP_SERVER_HPP

#include "headers.hpp"

#include <string>
#include <functional>
#include <map>
#include <array>

namespace psme {
namespace rest {
namespace http {

using std::string;

/*! @brief JSON content type used in negotiation */
static constexpr const char JSON_MIME[] = "application/json";
/*! @brief XML content type used in negotiation */
static constexpr const char XML_MIME[] = "application/xml";

/*!
 * @brief HTTP Request object.
 *
 * */
class Request {
public:
    /*!
     * @brief Constructor
     * @param[in] url Request URL.
     * @param[in] body Request body (if any).
     */
    Request(const string& url, const string& body = "");

    /*! @brief Destructor */
    ~Request() { }

    /*!
     * @brief Request URL getter.
     *
     * @return Request URL.
     */
    const string& get_url() const { return m_url; }

    /*!
     * @brief Request body getter.
     *
     * @return Request body.
     */
    const string& get_body() const { return m_body; }

    /*!
     * @brief Populates request header.
     *
     * @param[in] key Header key.
     * @param[in] value Header value.
     */
    void add_header(const std::string& key, const std::string& value);

    /*!
     * @brief Request headers getter.
     *
     * @return Request headers.
     */
    const HttpHeaders& get_headers() const { return m_headers; }

private:
    const string m_url;
    const string m_body;
    HttpHeaders m_headers;
};

/*!
 * @brief HTTP Response object.
 *
 * */
class Response {
public:
    /*! @brief Constructor */
    Response();

    /*! @brief Copy Constructor */
    Response(const Response&) = default;

    /*! @brief Assignment operator */
    Response& operator=(const Response&) = default;

    /*! @brief Destructor */
    ~Response() { }

    /*!
     * @brief Populates response object.
     *
     * @param[in] status HTTP status code.
     * @param[in] body Response message body.
     * @param[in] mime Response mime type.
     */
    void set_reply(const std::uint32_t status, const string& body = "",
        const char* mime = JSON_MIME);

    /*!
     * @brief Sends HTTP TEMPORARY_REDIRECT (307) response.
     *
     * @param[in] location Redirect location.
     */
    void send_redirect(const string& location = "");

    /*!
     * @brief Response HTTP status code getter.
     *
     * @return HTTP status code of response.
     */
    unsigned int get_status() const { return m_status; }

    /*!
     * @brief Response message body getter.
     *
     * @return Response message body.
     */
    const string& get_body() const { return m_body; }

    /*!
     * @brief Response MIME type getter.
     *
     * @return Response MIME type.
     */
    const char* get_mime() const { return m_mime; }

    /*!
     * @brief Adds response header.
     *
     * @param[in] key Header key.
     * @param[in] value Header value.
     */
    void add_header(const std::string& key, const std::string& value);

    /*!
     * @brief Response headers getter.
     *
     * @return Response headers.
     */
    const HttpHeaders& get_headers() const { return m_headers; }

private:
    unsigned int m_status;
    string m_body;
    const char* m_mime;
    HttpHeaders m_headers;
};

/*!
 * @brief HTTP server class interface.
 * */
class Server {
public:
    typedef std::function<void(const Request&, Response&)> MethodCallback;

    /*!
     * @enum Method
     * @brief Supported HTTP Method types.
     *
     * @var Method Server::POST
     * HTTP POST method.
     *
     * @var Method Server::GET
     * HTTP GET method.
     *
     * @var Method Server::PUT
     * HTTP PUT method.
     *
     * @var Method Server::PATCH
     * HTTP PATCH method.
     *
     * @var Method Server::DELETE
     * HTTP DELETE method.
     *
     * @var Method Server::HEAD
     * HTTP HEAD method.
     *
     * @var Method Server::NOT_ALLOWED
     * End guard value.
     */
    enum Method : unsigned int {
        POST    = 0,
        GET     = 1,
        PUT     = 2,
        PATCH   = 3,
        DELETE  = 4,
        HEAD    = 5,
        NOT_ALLOWED = 6
    };

    /*!
     * @brief Constructor
     * @param[in] url URL on which server will be started.
     */
    Server(const string& url);

    /*! @brief Destructor */
    virtual ~Server();

    /*! @brief Starts server */
    virtual void open() = 0;

    /*! @brief Stops server */
    virtual void close() = 0;

    /*!
     * @brief MethodCallback setter.
     *
     * @param[in] callback MethodCallback for all supported method types.
     */
    void support(MethodCallback callback);

    /*!
     * @brief MethodCallback setter.
     *
     * @param[in] method HTTP Method type.
     * @param[in] callback MethodCallback for given method type.
     */
    void support(const Method method, MethodCallback callback);

    /*!
     * @brief MethodCallback executor.
     *
     * @param[in] method HTTP Method type.
     * @param[in] request Request object.
     * @param[in] response Response object.
     */
    void call(const Method method, const Request& request, Response& response);

    /*!
     * @brief Server url getter.
     *
     * @return Server url.
     */
    const string& get_url() const { return m_url; }

private:
    string m_url;
    static constexpr unsigned int MAX_METHODS = Method::NOT_ALLOWED;
    std::array<MethodCallback, MAX_METHODS> m_method_callback;
};

} /* namespace http */
} /* namespace rest */
} /* namespace psme */

#endif /* PSME_REST_HTTP_SERVER_HPP */
