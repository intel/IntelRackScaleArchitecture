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
 * @file network_node_builder.hpp
 *
 * @brief Declaration of NetworkNodeBuilder class
 * */

#ifndef PSME_REST_NODE_NETWORK_NODE_BUILDER_HPP
#define PSME_REST_NODE_NETWORK_NODE_BUILDER_HPP

#include "psme/rest/node/builders/node_builder.hpp"

namespace psme {
namespace core {
namespace service {
    /*! forward declaration */
    class NetworkService;
}
}
}

namespace json {
    class Value;
}

namespace psme {
namespace rest {
namespace node {

using psme::core::service::NetworkService;

/*! @brief Builds REST Network nodes. */
class NetworkNodeBuilder: public AgentNodeBuilder {
public:
    /*!
     * @brief Constructor
     *
     * @param agent Pointer to agent
     * */
    NetworkNodeBuilder(AgentSharedPtr agent)
        : AgentNodeBuilder(agent) { }

    /*! @brief Destructor */
    ~NetworkNodeBuilder();

    /*!
     * @brief Builds node and it's subtree (if any).
     *
     * @param root Tree's root node.
     * @param component_id Id of node to build.
     * @return Vector of links between nodes.
     * */
    NodesLinkVec build_nodes(Node& root, const string& component_id);

private:

    /**
     * Finds neighbor switch and port index with format:
     * switch_identifier:port_identifier. If something cannot be read then will
     * be replaced with empty string. I.e ":sw0p25" or "switchorg123:" or just
     * "".
     * @param service reference to Network Service needed to invoke commands.
     * @param component switch component uuid.
     * @param port_identifier port identifier, NOT uuid.
     * @return string with format switch_identifier:port_identifier
     */
    inline const std::string find_neighbor(NetworkService& service,
                                           const std::string& component,
                                           const std::string& port_identifier);


    /**
     * Returns port identifier connected to Switch with identifier passed as an
     * @param service reference to Network Service needed to invoke commands.
     * @param component switch component uuid.
     * @param switch_identifier switch identifier, NOT uuid.
     * @return port identifier connected to switch with switch_identifier.
     */
    inline const std::string
    get_port_identifier(NetworkService& service,
                        const std::string& component,
                        const std::string& switch_identifier);

    /*!
     * @brief Builds fabric module.
     *
     * It does not affect current tree structure,
     * builds and populates fabric module aside.
     *
     * @param service NetworkService reference
     * @param uuid Fabric module uuid
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_fabric_module(NetworkService& service, const string& uuid);

    /*!
     * @brief Builds switch under fabric module.
     *
     * It does not affect current tree structure,
     * builds and populates switch collection aside.
     *
     * @param service NetworkService reference
     * @param uuid Switch node uuid
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_switch(NetworkService& service, const string& uuid);

    /*!
     * @brief Builds switch ports under switch.
     *
     * It does not affect current tree structure,
     * builds and populates switch port collection aside.
     *
     * @param[in] service NetworkService reference
     * @param[in] switch_node Switch node reference
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_switch_ports(NetworkService& service, Node& switch_node);

    /*!
     * @brief Builds vlans under switch port.
     *
     * It does not affect current tree structure,
     * builds and populates vlan collection aside.
     *
     * @param[in] service NetworkService reference
     * @param[in] switch_port Switch port node reference
     * @param[in] port_identifier Port identifier
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_vlans(NetworkService& service, Node& switch_port,
                const string& port_identifier);
};

}
}
}
#endif	/* PSME_REST_NODE_NETWORK_NODE_BUILDER_HPP */
