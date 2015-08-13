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
 * @file storage_node_builder.hpp
 *
 * @brief Declaration of StorageNodeBuilder class
 * */

#ifndef PSME_REST_NODE_STORAGE_NODE_BUILDER_HPP
#define	PSME_REST_NODE_STORAGE_NODE_BUILDER_HPP

#include "psme/rest/node/builders/node_builder.hpp"
#include "core/dto/component_dto.hpp"

#include <list>

namespace json {
    class Value;
}

namespace psme {

namespace core {
namespace service {
    /* Forward declaration */
    class StorageService;
}
}

namespace rest {
namespace node {

using psme::core::dto::ComponentDTO;
using psme::core::service::StorageService;

class StorageNodeBuilder: public AgentNodeBuilder {
public:
    /*!
     * @brief Constructor
     *
     * @param agent Pointer to agent
     * */
    StorageNodeBuilder(AgentSharedPtr agent)
        : AgentNodeBuilder(agent) { }

    /*! @brief Destructor */
    ~StorageNodeBuilder();

    /*! @brief Copy Constructor */
    StorageNodeBuilder(const StorageNodeBuilder&) = default;

    /*! @brief Assignment operator */
    StorageNodeBuilder& operator=(const StorageNodeBuilder&) = default;

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
     * @brief Creates service node.
     *
     * It does not affect current tree structure,
     * builds and populates service node.
     *
     * @return Reference to created node.
     * */
    NodeSharedPtr create_service(const std::string& component_uuid);

    /*!
     * @brief Builds drives.
     *
     * It does not affect current tree structure,
     * builds and populates drives.
     *
     * @param storage_service StorageService reference
     * @param parent Parent node
     * @param component_uuid The storage services uuid
     * */
    void build_drives(StorageService& storage_service, Node& parent,
                      const std::string& component_uuid,
                      const std::string& collection);

    /*!
     * @brief Builds and populates targets.
     *
     * @param service StorageService reference
     * @param parent targets parent node
     * @param storage_manager_node StorageService manager
     * @param logical_drives Logical drives node
     * @param component_uuid The component's uuid
     * @param collection Collection name
     * @return Vector of links between nodes.
     **/
    NodesLinkVec
    build_targets(StorageService& service, Node& parent,
                      Node& storage_manager_node,
                      const Node& logical_drives,
                      const std::string& component_uuid,
                      const std::string& collection);

    /*!
     * @brief Builds and populates logical drives.
     *
     * @param service StorageService reference
     * @param parent targets parent node
     * @param storage_manager_node StorageService manager
     * @param component_uuid The component's uuid
     * @param collection_name Collection name
     * @return Vector of links between nodes.
     **/
    NodesLinkVec
    build_logical_drives(StorageService& service, Node& parent,
                      Node& storage_manager_node,
                      const std::string& component_uuid,
                      const std::string& collection_name);

    /*!
     * @brief Return all children from logical_drives collection
     * for specified logical_drive parent
     *
     * @param parent LogicalDrive parent
     * @param collection_name Name of the collection
     * @param storage_service The storage service
     * @param root_node Highest node in the tree hierarchy to search for children
     * @return logical_drive children selected from logical_drives collection
     * */
    std::list<Node*>
    find_children(const Node& parent,
                  const std::string& collection_name,
                  StorageService& storage_service, const Node& root_node);

    /*!
     * @brief Resolves dependencies for storage components
     * {UsedBy, LogicalDrives, PhysicalDrives, etc}
     *
     * @param logical_drives_node Node that aggregates all LogicalDrives
     * @param drives_node Node that aggregates all PhysicalDrives
     * @param targets_node Node that aggregates all RemoteTargets
     * @param storage_service The storage service
     * */
    void resolve_dependencies(Node& logical_drives_node, Node& drives_node,
                              Node& targets_node, StorageService& storage_service);

    std::string get_node_id(const std::string& uuid);

    Node* m_root{nullptr};
};

}
}
}

#endif	/* STORAGE_NODE_BUILDER_HPP */

