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

#ifndef AGENT_NETWORK_DCRP_REMOTE_SWITCH_INFO_HPP
#define AGENT_NETWORK_DCRP_REMOTE_SWITCH_INFO_HPP

#include "api/remote_switch_info.hpp"

#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace agent {
namespace network {
namespace api {
namespace dcrp {

/*!
 * @brief dcrp switch Info.
 */
class RemoteSwitchInfo : public api::RemoteSwitchInfo {
public:
    /*!
     * @brief Default constructor.
     */
    RemoteSwitchInfo() = delete;

    /*!
     * @brief Default constructor.
     * @param[in] uuid Switch UUID.
     */
    RemoteSwitchInfo(const std::string& uuid);

    /*!
     * @brief Copy constructor.
     * @param orig object to be copied.
     */
    RemoteSwitchInfo(const RemoteSwitchInfo& orig) = delete;

    /*!
     * @brief Copy operator.
     * @param orig object to be copied.
     */
    RemoteSwitchInfo& operator=(const RemoteSwitchInfo& orig) = delete;

    /*!
     * @brief Destructor.
     */
    virtual ~RemoteSwitchInfo();

    /*!
     * @brief Gets known switches ids
     * @param[out] switches_id Vector of switches id to be filled.
     */
    void get_known_switches_id(vector<string>& switches_id);

    /*!
     * @brief Gets neighbor switches ids
     * @param[in] id Switch id
     * @param[out] switches_id Vector of neighbor switches id to be filled.
     */
    void get_neighbor_switches_id(const string& id, vector<string>& switches_id);

    /*!
     * @brief Gets switch info.
     * @param[in] id Id of the switch.
     * @param[out] response GetRemoteSwitchInfo response object
     */
    void get_switch_info(const string& id, Response& response);
private:
    /*!
     * @brief helper functions.
     */
    class ScopedSocket {
    public:
        /*!
         * @brief constructor.
         */
        ScopedSocket(int sock) : m_sock(sock) {}

        /*!
         * @brief Copy constructor.
         * @param orig object to be copied.
         */
        ScopedSocket(const ScopedSocket& orig) = delete;

        /*!
         * @brief Copy operator.
         * @param orig object to be copied.
         */
        ScopedSocket& operator=(const ScopedSocket& orig) = delete;

        /*!
         * @brief Destructor.
         */
        virtual ~ScopedSocket();

    private:
        int m_sock;
    };

    using Pointer = std::unique_ptr<char[]>;

    /*!
     * @brief Send uint32_t value to the socket.
     * @param sock socket to use.
     * @param value value to send.
     */
    static void writeInt(int sock, uint32_t value);

    /*!
     * @brief Read uint32_t value from the socket.
     * @param sock socket to use.
     * @param value value to read into.
     */
    static void readInt(int sock, uint32_t &value);

    /*!
     * @brief Send command and get response.
     * @param cmd Command to be sent.
     * @param buf returned result.
     * @param outsize the size of the result in bytes.
     */
    static uint32_t processCommand(const string& cmd, Pointer& buf,
                                   uint32_t& outsize);
};

}
}
}
}
#endif /* AGENT_NETWORK_DCRP_REMOTE_SWITCH_INFO_HPP */
