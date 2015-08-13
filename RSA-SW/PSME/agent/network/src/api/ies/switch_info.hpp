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
 * @file api/ies/switch_info.hpp
 *
 * @brief Switch parameters
 * */

#ifndef AGENT_NETWORK_IES_GET_SWITCH_INFO_HPP
#define	AGENT_NETWORK_IES_GET_SWITCH_INFO_HPP

#include "network_config.hpp"
#include "api/switch_info.hpp"

#ifdef IES_FOUND
#if __cplusplus >= 201103L
extern "C"{
#endif
#include <fm_sdk.h>
#include <platform_api.h>
#if __cplusplus >= 201103L
}
#endif

using std::uint8_t;
using std::uint32_t;
using std::string;
using std::to_string;
using std::runtime_error;

namespace agent {
namespace network {
namespace api {
namespace ies {

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
#endif /* IES_FOUND */
#endif /* AGENT_NETWORK_IES_SWITCH_INFO_HPP */
