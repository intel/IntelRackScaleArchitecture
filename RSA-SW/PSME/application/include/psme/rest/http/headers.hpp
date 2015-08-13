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
 * @file headers.hpp
 *
 * @brief Declaration of HTTP Headers container class
 * */

#ifndef PSME_REST_HTTP_HEADERS_HPP
#define PSME_REST_HTTP_HEADERS_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace psme {
namespace rest {
namespace http {

using std::string;

/*!
 * @brief HTTP Headers container.
 *
 * */
class HttpHeaders {
public:
    /*! @brief Location of URL redirection */
    static constexpr const char LOCATION[] = "Location";
    /*! @brief Host address */
    static constexpr const char HOST[] = "Host";

    /*!
     * @brief HTTP Header values.
     *
     * */
    class HeaderValues {
    public:
        using CIterator = std::vector<string>::const_iterator;

        /*!
         * @brief Constructor.
         *
         * @param[in] value Header value
         * */
        explicit HeaderValues(const std::string& value);

        /*!
         * @brief Get Header value(s) as string.
         *
         * @return Header value(s) (comma separated) as string.
         */
        std::string as_string() const;

        /*!
         * @brief Add/Append header value.
         *
         * @param[in] value Header value.
         */
        void add_value(const std::string& value);

        /*!
         * @brief Returns constant iterator to beginning
         *
         * @return A read-only (constant) iterator that points to the first
         * header in collection.
         * */
        CIterator begin() const;

        /*!
         * @brief Return constant iterator to end
         *
         * @return A read-only (constant) iterator that points
         * one past the last header value in collection.
         * */
        CIterator end() const;

    private:
        std::vector<string> m_values;
    };

    /*!
     * @brief Header values getter.
     *
     * @param[in] name Header name.
     *
     * @return Header values.
     */
    const HeaderValues& get_header_values(const std::string& name) const;

    /*!
     * @brief Add header/value pair.
     *
     * @param[in] name Header name.
     * @param[in] value Header value.
     */
    void add_header(const std::string& name, const std::string& value);

    /*!
     * @brief Add header with HeaderValues.
     *
     * @param[in] name Header name.
     * @param[in] values Header values.
     */
    void add_header(const std::string& name, const HeaderValues& values);

    using CIterator = std::unordered_map<string, HttpHeaders::HeaderValues>::const_iterator;

    /*!
     * @brief Returns constant iterator to beginning
     *
     * @return A read-only (constant) iterator that points to the first
     * header in collection.
     * */
    CIterator begin() const;

    /*!
     * @brief Return constant iterator to end
     *
     * @return A read-only (constant) iterator that points
     * one past the last header in collection.
     * */
    CIterator end() const;

private:
    std::unordered_map<string, HeaderValues> m_headers{};
};

} /* namespace http */
} /* namespace rest */
} /* namespace psme */

#endif /* PSME_REST_HTTP_HEADERS_HPP */
