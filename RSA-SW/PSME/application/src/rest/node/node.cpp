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
 */

#include "psme/rest/node/node.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/server.hpp"
#include "psme/rest/error/error_factory.hpp"
#include "psme/rest/error/server_exception.hpp"
#include "bits/basic_string.h"

#include <exception>
#include <algorithm>

using namespace psme::rest::node;
using namespace psme::rest::error;

namespace {
    constexpr const char PATH_SEPARATOR = '/';
}

namespace psme {
namespace rest {
namespace node {

void http_method_not_allowed(const Request&, Response& response) {
    auto error = ErrorFactory::create_method_not_allowed_error();
    response.set_reply(error.get_http_status_code(),
                       error.as_string());
}

}
}
}

Node::Node(const string& uuid,
           const string& gami_id,
           const string& id,
           std::unique_ptr<Resource> resource)
  : m_uuid(uuid),
    m_gami_id(gami_id),
    m_id(id),
    m_back(nullptr),
    m_child_id(0),
    m_nodes(),
    m_links(),
    m_resource(std::move(resource))
{ }

Node::~Node() {
    clear_links();
}

void Node::get(const Request&, Response& response) {
    auto& r = get_resource();
    r.update_json_properties(*this);
    response.set_reply(http::HttpStatusCode::OK, r.as_string());
}

void Node::del(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void Node::patch(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void Node::put(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void Node::post(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

void Node::head(const Request& request, Response& response) {
    http_method_not_allowed(request, response);
}

bool Node::is_collection() const {
    return false;
}

void Node::add_node(shared_ptr<Node> node) {
    node->m_back = this;
    if (node->m_id.empty()) {
        node->m_id = generate_child_id();
    }
    m_nodes[node->m_id] = node;
    node->get_resource().update_modified();
}

string Node::generate_child_id() const {
    return std::to_string(++m_child_id);
}

void Node::erase(Node& node) {
    if (nullptr != node.m_back) {
        node.clear_links();
        node.get_back()->m_nodes.erase(node.m_id);
        node.get_back()->get_resource().update_modified();
    }
}

Node* Node::get_node_by_uuid(const string& uuid) const {
    return get_root()->find_if([&uuid](const Node& node) {
        return node.get_uuid() == uuid;
    });
}

Node& Node::get_node_by_id(const string& path) const {
    const Node* node = this;

    if (0 == path.size()) {
        /* Nothing to do */
        return const_cast<Node&>(*node);
    }

    auto path_cbegin = path.cbegin();
    auto path_cend = path.cend();

    /* Absolute or relative path? */
    if (PATH_SEPARATOR == path[0]) {
        while (nullptr != node->m_back) {
            node = node->m_back;
        }
        ++path_cbegin;
    }

    size_t count = 0;
    size_t max_size = 0;

    /* Count maximum string length in path between // */
    std::for_each(path_cbegin, path_cend,
        [&count, &max_size] (const char& ch) {
            if (PATH_SEPARATOR == ch) {
                max_size = (count > max_size) ? count : max_size;
                count = 0;
            } else {
                ++count;
            }
        }
    );

    /* Bound end check */
    if (count > 0) {
        max_size = (count > max_size) ? count : max_size;
    }

    string token {};
    token.reserve(max_size);

    try {
        /* Nodes iterate using path */
        std::for_each(path_cbegin, path_cend,
            [&token, &node] (const char& ch) {
                if (PATH_SEPARATOR == ch) {
                    node = node->m_nodes.at(token).get();
                    token.clear();
                } else {
                    token.push_back(ch);
                }
            }
        );

        /* Bound end check */
        if (token.size() > 0) {
            node = node->m_nodes.at(token).get();
        }
    } catch (const std::out_of_range&) {
        ServerError error = ErrorFactory::create_not_found_error();
        throw ServerException(error);
    }

    return const_cast<Node&>(*node);
}

const Node* Node::get_root() const {
    const Node* node = this;
    while (nullptr != node->m_back) {
        node = node->m_back;
    }
    return node;
}

Node* Node::get_next() const {
    Node* node = nullptr;
    if (!m_nodes.empty()) {
        node = m_nodes.begin()->second.get();
    }
    return node;
}

string Node::get_path() const {
    string path = "";
    const Node* node = this;
    while (nullptr != node->m_back) {
        path = PATH_SEPARATOR + node->get_id() +  path;
        node = node->m_back;
    }
    return path.empty() ? PATH_SEPARATOR + get_id() : path;
}

void Node::add_link(const string& link_name,
                    Node& node,
                    const string& other_side_link_name) {
    m_links.emplace_back(link_name, &node);
    get_resource().update_modified();
    node.m_links.emplace_back(other_side_link_name, this);
    node.get_resource().update_modified();
}

void Node::remove_link(Node& node) {
    if (this != &node) {
        auto& n_links = node.m_links;
        n_links.erase(
                std::remove_if(n_links.begin(), n_links.end(),
                [this](const Link & l) {
                    return l.m_node == this; }),
        n_links.end());
        node.get_resource().update_modified();
    }
    m_links.erase(
        std::remove_if(m_links.begin(), m_links.end(),
            [&node](const Link& l) { return l.m_node == &node; }),
        m_links.end());
    get_resource().update_modified();
}

void Node::clear_links() {
    for (const auto& link : m_links) {
        if (this != link.m_node) {
            auto& l_links = link.m_node->m_links;
            link.m_node->get_resource().update_modified();
            l_links.erase(
                    std::remove_if(l_links.begin(), l_links.end(),
                    [this](const Link & l) {
                        return l.m_node == this; }),
            l_links.end());
        }
    }
    m_links.clear();
    get_resource().update_modified();
}

Node* Node::find_if(NodePredicate predicate) const {
    if (predicate(*this)) {
        return const_cast<Node*>(this);
    }
    for (const auto& n: *this) {
        Node* found = n.find_if(predicate);
        if (nullptr != found) {
            return found;
        }
    }
    return nullptr;
}

void Node::for_each(NodeVisitor visitor) {
    visitor(*this);
    for (auto& n: *this) {
        n.for_each(visitor);
    }
}

Node* Node::find_back_if(NodePredicate predicate) const {
    const Node* node = this->m_back;
    while (nullptr != node) {
        if (predicate(*node)) {
            return const_cast<Node*>(node);
        }
        node = node->m_back;
    }
    return nullptr;
}
