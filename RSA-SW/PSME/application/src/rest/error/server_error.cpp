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
#include "psme/rest/error/server_error.hpp"
#include "psme/rest/utils/enum_chars.hpp"

using namespace psme::rest::error;

namespace {
    constexpr char ERROR[] = "error";
    constexpr char DETAILS[] = "details";
    constexpr char CODE[] = "code";
    constexpr char MESSAGE[] = "message";
    constexpr char TARGET[] = "target";
}

ServerError::~ServerError() { }

ServerError::ServerError()
    : m_http_status_code(HttpStatusCode::INTERNAL_SERVER_ERROR),
      m_error(json::Value::Type::OBJECT) {
    set_code("UnknownException");
    set_message("Unknown Exception");
}

ServerError::ServerError(HttpStatusCode status_code)
    : m_http_status_code(status_code),
      m_error(json::Value::Type::OBJECT) {
    set_code(std::to_string(utils::to_integral(m_http_status_code)));
    set_message(utils::to_string(m_http_status_code));
}

void ServerError::set_http_status_code(HttpStatusCode status_code) {
    m_http_status_code = status_code;
}

void ServerError::add_error_detail(ErrorDetail& detail) {
    if (!m_error[ERROR].is_member(DETAILS)) {
        m_error[ERROR][DETAILS] = json::Value::Type::ARRAY;
    }
    m_error[ERROR][DETAILS].push_back(detail.as_json());
}

void ServerError::set_code(const string& code) {
    m_error[ERROR][CODE] = code;
}

const string& ServerError::get_code() const {
    return m_error[ERROR][CODE].as_string();
}

void ServerError::set_message(const string& message) {
    m_error[ERROR][MESSAGE] = message;
}

const string& ServerError::get_message() const {
    return m_error[ERROR][MESSAGE].as_string();
}

string ServerError::as_string() const {
    return json::Serializer(m_error);
}

ErrorDetail::ErrorDetail() : m_error(json::Value::Type::OBJECT) { }

ErrorDetail::~ErrorDetail() { }

void ErrorDetail::set_code(const string& code) {
    m_error[CODE] = code;
}

const string& ErrorDetail::get_code() const {
    return m_error[CODE].as_string();
}

void ErrorDetail::set_message(const string& message) {
    m_error[MESSAGE] = message;
}

const string& ErrorDetail::get_message() const {
    return m_error[MESSAGE].as_string();
}

void ErrorDetail::set_target(const string& target) {
    m_error[TARGET] = target;
}

string ErrorDetail::as_string() const {
    return json::Serializer(m_error);
}
