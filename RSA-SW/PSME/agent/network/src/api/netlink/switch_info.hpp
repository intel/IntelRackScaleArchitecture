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
 * @file api/netlink/switch_info.hpp
 *
 * @brief Switch parameters
 * */

#ifndef AGENT_NETWORK_NETLINK_GET_SWITCH_INFO_HPP
#define	AGENT_NETWORK_NETLINK_GET_SWITCH_INFO_HPP

#include "api/switch_info.hpp"

namespace agent {
namespace network {
namespace api {
namespace netlink {

/*!
 * @brief Switch Info.
 */
class SwitchInfo : public api::SwitchInfo {
public:
    /*!
     * @brief Default constructor.
     * @param[in] uuid Switch UUID.
     */
    SwitchInfo(const std::string& uuid);

    /*!
     * @brief Default constructor.
     * @param[in] switch_id Switch Id.
     */
    SwitchInfo(uint16_t switch_id);

    /*!
     * @brief Copy constructor.
     * @param[in] orig object to be copied.
     */
    SwitchInfo(const SwitchInfo& orig) = delete;

    /*!
     * @brief Copy operator
     * @param[in] orig object to be copied.
     */
    SwitchInfo& operator=(const SwitchInfo &orig) = delete;

    /*!
     * @brief Default destructor.
     */
    virtual ~SwitchInfo();

    /*!
     * @brief Reads information about switch from the HW.
     */
    void read_switch_info();

    /*!
     * @brief Read switch port from the HW.
     */
    void read_switch_port_list();

};

}
}
}
}
#endif /* AGENT_NETWORK_NETLINK_SWITCH_INFO_HPP */
