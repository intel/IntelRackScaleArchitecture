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
 * @file compute_node_builder.hpp
 *
 * @brief Declaration of ComputeNodeBuilder class
 * */

#ifndef PSME_REST_NODE_COMPUTE_NODE_BUILDER_HPP
#define PSME_REST_NODE_COMPUTE_NODE_BUILDER_HPP

#include "psme/rest/node/builders/node_builder.hpp"

namespace psme {
namespace core {
namespace service {
    /*! forward declaration */
    class ComputeService;
}
}
}

namespace json {
    class Value;
}

namespace psme {
namespace rest {
namespace node {

using psme::core::service::ComputeService;

/*! @brief Builds REST Compute nodes. */
class ComputeNodeBuilder: public AgentNodeBuilder {
public:
    /*!
     * @brief Constructor
     *
     * @param agent Pointer to agent
     * */
    ComputeNodeBuilder(AgentSharedPtr agent)
        : AgentNodeBuilder(agent) { }

    /*! @brief Destructor */
    ~ComputeNodeBuilder();

    /*!
     * @brief Builds node and it's subtree (if any).
     *
     * @param root Tree's root node.
     * @param component_id Id of node to build.
     * @return Vector of links between nodes.
     * */
    NodesLinkVec build_nodes(Node& root, const string& component_id);

private:
    /*!
     * @brief Builds compute module node.
     *
     * It does not affect current tree structure, builds the compute module
     * node aside.
     *
     * @param uuid Node uuid
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_compute_module(ComputeService& service, const std::string& uuid);

    /*!
     * @brief Builds blade node.
     *
     * It does not affect current tree structure, builds the blade node aside.
     *
     * @param uuid Node uuid
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_blade(ComputeService& service, const std::string& uuid);

    /*!
     * @brief Builds processors for blade.
     *
     * It does not affect current tree structure,
     * builds and populates processors collection aside.
     *
     * @param blade Processors owner
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_processors(ComputeService& service, Node& blade,
                     json::Value& blade_json, std::uint32_t slot_count);

    /*!
     * @brief Builds memory for blade.
     *
     * It does not affect current tree structure,
     * builds and populates memory collection aside.
     *
     * @param service JSONRPC compute service.
     * @param blade Memory owner
     * @param blade_json Blade's JSON representation.
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_memory(ComputeService& service, Node& blade,
                 json::Value& blade_json, std::uint32_t slot_count);

    /*!
     * @brief Builds storage controllers for blade.
     *
     * It does not affect current tree structure,
     * builds and populates storage controllers collection aside.
     *
     * @param service JSONRPC compute service.
     * @param blade Storage controllers owner
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_storage_controllers(ComputeService& service, Node& blade,
                              std::uint32_t slot_count);

    /*!
     * @brief Builds storage controllers for blade.
     *
     * It does not affect current tree structure,
     * builds and populates storage controllers collection aside.
     *
     * @param service JSONRPC compute service.
     * @param storage_controller Drives owner
     * @param sc_idx Storage controller's index (JSONRPC query parameter)
     * @param drive_count Number of drives to query
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_drives(ComputeService& service, Node& storage_controller,
                 std::uint32_t sc_idx, std::uint32_t drive_count);

    /*!
     * @brief Creates EthernetInterfaces for Blade.
     *
     * @param service JSONRPC compute service.
     * @param blade Network Interface owner.
     * @param count Number of EthernetInterfaces.
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr
    build_ethernet_interfaces(ComputeService& service, Node& blade,
                              std::uint32_t count);


    /*!
     * @brief Update drawer location id
     *
     * @param service JSONRPC compute service
     * @param drawer Drawer node object
     * @param component Chassis component uuid
     */
    void
    update_drawer_location(ComputeService& service, Node* drawer,
                           const std::string& component);
};

}
}
}
#endif	/* PSME_REST_NODE_COMPUTE_NODE_BUILDER_HPP */

