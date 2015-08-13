/*!
 * @section LICENSE
 *
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
 * @section DESCRIPTION
 *
 * @file get_node_info.hpp
 *
 * @brief ...
 * */

#ifndef AGENT_IPMI_GET_NODE_INFO_HPP
#define	AGENT_IPMI_GET_NODE_INFO_HPP

#include "request.hpp"
#include "response.hpp"

#include <cstdint>
#include <stdexcept>
#include <vector>

using std::size_t;
using std::uint8_t;
using std::vector;
using std::to_string;
using std::runtime_error;

namespace agent {
namespace compute {
namespace ipmi {

namespace request {
/*!
 * @brief Represents request message for Get Node Info command.
 */
class GetNodeInfo: public Request {
public:

    /*!
     * @brief Default constructor.
     */
    GetNodeInfo();

    /*!
     * @brief Copy constrcutor.
     * @param orig object to be copied.
     */
    GetNodeInfo(const GetNodeInfo& orig);

    /*!
     * @brief Default destructor.
     */
    virtual ~GetNodeInfo();

    virtual void pack(vector<uint8_t>& data) const {
        // GetNodeInfo without request parameters.
        (void)data;
    }

private:

};
}

namespace response {

/*!
 * @brief Represents response message for Get Node Info command. Returns list of avaiable nodes.
 */
class GetNodeInfo: public Response {
public:

    /*!
     * @brief Default constructor.
     */
    GetNodeInfo();

    /*!
     * @brief Copy constrcutor.
     * @param orig object to be copied.
     */
    GetNodeInfo(const GetNodeInfo& orig);

    /*!
     * @brief Default destructor.
     */
    virtual ~GetNodeInfo();

    virtual void unpack(const vector<uint8_t>& data);

    /*!
     * @brief Checks if node is present (slot is occupied).
     * @param node_nr node number.
     * @return true if node exists, otherwise false.
     */
    bool is_present(size_t node_nr) const;

    /*!
     * @brief Gets number of available nodes.
     * @return number of available nodes.
     */
    size_t get_nodes_number() const;

private:
    uint8_t m_nodes_no{};
    vector<bool> m_nodes_present = {};

    static constexpr size_t NODE_NUMBER = 12;
    static constexpr size_t OFFSET_NODES_NUMBER  = 1;
    static constexpr size_t OFFSET_NODES_PRESENT = 2;
    static constexpr uint8_t NODE_PRESENT_MASK = 0x01;

    static constexpr size_t RESPONSE_SIZE = 14;
};
}

}
}
}

#endif	/* AGENT_IPMI_GET_NODE_INFO_HPP */

