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
 * @file targets.hpp
 *
 * @brief Declaration of Target and Targets classes
 * */
#ifndef PSME_REST_NODE_TARGETS_HPP
#define PSME_REST_NODE_TARGETS_HPP

#include "psme/rest/node/node.hpp"

namespace psme {
namespace rest {
namespace node {

/*!
 * @brief Node representing Target collection resource.
 * */
class Targets : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "Targets";
    /*! @brief Link name */
    static constexpr const char REMOTE_TARGETS[] = "RemoteTargets";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    Targets(const string& uuid = "",
            const string& gami_id = "",
            const string& id = TYPE);

    /*! @brief Destructor */
    ~Targets();

    /*!
     * @brief Tells if node represents resource collection.
     *
     * @return true if node represents resource collection, false otherwise.
     * */
    bool is_collection() const override { return true;}

    /*!
     * @brief Gets node type.
     *
     * @return Node's type.
     * */
    const char* get_type() const override { return TYPE;}

    /*!
     * @brief POST HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
     void post(const Request& request, Response& response) override;

protected:
    std::string generate_child_id() const override;
};

/*!
 * @brief Node representing Target resource.
 * */
class Target : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "Target";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    Target(const string& uuid = "",
           const string& gami_id = "",
           const string& id = "");

    /*! @brief Destructor */
    ~Target();

    /*!
     * @brief Gets node type.
     *
     * @return Node's type.
     * */
    const char* get_type() const override { return TYPE;}

    /*!
     * @brief DELETE HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    void del(const Request& request, Response& response) override;

    /*!
     * @brief PATCH HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
     void patch(const Request& request, Response& response) override;
};

}
}
}

#endif /* PSME_REST_NODE_TARGETS_HPP */
