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
 * @file api/switch_port_info.hpp
 *
 * @brief Switch port parameters
 * */

#ifndef AGENT_NETWORK_SWITCH_PORT_INFO_HPP
#define AGENT_NETWORK_SWITCH_PORT_INFO_HPP

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <map>

using std::uint8_t;
using std::uint32_t;
using std::vector;
using std::array;
using std::pair;
using std::string;

namespace agent {
namespace network {
namespace api {

/*!
 * @brief Switch port info. Need to set switch id and port number.
 */
class SwitchPortInfo {
public:

    /*!
     * @enum State
     * @brief Switch port state
     *
     * @var State SwitchPortInfo::UP
     * Switch port state is up.
     *
     * @var State SwitchPortInfo::DOWN
     * Switch port state is down.
     *
     * @var State SwitchPortInfo::UNKNOWN
     * Switch port state is unknown.
     *
     * */
    enum class State {
        UP,
        DOWN,
        UNKNOWN,
    };

    /*!
     * @enum PortType
     * @brief Switch port type
     *
     * @var State SwitchPortInfo::ETHERNET
     * Ethernet switch port type.
     *
     * @var State SwitchPortInfo::PCIE
     * PCIe switch port type.
     *
     * */
    enum class PortType {
        ETHERNET,
        PCIE
    };

    static constexpr int STATE_NUM = 3;

    /*!
     * @enum PortAttribute
     * @brief Port attribute list.
     *
     * @var PortAttribute SwitchPortInfo::STATUS
     * Port Status.
     *
     * @var PortAttribute SwitchPortInfo::PORTIDENTIFIER
     * Port Identifier.
     *
     * @var PortAttribute SwitchPortInfo::PORTTYPE
     * Port type.
     *
     * @var PortAttribute SwitchPortInfo::LINKTECHNOLOGY
     * This is link technology, such as Ethernet, for this NIC.
     *
     * @var PortAttribute SwitchPortInfo::LINKSPEEDGBPS
     * Current port speed.
     *
     * @var PortAttribute SwitchPortInfo::MAXSPEEDGBPS
     * Max port speed.
     *
     * @var PortAttribute SwitchPortInfo::LINKSTATE
     * Port link state.
     *
     * @var PortAttribute SwitchPortInfo::OPERATIONALSTATE
     * Operational (administrative) state.
     *
     * @var PortAttribute SwitchPortInfo::PORTWIDTH
     * Port width, for PCI-e port this is number of PCI-e lanes.
     *
     * @var PortAttribute SwitchPortInfo::FRAMESIZE
     * MAC Frame size in bytes.
     *
     * @var PortAttribute SwitchPortInfo::AUTOSENSE
     * Indicates if the speed and duplex is automatically configured by the
     * port.
     *
     * @var PortAttribute SwitchPortInfo::ISMANAGEMENTPORT
     * Indicates if a port may be used for switch management.
     *
     * @var PortAttribute SwitchPortInfo::LASTERRORCODE
     * Code of last error detected.
     *
     * @var PortAttribute SwitchPortInfo::ERRORCLEARED
     * Error cleared.
     *
     * @var PortAttribute SwitchPortInfo::LASTSTATECHANGETIME
     * Time of the last port state change.
     *
     * @var PortAttribute SwitchPortInfo::MACADDRESS
     * Switch port MAC address.
     *
     * @var PortAttribute SwitchPortInfo::IPV4ADDRESS
     * IPv4 address.
     *
     * @var PortAttribute SwitchPortInfo::IPV6ADDRESS
     * IPv6 address.
     *
     * @var PortAttribute SwitchPortInfo::NEIGHBORINFO
     * Neighbor information.
     *
     * @var PortAttribute SwitchPortInfo::VLANENABLE
     * Indicates if VLANs are enabled on the switch port.
     *
     * @var PortAttribute SwitchPortInfo::VLANLIST
     * List of created VLANs.
     *
     * */
    enum PortAttribute {
        STATUS,
        PORTIDENTIFIER,
        PORTTYPE,
        LINKTECHNOLOGY,
        LINKSPEEDGBPS,
        MAXSPEEDGBPS,
        LINKSTATE,
        OPERATIONALSTATE,
        PORTWIDTH,
        FRAMESIZE,
        AUTOSENSE,
        ISMANAGEMENTPORT,
        LASTERRORCODE,
        ERRORCLEARED,
        LASTSTATECHANGETIME,
        MACADDRESS,
        IPV4ADDRESS,
        IPV6ADDRESS,
        NEIGHBORINFO,
        VLANENABLE,
        VLANLIST
    };

    /*! Port attribute value class */
    class PortAttributeValue {
    public:
        /*!
         * @brief Default constructor.
         */
        PortAttributeValue(): m_number(0), m_bool(false), m_string{} {}

        /*!
         * @brief Default destructor.
         */
        ~PortAttributeValue(){}

        /*!
         * @brief Get bool value
         * @return Bool value.
         */
        bool get_bool() const { return m_bool; }

        /*!
         * @brief Get number value
         * @return Number value.
         */
        uint32_t get_number() const { return m_number; }

        /*!
         * @brief Get string value
         * @return String value.
         */
        std::string get_string() const { return m_string; }

        /*!
         * @brief Set bool value
         * @param[in] value Value to set.
         */
        void set(bool value) { m_bool = value; }

        /*!
         * @brief Set number value
         * @param[in] value Value to set.
         */
        void set(uint32_t value) { m_number = value; }

        /*!
         * @brief Set string value
         * @param[in] value Value to set.
         */
        void set(const std::string& value) { m_string = value; }

    private:
        uint32_t m_number;
        bool m_bool;
        std::string m_string;
    };

    using VlanList = vector<string>;

    /*!
     * @brief Default constructor.
     */
    SwitchPortInfo() = default;

    /*!
     * @brief Default constructor.
     * @param[in] switch_id Switch index.
     * @param[in] port_index port index.
     */
    SwitchPortInfo(uint8_t switch_id, uint8_t port_index);

    /*!
     * @brief Default destructor.
     */
    virtual ~SwitchPortInfo();

    /*!
     * @brief Gets Port present flag
     * @return present flag.
     */
    bool get_is_present() const {
        return m_is_present;
    }

    /*!
     * @brief Sets Port present flag
     * @param is_present Port present plag.
     */
    void set_is_present(const bool is_present) {
        m_is_present = is_present;
    }

    /*!
     * @brief Gets switch id
     * @return Switch id.
     */
    uint8_t get_switch_id() const {
      return m_switch_id;
    }

    /*!
     * @brief Sets switch id
     * @param switch_id Switch id.
     */
    void set_switch_id(const uint8_t switch_id) {
        m_switch_id = switch_id;
    }

    /*!
     * @brief Gets port index
     * @return Switch port index.
     */
    uint8_t get_index() const {
      return m_index;
    }

    /*!
     * @brief Sets port index
     * @param index Port index.
     */
    void set_index(const uint8_t index) {
        m_index = index;
    }

    /*!
     * @brief Gets switch port link state.
     * @return Switch port link state.
     */
    string get_link_state() const {
        return state_get_string(m_link_state);
    }

    /*!
     * @brief Gets switch port link state.
     * @return Switch port link state.
     */
    State get_link_state_enum() const {
      return m_link_state;
    }

    /*!
     * @brief Sets switch port link state.
     * @param link_state Port link state.
     */
    void set_link_state(const string& link_state) {
        m_link_state = state_get_enum(link_state);
    }

    /*!
     * @brief Sets switch port link state using enum value.
     * @param link_state Port link state.
     */
    void set_link_state(State link_state) {
        m_link_state = link_state;
    }

    /*!
     * @brief Gets switch port operational state.
     * @return Switch port operational state.
     */
    string get_operational_state() const {
        return state_get_string(m_operational_state);
    }

    /*!
     * @brief Sets switch port operational state.
     * @param operational_state Port operational state.
     */
    void set_operational_state(const string& operational_state) {
        m_operational_state = state_get_enum(operational_state);
    }

    /*!
     * @brief Sets switch port operational state.
     * @param operational_state Port operational state.
     */
    void set_operational_state(State operational_state) {
        m_operational_state = operational_state;
    }

    /*!
     * @brief Gets switch port link speed.
     * @return Switch port link speed.
     */
    uint32_t get_link_speed() const {
      return m_link_speed;
    }

    /*!
     * @brief Sets switch port link speed
     * @param link_speed Port link speed.
     */
    void set_link_speed(uint32_t link_speed) {
        m_link_speed = link_speed;
    }

    /*!
     * @brief Gets switch port max frame size.
     * @return Switch port max frame size.
     */
    uint32_t get_max_frame_size() const {
      return m_max_frame_size;
    }

    /*!
     * @brief Sets switch port max frame size
     * @param max_frame_size Port max frame size.
     */
    void set_max_frame_size(uint32_t max_frame_size) {
        m_max_frame_size = max_frame_size;
    }

    /*!
     * @brief Gets switch port identifier.
     * @return Switch port identifier.
     */
    string get_identifier() const {
      return m_identifier;
    }

    /*!
     * @brief Sets switch port identifier
     * @param identifier Port identifier.
     */
    void set_identifier(string identifier) {
        m_identifier = identifier;
    }

    /*!
     * @brief Get switch port type.
     * @return Switch port type.
     */
    PortType get_type() const {
        return m_type;
    }

    /*!
     * @brief Set switch port type.
     * @param type Port type.
     */
    void set_type(PortType type) {
        m_type = type;
    }

    /*!
     * @brief Get switch port type as string.
     * @return Switch port type as a string.
     */
    const std::string& get_type_str() const;

    /*!
     * @brief Set switch port type.
     * @param type Port type as string.
     */
    void set_type(const string& type);

     /*!
     * @brief Gets Port index (Port number).
     * @return port_index Port index.
     */
    uint32_t get_port_index() const {
        return m_index;
    }

    /*!
     * @brief Checks if the link state is valid.
     * @return true if the link state is valid, false otherwise.
     */
    bool link_state_is_valid() const {
        return State::UNKNOWN != get_link_state_enum();
    }

    /*!
     * @brief Get vlan list on the port.
     * @return Vlan list.
     */
    const VlanList& get_vlan_list() const {
      return m_vlan_list;
    }

    /*!
     * @brief Set vlan list on the port.
     * @param[in] vlan_list Vlan list.
     */
    void set_vlan_list(const VlanList& vlan_list) {
        m_vlan_list = vlan_list;
    }

private:
    string state_get_string(State state) const {
        for (const auto& elem : m_state2string) {
            if (elem.first == state) {
                return elem.second;
            }
        }
        throw std::runtime_error("cannot convert enum state to string");
    }

    State state_get_enum(const string& str) const {
        for (const auto& elem : m_state2string) {
            if (elem.second == str) {
                return elem.first;
            }
        }
        throw std::runtime_error("cannot convert string state to enum");
    }

    bool m_is_present{false};
    uint8_t m_switch_id{0};
    uint8_t m_index{0};

    VlanList m_vlan_list{};
    State m_link_state{State::UNKNOWN};
    State m_operational_state{State::UNKNOWN};
    uint32_t m_link_speed{0};
    uint32_t m_max_frame_size{0};
    string m_identifier{"Unknown"};
    PortType m_type{PortType::ETHERNET};

    static constexpr char STR_UP[]      = "Up";
    static constexpr char STR_DOWN[]    = "Down";
    static constexpr char STR_UNKNOWN[] = "Unknown";

    using state2string_t = array<pair<State, const char*>, STATE_NUM>;
    static const state2string_t m_state2string;

    /*!
     * Switch port type map.
     * */
    std::map<std::string, PortType> m_port_type_map {
        {"Ethernet", PortType::ETHERNET},
        {"PCIe",     PortType::PCIE}
    };
};

}
}
}
#endif /* AGENT_NETWORK_SWITCH_PORT_INFO_HPP */
