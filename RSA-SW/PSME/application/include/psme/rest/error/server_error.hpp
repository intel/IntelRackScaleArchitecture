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
 * @file server_error.hpp
 *
 * @brief Declaration of ServerError and ErrorDetail classes
 * */
#ifndef PSME_REST_ERROR_SERVERERROR_HPP
#define	PSME_REST_ERROR_SERVERERROR_HPP

#include "psme/rest/http/http_status_code.hpp"

#include "json/json.hpp"
#include <string>
#include <exception>

namespace psme {
namespace rest {
namespace error {

using std::string;
using psme::rest::http::HttpStatusCode;

/*! Forward declaration */
class ErrorDetail;

/*!
 * @brief Rest ServerError for extended error handling compatible with redfish spec.
 *
 * */
class ServerError {
public:
    /*!
     * @brief Default constructor.
     *
     * Create ServerError with INTERNAL_SERVER_ERROR HTTP status code.
     * */
    ServerError();

    /*!
     * @brief Create ServerError object with given HTTP status code.
     *
     * @param[in] http_status_code HTTP status code associated with the error.
     * */
    explicit ServerError(HttpStatusCode http_status_code);

    /*! @brief Default copy constructor */
    ServerError(const ServerError&) = default;

    /*! @brief Assignment operator */
    ServerError& operator=(const ServerError&) = default;

    /*! @brief Destructor */
    ~ServerError();

    /*!
     * @brief Set HTTP status code.
     *
     * @param[in] http_status_code HTTP status code associated with the error.
     * */
    void set_http_status_code(HttpStatusCode http_status_code);

    /*!
     * @brief Get HTTP status code.
     *
     * @return HTTP status code associated with the error.
     * */
    HttpStatusCode get_http_status_code() const { return m_http_status_code; }

    /*!
     * @brief Set error code.
     *
     * @param[in] code  String indicating a specific error or message
     * (not to be confused with the HTTP status code). This code can be used
     * to access a detailed message from message registry.
     * */
    void set_code(const string& code);

    /*!
     * @brief Error code getter.
     *
     * @return Error code.
     * */
    const string& get_code() const;

    /*!
     * @brief Set error message.
     *
     * @param[in] message  A human readable error message indicating the semantics
     * associated with the error.
     * */
    void set_message(const string& message);

    /*!
     * @brief Error message getter.
     *
     * @return Error message.
     * */
    const string& get_message() const;

    /*!
     * @brief Add error detail.
     *
     * Add error detail to an optional array of JSON objects with code, message,
     * target, severity, and resolution properties, providing more detailed
     * information about the error.
     *
     * @param[in] error_detail error detail.
     * */
    void add_error_detail(ErrorDetail& error_detail);

    /*!
     * @brief Convert error JSON value to string
     *
     * @return JSON value as a string
     * */
    string as_string() const;

private:
    HttpStatusCode m_http_status_code;
    json::Value m_error;
};

/*!
 * @brief Rest ErrorDetail for extended error handling compatible with redfish spec.
 *
 * */
class ErrorDetail {
public:
    /*!
     * @brief Set error code.
     *
     * @param[in] code  String indicating a specific error or message
     * (not to be confused with the HTTP status code). This code can be used
     * to access a detailed message from message registry.
     * */
    void set_code(const string& code);

    /*!
     * @brief Error code getter.
     *
     * @return Error code.
     * */
    const string& get_code() const;

    /*!
     * @brief Set error message.
     *
     * @param[in] message  A human readable error message indicating the semantics
     * associated with the error.
     * */
    void set_message(const string& message);

    /*!
     * @brief Error message getter.
     *
     * @return Error message.
     * */
    const string& get_message() const;

    /*!
     * @brief Set error target.
     *
     * @param[in] target An optional string defining the target of the particular
     * error.
     * */
    void set_target(const string& target);

    /*!
     * @brief Get JSON representation of the error.
     *
     * @return  JSON object representation of the error.
     * */
    const json::Value& as_json() const { return m_error; }

    /*!
     * @brief Convert error JSON value to string
     *
     * @return JSON value as a string
     * */
    string as_string() const;

    /*! @brief Default constructor */
    ErrorDetail();

    /*! @brief Default copy constructor */
    ErrorDetail(const ErrorDetail&) = default;

    /*! @brief Assignment operator */
    ErrorDetail& operator=(const ErrorDetail&) = default;

    /*! @brief Destructor */
    ~ErrorDetail();

protected:
    json::Value m_error;
};

}
}
}

#endif	/* PSME_REST_ERROR_SERVERERROR_HPP */

