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
 * @file api/ies/switch_port_info.hpp
 *
 * @brief Switch port parameters
 * */

#ifndef AGENT_NETWORK_IES_SWITCH_PORT_INFO_HPP
#define AGENT_NETWORK_IES_SWITCH_PORT_INFO_HPP

#include "network_config.hpp"
#include "api/switch_port_info.hpp"

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
 * @brief Switch Port Info.
 */
class SwitchPortInfo : public api::SwitchPortInfo {
public:
    /*!
     * @enum SpeedCapability
     * @brief Port speed capability list.
     *
     * @var SpeedCapability SwitchPortInfo::LINKSPEED10M
     * 10M speed possible to set on the switch port.
     *
     * @var SpeedCapability SwitchPortInfo::LINKSPEED100M
     * 100M speed possible to set on the switch port.
     *
     * @var SpeedCapability SwitchPortInfo::LINKSPEED1G
     * 1G speed possible to set on the switch port.
     *
     * @var SpeedCapability SwitchPortInfo::LINKSPEED2PT5G
     * 2.5G speed possible to set on the switch port.
     *
     * @var SpeedCapability SwitchPortInfo::LINKSPEED5G
     * 5G speed possible to set on the switch port.
     *
     * @var SpeedCapability SwitchPortInfo::LINKSPEED10G
     * 10G speed possible to set on the switch port.
     *
     * @var SpeedCapability SwitchPortInfo::LINKSPEED20G
     * 20G speed possible to set on the switch port.
     *
     * @var SpeedCapability SwitchPortInfo::LINKSPEED40G
     * 40G speed possible to set on the switch port.
     *
     * */
    enum SpeedCapability {
        LINKSPEED10M   = 10,
        LINKSPEED100M  = 100,
        LINKSPEED1G    = 1000,
        LINKSPEED2PT5G = 2500,
        LINKSPEED5G    = 5000,
        LINKSPEED10G   = 10000,
        LINKSPEED20G   = 20000,
        LINKSPEED40G   = 40000
    };

    /*!
     * @brief Default constructor.
     * @param[in] uuid Switch UUID.
     * @param[in] port_identifier Port identifier.
     */

    SwitchPortInfo(const std::string& uuid, const std::string& port_identifier);

    /*!
     * @brief Copy constructor.
     * @param[in] orig object to be copied.
     */
    SwitchPortInfo(const SwitchPortInfo& orig) = delete;

    /*!
     * @brief Copy operator
     * @param[in] orig object to be copied.
     */
    SwitchPortInfo& operator=(const SwitchPortInfo &orig) = delete;

    /*!
     * @brief Default destructor.
     */
    virtual ~SwitchPortInfo();

    /*!
     * @brief Checks switch port speed configurability
     */
    void check_switch_port_speed_configurability();

    /*!
     * @brief Checks switch port speed capabilities
     * @param[in] speed Speed port attribute.
     */
    void check_switch_port_speed_capabilities(fm_uint32 speed);

    /*!
     * @brief Gets port attribute from a switch.
     * @param[in] attr Port attribute to get.
     */
    void get_switch_port_attribute(PortAttribute attr);

    /*!
     * @brief Sets port attribute on a switch.
     * @param[in] attr Port attribute to set.
     */
    void set_switch_port_attribute(PortAttribute attr);

    /*!
     * @brief Gets port present flag
     * @return present flag.
     */
    bool get_switch_port_is_present(fm_int port_index);

    /*!
     * @brief Reads switch port state
     * @param[in] switch_id Switch id.
     * @param[in] port_index Port index.
     * @param[in] mode_ptr Pointer to mode parameter.
     * @param[in] state_ptr Pointer to state parameter.
     * @param[in] info_ptr Pointer to info parameter.
     */
    static void read_switch_port_state(
        fm_int switch_id, fm_int port_index, fm_int* mode_ptr, fm_int* state_ptr, fm_int* info_ptr);

    /*!
     * @brief Writes switch port state
     * @param[in] switch_id Switch id.
     * @param[in] port_index Port index.
     * @param[in] mode Mode parameter.
     */
    static void write_switch_port_state(
        fm_int switch_id, fm_int port_index, fm_int mode);

    /*!
     * @brief Writes switch port state
     * @param[in] switch_id Switch id.
     * @param[in] port_index Port index.
     * @param[in] speed Speed parameter.
     */
    static void write_switch_port_speed(
        fm_int switch_id, fm_int port_index, fm_uint32 speed);

    /*!
     * @brief Writes switch port max frame size
     * @param[in] switch_id Switch id.
     * @param[in] port_index Port index.
     * @param[in] max_frame_size Max frame size parameter.
     */
    static void write_switch_port_max_frame_size(
        fm_int switch_id, fm_int port_index, fm_int max_frame_size);

    /*!
     * @brief Reads switch port capabilities
     * @param[in] switch_id Switch id.
     * @param[in] port_index Port index.
     * @param[in] cap_value_ptr Pointer to port capabilities.
     */
    static void read_switch_port_capabilities(
        fm_int switch_id, fm_int port_index, fm_uint32* cap_value_ptr);

    /*!
     * @brief Reads switch port attribute
     * @param[in] switch_id Switch id.
     * @param[in] port_index Port index.
     * @param[in] attr Port attribute.
     * @param[in] attr_value_ptr Pointer to port attribute.
     */
    static void read_switch_port_attribute(
        fm_int switch_id, fm_int port_index, fm_int attr, fm_uint64* attr_value_ptr);

    /*!
     * @brief Writes switch port attribute
     * @param[in] switch_id Switch id.
     * @param[in] port_index Port index.
     * @param[in] mac Port mac parameter.
     * @param[in] line Port line parameter.
     * @param[in] attr Port attribute.
     * @param[in] attr_value_ptr Pointer to port attribute.
     */
    static void write_switch_port_attribute(
        fm_int switch_id, fm_int port_index, fm_int mac, fm_int line, fm_int attr, fm_uint64* attr_value_ptr);

private:
    fm_int m_mode{0};
    fm_int m_state{0};
    fm_int m_info{0};
    fm_uint64 m_attr_value{0};

    static constexpr int IES_PORT_MODE_NUM = 3;
    using iesmode2state_t = array<pair<const fm_int, State>, IES_PORT_MODE_NUM>;
    static const iesmode2state_t m_iesmode2state;

    fm_int mode_to_ies(State state) const;
    State mode_to_enum(const fm_int iesmode) const;


    static constexpr int IES_PORT_STATE_NUM = 4;
    using iesstate2state_t = array<pair<const fm_int, State>, IES_PORT_STATE_NUM>;
    static const iesstate2state_t m_iesstate2state;

    fm_int state_to_ies(State state) const;
    State state_to_enum(const fm_int iesstate) const;


    static constexpr int IES_PORT_ATTR_NUM = 2;
    using iesattr2attr_t = array<pair<const fm_int, PortAttribute>, IES_PORT_ATTR_NUM>;
    static const iesattr2attr_t m_iesattr2attr;

    fm_int attr_to_ies(PortAttribute attr) const;
    PortAttribute attr_to_enum(const fm_int iesattr) const;

    static constexpr int IES_PORT_CAP_NUM = 8;
    using iescap2speed_t = array<pair<const fm_uint32, SpeedCapability>, IES_PORT_CAP_NUM>;
    static const iescap2speed_t m_iescap2speed;

    fm_uint32 speed_to_iescap(fm_uint32 speed) const;
};

}
}
}
}
#endif /* IES_FOUND */
#endif /* AGENT_NETWORK_IES_SWITCH_PORT_INFO_HPP */
