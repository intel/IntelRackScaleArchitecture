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

#include "psme/rest/http/headers.hpp"

#include <sstream>

using namespace psme::rest::http;

constexpr const char HttpHeaders::LOCATION[];
constexpr const char HttpHeaders::HOST[];

void HttpHeaders::add_header(const std::string& key, const std::string& value) {
    auto it = m_headers.find(key);
    if (it != m_headers.end()) {
        it->second.add_value(value);
    } else {
        m_headers.emplace(key, HeaderValues{value});
    }
}

void HttpHeaders::add_header(const std::string& key, const HeaderValues& values) {
    auto it = m_headers.find(key);
    if (it != m_headers.end()) {
        for (const auto& value: values) {
            it->second.add_value(value);
        }
    } else {
        m_headers.emplace(key, values);
    }
}

HttpHeaders::CIterator HttpHeaders::begin() const {
    return m_headers.cbegin();
}

HttpHeaders::CIterator HttpHeaders::end() const {
    return m_headers.cend();
}

HttpHeaders::HeaderValues::HeaderValues(const std::string& value) : m_values() {
    add_value(value);
}

const HttpHeaders::HeaderValues& HttpHeaders::get_header_values(
                                            const std::string& name) const {
    return m_headers.at(name);
}

std::string HttpHeaders::HeaderValues::as_string() const {
    std::ostringstream values_str;
    auto it = m_values.cbegin();
    if (it != m_values.cend()) {
        values_str << *it;
        for (++it; it != m_values.cend(); ++it) {
            values_str << "," << *it;
        }
    }
    return values_str.str();
}

void HttpHeaders::HeaderValues::add_value(const std::string& value) {
    m_values.emplace_back(value);
}

HttpHeaders::HeaderValues::CIterator HttpHeaders::HeaderValues::begin() const {
    return m_values.cbegin();
}

HttpHeaders::HeaderValues::CIterator HttpHeaders::HeaderValues::end() const {
    return m_values.cend();
}
