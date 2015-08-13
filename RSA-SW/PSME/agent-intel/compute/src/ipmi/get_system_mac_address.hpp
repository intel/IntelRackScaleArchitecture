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
 * @file get_system_mac_address.hpp
 *
 * @brief Implementation of GetSystemMacAddress Request/Response.
 * */

#ifndef AGENT_IPMI_GET_SYSTEM_MAC_ADDRESS_HPP
#define	AGENT_IPMI_GET_SYSTEM_MAC_ADDRESS_HPP

#include "request.hpp"
#include "response.hpp"
#include "agent-framework/logger_ext.hpp"

#include <string>
#include <map>
#include <vector>

using std::size_t;
using std::uint8_t;
using std::string;
using std::to_string;
using std::vector;
using std::map;

namespace agent {
namespace compute {
namespace ipmi {

namespace request {
/*!
 * @brief Represents request message for Get command.
 */
class GetSystemMacAddress: public Request {
public:

    /*!
     * @brief Default constructor.
     */
    GetSystemMacAddress();

    /*!
     * @brief Copy constructor
     * @param orig object to copy.
     */
    GetSystemMacAddress(const GetSystemMacAddress& orig);

    /*!
     * @brief Destructor.
     */
    virtual ~GetSystemMacAddress();

    /**
     * @brief Packs data from object to output vector
     * @param[out] data vector where data will be packed.
     */
    virtual void pack(vector<uint8_t>& data) const;

    /*!
     * @brief Sets ethernet interafce index.
     * @param index etherenet interafce index.
     */
    void set_interface_id(uint16_t index) {
        m_interface_id = index;
    }

private:
    uint16_t m_interface_id{};
};
}

namespace response {

/*!
 * @brief Represents response message for GetDeviceId command.
 */
class GetSystemMacAddress: public Response {
public:

    /**
     * @brief Default constructor.
     */
    GetSystemMacAddress();

    /*!
     * @brief Copy constructor
     * @param orig object to copy.
     */
    GetSystemMacAddress(const GetSystemMacAddress& orig);

    /*!
     * @brief Destructor.
     */
    virtual ~GetSystemMacAddress();

    std::string get_system_mac_address() {
        return m_system_mac_address;
    }

    /*!
     * @brief Unpacks data from vector to object.
     * @param data bytes to be copied to object,
     */
    virtual void unpack(const vector<uint8_t>& data);

private:
    std::string m_system_mac_address{};

    static constexpr size_t RESPONSE_SIZE = 17;
    static constexpr size_t MAC_STR_SIZE = 18;
    static constexpr size_t MAC_ADDRESS_OFFSET = 3;
};
}


}
}
}

#endif	/* AGENT_IPMI_GET_SYSTEM_MAC_ADDRESS_HPP */

