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
 * @file node_builder.hpp
 *
 * @brief Declaration of NodeBuilder class
 * */

#ifndef PSME_REST_NODE_NODE_BUILDER_HPP
#define PSME_REST_NODE_NODE_BUILDER_HPP

#include "psme/rest/node/node.hpp"
#include "core/service/agent_service.hpp"
#include "core/agent/agent.hpp"

#include <vector>

namespace psme {
namespace core {
namespace dto {
    /*! Forward declaration */
    class ManagerInfoDTO;
}
}
}

namespace psme {
namespace rest {
namespace node {

/*!
 * @enum LinkType
 * @brief Enumerates link types
 *
 * @var ASSOCIATION
 * Only association.
 *
 * @var COMPOSITION
 * Adds node as a child
 * */
enum class LinkType {
    ASSOCIATION,
    COMPOSITION
};

/*! @brief Specifies relation between two nodes */
struct NodesLink {
    /*!
     * @brief Constructor
     *
     * @param link_type Type of link (association or composition).
     * @param[in] first_link_name Link name on first node side.
     * @param first Node to be added to tree.
     * @param second Node to be added to tree.
     * @param[in] second_link_name Link name on second node side.
     * */
    NodesLink(LinkType link_type,
              const string& first_link_name,
              Node& first,
              NodeSharedPtr second,
              const string& second_link_name = "")
    : m_link_type(link_type),
      m_first_link_name(first_link_name),
      m_first(first),
      m_second(second),
      m_second_link_name(second_link_name) { }
    /*! @brief Link type */
    LinkType m_link_type;
    /*! @brief Link name on first node side */
    const string m_first_link_name;
    /*! @brief First node */
    Node& m_first;
    /*! @brief Second node */
    NodeSharedPtr m_second;
    /*! @brief Link name on second node side */
    const string m_second_link_name;
};

using NodesLinkVec = std::vector<NodesLink>;
using psme::core::service::AgentService;

/*! @brief Base abstract class for node creation */
class NodeBuilder {
public:
    /*! @brief Default constructor */
    NodeBuilder();
    /*! @brief Destructor */
    virtual ~NodeBuilder();

    /*!
     * @brief Builds node and it's subtree (if any).
     *
     * @param root Tree's root node.
     * @param component_id Id of node to build.
     * @return Vector of links between nodes.
     */
    virtual NodesLinkVec build_nodes(Node& root, const string& component_id) = 0;

    /*!
     * @brief Links two nodes.
     *
     * Depending on link_type an association or composition relation is created.
     * In case of composition link the order matters, first node becomes a parent
     * of second node, additionally an association is created between
     * parent and child nodes.
     *
     * @param link_type Type of link (association or composition).
     * @param first_link_name Link name on first node side.
     * @param first Node to be added to tree.
     * @param second Node to be added to tree.
     * @param second_link_name Link name on second node side.
     * */
    static void link_nodes(LinkType link_type,
        const string& first_link_name,
        Node& first,
        NodeSharedPtr second,
        const string& second_link_name = "");

protected:
    /*!
     * @brief Builds manager node.
     *
     * It does not affect current tree structure, builds the manager node aside.
     *
     * @param service Service to call get_manager_info
     * @param managed_node Node managed by this manager
     * @param uuid Manager's uuid (optional)
     *
     * @return Pointer to created node.
     * */
    NodeSharedPtr
    build_manager(AgentService& service,
                  const Node& managed_node, const std::string& uuid = "");

    /*!
     * @brief Builds manager node.
     *
     * It does not affect current tree structure, builds the manager node aside.
     * This version does not need AgentService (Drawer case).
     *
     * @param manager_info Manager info class
     * @param managed_node Node managed by this manager
     * @param uuid Manager's uuid (optional)
     *
     * @return Pointer to created node.
     * */
    NodeSharedPtr
    build_manager(const psme::core::dto::ManagerInfoDTO::Response& manager_info,
        const Node& managed_node, const std::string& uuid = "");

    /*!
     * @brief Builds storage manager node
     *
     * @param manager_info Manager info class
     *
     * @return Pointer to created node
     * */
    NodeSharedPtr
    build_storage_manager(
            const psme::core::dto::ManagerInfoDTO::Response& manager_info);

    /*!
     * @brief Creates NetworkService for Manager.
     *
     * @param manager Manager node to which NetworkService is added
     * @param network_services Available Network Services returned from JSONRPC
     * */
    void
    populate_networkservice(Node& manager,
                            const psme::core::dto::NetworkServices& network_services);
};

using NodeBuilderUPtr = std::unique_ptr<NodeBuilder>;

using psme::core::agent::AgentSharedPtr;

/*! @brief Abstract class for node creation based on data provided by agents */
class AgentNodeBuilder : public NodeBuilder {
public:
    /*!
     * @brief Constructor
     *
     * @param agent Pointer to agent
     * */
    AgentNodeBuilder(AgentSharedPtr agent) : m_agent(agent) { }

    /*! @brief Destructor */
    ~AgentNodeBuilder();

protected:
    AgentSharedPtr get_agent() {
        return m_agent;
    }

private:
    AgentSharedPtr m_agent;
};

}
}
}
#endif	/* PSME_REST_NODE_NODE_BUILDER_HPP */

