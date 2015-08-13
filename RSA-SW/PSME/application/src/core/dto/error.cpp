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

#include "error.hpp"

using namespace psme::core::dto;

Error::Error() : Error(NO_ERROR) { }

Error::Error(int code, const std::string& msg, const Json::Value& data)
    : m_code(code), m_message(msg), m_data(data) { }

int Error::get_code() const {
    return m_code;
}

const Json::Value& Error::get_data() const {
    return m_data;
}

const std::string& Error::get_message() const {
    return m_message;
}

Json::Value Error::as_json() const {
    Json::Value error;
    error["code"] = std::to_string(m_code);
    error["message"] = m_message;
    return error;
}

std::string Error::to_string() const {
    return as_json().toStyledString();
}
