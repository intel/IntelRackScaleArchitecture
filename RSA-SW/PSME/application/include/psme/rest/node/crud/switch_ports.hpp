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
 * @file switch_ports.hpp
 *
 * @brief Declaration of SwitchPort and SwitchPorts classes
 * */
#ifndef PSME_REST_NODE_SWITCH_PORTS_HPP
#define PSME_REST_NODE_SWITCH_PORTS_HPP

#include "psme/rest/node/node.hpp"

namespace psme {
namespace rest {
namespace node {

/*!
 * @brief Node representing SwitchPort collection resource.
 * */
class SwitchPorts : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "SwitchPorts";
    /*! @brief Link name */
    static constexpr const char PORTS[] = "Ports";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    SwitchPorts(const string& uuid = "",
                const string& gami_id = "",
                const string& id = PORTS);

    /*! @brief Destructor */
    ~SwitchPorts();

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
};

/*!
 * @brief Node representing SwitchPort resource.
 * */
class SwitchPort : public Node {
public:
    /*! @brief Type identifier */
    static constexpr const char TYPE[] = "SwitchPort";

    /*! Property name */
    static constexpr const char NEIGHBOUR_PORT[] = "NeighbourPort";

    /*! Property name */
    static constexpr const char LINK_SPEED_GBPS[] = "LinkSpeedGbps";

    /*! Property name */
    static constexpr const char OPERATIONAL_STATE[] = "OperationalState";

    /*! Property name */
    static constexpr const char ADMINISTRATIVE_STATE[] = "AdministrativeState";

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     */
    SwitchPort(const string& uuid = "",
               const string& gami_id = "",
               const string& id = "");

    /*! @brief Destructor */
    ~SwitchPort();

    /*!
     * @brief Gets node type.
     *
     * @return Node's type.
     * */
    const char* get_type() const override { return TYPE;}

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

#endif /* PSME_REST_NODE_SWITCH_PORTS_HPP */
