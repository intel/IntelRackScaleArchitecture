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
 * @file hw/network_controller.hpp
 *
 * @brief Interface for Network Controller. Implement this interface to
 * handle connections and sending/receiving commands.
 * */

#ifndef AGENT_NETWORK_NETWORK_CONTROLLER_HPP
#define AGENT_NETWORK_NETWORK_CONTROLLER_HPP

#include <memory>
#include <map>

namespace agent {
namespace network {
namespace hw {

 /*!
 * @brief NetworkController interafce.
 * */
class NetworkController {
public:
    using UUID = std::string;
    using SwitchId = uint32_t;
    using SwitchPortId = uint32_t;

    NetworkController();
    virtual ~NetworkController();

    /*!
     * @brief Get switch UUID by switch id.
     * @param[in] switch_id Switch Id.
     * @return Switch UUID.
     */
    const std::string& get_switch_uuid(SwitchId switch_id) const;

    /*!
     * @brief Get switch id by switch UUID.
     * @param[in] switch_uuid Switch UUID.
     * @return Switch id.
     */
    SwitchId get_switch_id(UUID switch_uuid) const;

    /*!
     * @brief Get switch port type.
     * @param[in] port_id Switch port type.
     * @return Port id.
     */
    const std::string get_switch_port_type(SwitchPortId port_id) const;

    /* Default switch id */
    static constexpr SwitchId DEFAULT_SWITCH_ID = 0;

    /* Default switch port type */
    static const constexpr char* UNKNOWN_SWITCH_PORT_TYPE = "Unknown";

private:
    std::map<UUID, SwitchId> m_switch_map{};
    std::map<SwitchPortId, std::string> m_switch_port_type_map{};
};

}
}
}
#endif	/* AGENT_NETWORK_NETWORK_CONTROLLER_HPP */
