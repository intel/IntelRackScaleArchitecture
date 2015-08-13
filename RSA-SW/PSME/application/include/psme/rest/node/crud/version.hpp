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
 * @file crud/version.hpp
 *
 * @brief Declaration of Version class
 * */
#ifndef PSME_REST_NODE_VERSION_HPP
#define PSME_REST_NODE_VERSION_HPP

#include "psme/rest/node/node.hpp"

namespace json {
    /*! Forward declaration */
    class Value;
}

namespace psme {
namespace rest {
namespace node {

/*!
 * @brief Node representing ServiceRoot resource.
 * */
class Version : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "v1";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    Version(const string& uuid = "",
            const string& gami_id = "",
            const string& id = TYPE);

    /*! @brief Destructor */
    ~Version();

    /*!
     * @brief Gets node type.
     *
     * @return Node's type.
     * */
    const char* get_type() const override { return TYPE;}

    /*!
     * @brief GET HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    void get(const Request& request, Response& response) override;

    /*!
     * @brief Reads/Generates persistent service UUID.
     *
     * @param[in] config JSON configuration object.
     *
     * */
    void handle_service_uuid(const json::Value& config);
};

}
}
}

#endif /* PSME_REST_NODE_VERSION_HPP */
