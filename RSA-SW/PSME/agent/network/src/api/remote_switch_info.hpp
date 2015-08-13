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
 */

#ifndef AGENT_NETWORK_REMOTE_SWITCH_INFO_HPP
#define AGENT_NETWORK_REMOTE_SWITCH_INFO_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "agent-framework/command/network/get_remote_switch_info.hpp"

using std::uint32_t;
using std::string;
using std::vector;

using agent_framework::command::network::GetRemoteSwitchInfo;

namespace agent {
namespace network {
namespace api {

/*!
 * @brief Remote switch info. Need to set switch id and port number.
 */
class RemoteSwitchInfo {
public:
    using Hop = GetRemoteSwitchInfo::Response::Hop;

    /*! GetRemoteSwitchInfo response */
    class Response {
    public:
        /*!
         * @brief Set mac address from request
         * @param mac_address switch mac address
         * */
        void set_mac_address(const std::string& mac_address) {
            m_mac_address = mac_address;
        }

        /*!
         * @brief Get MAC address
         * @return MAC address
         * */
        const string& get_mac_address() const {
            return m_mac_address;
        }

        /*!
         * @brief Add next hop
         * @param next_hop Next hop
         * */
        void add_next_hop(const Hop& next_hop) {
            m_next_hops.push_back(next_hop);
        }

        /*!
         * @brief Get next hops
         * @return Next hop
         * */
        const vector<Hop>& get_next_hops() const {
            return m_next_hops;
        }

    private:
        string m_mac_address{};
        vector<Hop> m_next_hops{};
    };

    /*!
     * @brief Default constructor.
     */
    RemoteSwitchInfo();

    /*!
     * @brief Default destructor.
     */
    virtual ~RemoteSwitchInfo();

    /*!
     * @brief Gets known switches ids.
     * @param[in] switches_id Vector of switches id to be filled.
     */
    virtual void get_known_switches_id(vector<string>& switches_id) = 0;

    /*!
     * @brief Gets neighbor switches ids.
     * @param[in] id Switch id
     * @param[in] switches_id Vector of neighbor switches id to be filled.
     */
    virtual void get_neighbor_switches_id(const string& id,
                                          vector<string>& switches_id) = 0;

    /*!
     * @brief Gets switch info.
     * @param id Id of the remote switch.
     * @param response GetRemoteSwitchInfo response object
     */
    virtual void get_switch_info(const string& id, Response& response) = 0;
};

}
}
}
#endif /* AGENT_NETWORK_REMOTE_SWITCH_INFO_HPP */
