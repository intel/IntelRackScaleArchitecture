/*!
 * @copyright Copyright (c) 2015, Tymoteusz Blazejczyk
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of OpenRedfish nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file node.hpp
 *
 * @brief Declaration of virtual Node class
 */

#ifndef PSME_REST_NODE_HPP
#define PSME_REST_NODE_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>

namespace psme {
namespace rest {

/*! Forward declarations */
namespace http {
    class Request;
    class Response;
}

namespace resource {
    class Resource;
}

namespace node {

using std::map;
using std::pair;
using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;

using psme::rest::http::Request;
using psme::rest::http::Response;
using psme::rest::resource::Resource;

/*! Forward declarations */
class NodeFactory;
class Node;

using NodeSharedPtr = std::shared_ptr<Node>;

/*!
 * @brief Default http not allowed handler
 *
 * @param[in] request HTTP request object
 * @param[out] response HTTP response object
 * */
void http_method_not_allowed(const Request& request, Response& response);

/*!
 * @brief Composite pattern. Base class for accessing web resources.
 *
 * */
class Node {
public:
    using NodePredicate = std::function<bool(const Node&)>;
    using NodeVisitor = std::function<void(Node&)>;

    /*! @brief Link representation. */
    struct Link {
        /*!
         * @brief Constructor
         *
         * @param[in] name Link name
         * @param node Linked node
         * */
        Link(const string& name, Node* node)
            : m_name(name), m_node(node) { }

        /*! @brief Copy constructor */
        Link(const Link&) = default;
        /*! @brief Assignment operator */
        Link& operator=(const Link&) = default;

        /*! @brief Link name. */
        string m_name;
        /*! @brief Linked node. */
        Node* m_node;
    };

    using Links = vector<Link>;

    /*!
     * @brief Constructor
     * @param[in] uuid Node's uuid known to agent.
     * @param[in] gami_id Node's agent uuid.
     * @param[in] id Node id in node's path.
     * @param[in] resource Node's web resource.
     */
    Node(const string& uuid,
         const string& gami_id,
         const string& id,
         std::unique_ptr<Resource> resource);

    /*! @brief Destructor */
    virtual ~Node();
    /*! @brief Disabled copy constructor */
    Node(const Node&) = delete;
    /*! @brief Disabled assignment operator */
    Node& operator=(const Node&) = delete;

    /*!
     * @brief Gets node type.
     *
     * @return Node's type.
     * */
    virtual const char* get_type() const = 0;

    /*!
     * @brief Get web resource represented by this node.
     *
     * @return Resource represented by this node.
     * */
    virtual const Resource& get_resource() const { return *m_resource; }

    /*!
     * @brief Get web resource represented by this node.
     *
     * @return Resource represented by this node.
     * */
    virtual  Resource& get_resource()  {
        return const_cast<Resource&>(
                static_cast<const Node*>(this)->get_resource());
    }

    /*!
     * @brief Tells if node represents resource collection.
     *
     * @return true if node represents resource collection, false otherwise.
     * */
    virtual bool is_collection() const;

    /*!
     * @brief GET HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    virtual void get(const Request& request, Response& response);

    /*!
     * @brief DELETE HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    virtual void del(const Request& request, Response& response);

    /*!
     * @brief POST HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    virtual void post(const Request& request, Response& response);

    /*!
     * @brief PATCH HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    virtual void patch(const Request& request, Response& response);

    /*!
     * @brief PUT HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    virtual void put(const Request& request, Response& response);

    /*!
     * @brief HEAD HTTP method handler
     * @param[in] request HTTP request object
     * @param response HTTP response object
     */
    virtual void head(const Request& request, Response& response);

    /*!
     * @brief Adds child node
     * @param node Node to be added
     */
    void add_node(NodeSharedPtr node);

    /*!
     * @brief Removes node
     * @param node Node to be removed
     */
    void erase(Node& node);

    /*!
     * @brief Selects node based on path string (URL like)
     *
     * @param[in] id Path to node
     *
     * @return Reference to node if found, otherwise ServerException
     * with not found error.
     *
     * @throw ServerException with Not Found error message
     */
    Node& get_node_by_id(const string& id) const;

    /*!
     * @brief Selects node by its uuid
     *
     * @param[in] uuid Node's uuid
     *
     * @return Pointer to node if found. Otherwise nullptr
     **/
    Node* get_node_by_uuid(const string& uuid) const;

    /*!
     * @brief Selects root node
     *
     * Selects root node if node is attached to tree.
     *
     * @return Pointer to root node (may be this node)
     */
    const Node* get_root() const;

    /*!
     * @brief Selects first child node.
     *
     * @return Pointer to first child node if present, nullptr otherwise.
     */
    Node* get_next() const;

    /*!
     * @brief Selects parent node.
     *
     * @return Pointer to parent node if present, nullptr otherwise.
     */
    Node* get_back() const { return m_back; }

    /*!
     * @brief Gets number of child nodes.
     *
     * @return Number of child nodes.
     */
    size_t size() const { return m_nodes.size(); }

    /*!
     * @brief Gets path from root to this node.
     *
     * @return Path from root to this node.
     */
    string get_path() const;

    /*!
     * @brief Gets node id.
     *
     * Id is part of node path corresponding to this node.
     *
     * @return Node id.
     */
    const string& get_id() const { return m_id; }

    /*!
     * @brief Gets node UUID.
     *
     * UUID is used for JSON RPC communication.
     *
     * @return Node UUID.
     */
    const string& get_uuid() const { return m_uuid; }

    /*!
     * @brief Gets node agent UUID.
     *
     * @return Node Agent UUID.
     */
    const string& get_gami_id() const { return m_gami_id; }

    /*!
     * @brief Add link to other node.
     *
     * Link is bidirectional. This node keeps pointer to other node and
     * other node keeps pointer to this node.
     *
     * @param[in] link_name Link name on this node's side.
     * @param[in] link Node to link with.
     * @param[in] other_side_link_name Link name on linked node's side.
     * */
    void add_link(const string& link_name,
                  Node& link,
                  const string& other_side_link_name = "");

    /*!
     * @brief Remove link to other node.
     *
     * Link is bidirectional. Operation removes pointer to other node and
     * other node's pointer to this node.
     *
     * @param[in] link      unlinked Node
     * */
    void remove_link(Node& link);

    /*!
     * @brief Remove all links to other nodes.
     *
     * Links are bidirectional. Operation removes pointers to other nodes and
     * other nodes pointer to this node.
     * */
    void clear_links();

    /*!
     * @brief Get reference to links vector
     *
     * @return      Vector containing pointers to other nodes
     * */
    const Links& get_links() const { return m_links; }

    /*!
     * @brief Finds first node in hierarchy for which predicate returns true.
     *
     * Iteration starts on this node and continues for all nodes down the hierarchy.
     *
     * @return Pointer to first node for which NodePredicate returns true,
     * nullptr if no such node found.
     **/
    Node* find_if(NodePredicate f) const;

    /*!
     * @brief Finds first node up in hierarchy for which predicate returns true.
     *
     * Iteration starts on this node's parent and continues for all nodes up the hierarchy.
     *
     * @return Pointer to first node for which NodePredicate returns true,
     * nullptr if no such node found.
     * */
    Node* find_back_if(NodePredicate predicate) const;

    /*!
     * @brief Recursively iterates nodes and executes functor on each visited node.
     *
     * Iteration starts on this node and continues for all nodes down the hierarchy.
     *
     * @param visitor Functor to be executed on each visited node
     * */
    void for_each(NodeVisitor visitor);

    /*!
     * @brief Iterator over child nodes
     *
     * */
    template<typename T>
    class IteratorT {
    public:
       /*! @brief Constructor */
       IteratorT(const T& it) : m_iterator(it) { }
       /*! @brief Dereference operator */
       Node& operator*() const { return *(*m_iterator).second.get(); }
       /*! @brief Increment operator */
       IteratorT& operator++() { m_iterator++; return *this; }
       /*! @brief Inequality operator */
       bool operator!=(const IteratorT& it) const {
           return m_iterator != it.m_iterator;
       }
    private:
       T m_iterator;
    };

    using Iterator = IteratorT<map<string, NodeSharedPtr>::iterator>;
    using CIterator = IteratorT<map<string, NodeSharedPtr>::const_iterator>;

    /*!
     * @brief Return iterator to beginning
     *
     * @return A read/write iterator that points to the first
     * element in children collection.
     */
    Iterator begin() { return {m_nodes.begin()}; }

    /*!
     * @brief Return iterator to end
     *
     * @return A read/write iterator that points one past the last
     * element in children collection.
     */
    Iterator end() { return {m_nodes.end()}; }

    /*!
     * @brief Return constant iterator to beginning
     *
     * @return A read-only (constant) iterator that points to the first
     * element in children collection.
     * */
    CIterator begin() const { return {m_nodes.cbegin()}; }

    /*!
     * @brief Return constant iterator to end
     *
     * @return A read-only (constant) iterator that points
     * one past the last element in children collection.
     */
    CIterator end() const { return {m_nodes.cend()}; }

    template<typename T> friend class IteratorT;

private:
    string m_uuid;
    string m_gami_id;
    string m_id;
    Node* m_back;
    mutable std::uint32_t m_child_id;
    map<string, NodeSharedPtr> m_nodes;
    Links m_links;
    unique_ptr<Resource> m_resource;

protected:
    virtual string generate_child_id() const;
};

}
}
}
#endif /* PSME_REST_NODE_HPP */
