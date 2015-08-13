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
 * @file tree_manager.hpp
 *
 * @brief Declaration of TreeManager class
 * */
#ifndef PSME_REST_NODE_TREE_MANAGER_HPP
#define PSME_REST_NODE_TREE_MANAGER_HPP

#include <memory>

namespace json {
    /*! Forward declaration */
    class Value;
}

namespace psme {
namespace rest {

namespace http {
    /*! Forward declarations */
    class Request;
    class Response;
}

namespace node {

using http::Request;
using http::Response;

/*!
 * @brief TreeManager.
 *
 * Creates tree nodes and updates tree based on data gathered from agents.
 * */
class TreeManager {
public:
    /*!
     * @brief Constructor.
     *
     * Creates nodes tree structure.
     *
     * @param[in] config Configuration (JSON object)
     * */
    TreeManager(const json::Value& config);

    /*! @brief Destructor */
    ~TreeManager();

    /*!
     * @brief Start thread updating tree
     */
    void start();

    /*!
     * @brief Stop thread updating tree
     */
    void stop();

    /*!
     * @brief GET HTTP method handler
     * @param[in] request HTTP request object
     * @param[out] response HTTP response object
     */
    void get(const Request& request, Response& response);

    /*!
     * @brief DELETE HTTP method handler
     * @param[in] request HTTP request object
     * @param[out] response HTTP response object
     */
    void del(const Request& request, Response& response);

    /*!
     * @brief PUT HTTP method handler
     * @param[in] request HTTP request object
     * @param[out] response HTTP response object
     */
    void put(const Request& request, Response& response);

    /*!
     * @brief POST HTTP method handler
     * @param[in] request HTTP request object
     * @param[out] response HTTP response object
     */
    void post(const Request& request, Response& response);

    /*!
     * @brief PATCH HTTP method handler
     * @param[in] request HTTP request object
     * @param[out] response HTTP response object
     */
    void patch(const Request& request, Response& response);

    /*!
     * @brief HEAD HTTP method handler
     * @param[in] request HTTP request object
     * @param[out] response HTTP response object
     */
    void head(const Request& request, Response& response);

private:
    /*! pimpl idiom */
    class EventBasedImpl;
    std::unique_ptr<EventBasedImpl> m_impl;

    TreeManager(const TreeManager&) = delete;
    TreeManager& operator=(const TreeManager&) = delete;
};

}
}
}
#endif /* PSME_REST_NODE_TREE_MANAGER_HPP */
