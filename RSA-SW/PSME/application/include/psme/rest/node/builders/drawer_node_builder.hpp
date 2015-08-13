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
 * @file drawer_node_builder.hpp
 *
 * @brief Declaration of DrawerNodeBuilder class.
 * */

#ifndef PSME_REST_NODE_DRAWER_NODE_BUILDER_HPP
#define PSME_REST_NODE_DRAWER_NODE_BUILDER_HPP

#include "psme/rest/node/builders/node_builder.hpp"


namespace json {
    class Value;
}

namespace psme {
namespace rest {
namespace node {

/*! @brief Builds REST Drawer nodes. */
class DrawerNodeBuilder: public NodeBuilder {
public:
    /*!
     * @brief Constructor
     *
     * @param[in] config Application configuration
     * */
    DrawerNodeBuilder(const json::Value& config) : m_config(config) { }

    /*! @brief Destructor */
    ~DrawerNodeBuilder();

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
     * @brief Builds drawer node.
     *
     * It does not affect current tree structure, builds the drawer node aside.
     *
     * @param name Node name
     *
     * @return Pointer to created node.
     * */
    NodeSharedPtr build_drawer(const std::string& name);

private:
    const json::Value& m_config;
};

}
}
}
#endif	/* PSME_REST_NODE_DRAWER_NODE_BUILDER_HPP */
