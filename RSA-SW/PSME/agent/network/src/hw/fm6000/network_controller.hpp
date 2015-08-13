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
 * @file hw/fm6000/network_controller.hpp
 *
 * @brief Implementation of network interface
 * */

#ifndef AGENT_NETWORK_HW_FM6000_NETWORK_CONTROLLER_HPP
#define AGENT_NETWORK_HW_FM6000_NETWORK_CONTROLLER_HPP

#include "network_config.hpp"
#include "logger/logger_factory.hpp"
#include "hw/network_controller.hpp"

#ifdef IES_FOUND
#if __cplusplus >= 201103L
extern "C"{
#endif
#include <fm_sdk.h>
#include <platform_api.h>
#if __cplusplus >= 201103L
}
#endif

#include <cstring>
#include <vector>
#include <string>
#include <map>


using std::string;
using std::to_string;
using std::runtime_error;
using std::map;

namespace agent {
namespace network {
namespace hw {
namespace fm6000 {

/*! Network Controller for fm6000 */
class NetworkController: public hw::NetworkController {
    fm_int m_switch_id = 0;
    bool m_is_initialized = false;
    /* Because of GCC compilation issue, we need to initialize all fields of the
     * structure */
    fm_switchInfo m_switch_info{0,0,0,0,{},{},{},{},0,0,0,0,0,0,0,0,0,0,0,0};
public:

    /*!
     * @brief Default constructir
     * */
    NetworkController() {
    }

    void initialize();

    /*!
     * @brief Gets initialization flag
     * @return Initialization flag. Is set to true if switch is initialized.
     * */
    bool get_is_initialized() {
        return m_is_initialized;
    }

    /*!
     * @brief Gets number of ports of fm6000
     * @return Number of ports.
     * */
    fm_int get_ports_count() const;

    /*!
     * @brief Get list port ports.
     * @return Port list.
     */
    const std::vector<std::string> get_port_list() const;

    /*!
     * @brief Deinitializes switch
     * */
    void deinitialize();

    ~NetworkController();

private:
    static void m_switch_event_handler(fm_int event, fm_int switch_id, void *ptr);

    void m_initialize_switch();

    void m_initialize_ports();

    void m_initialize_vlans();

    void m_initialize_spanning_tree();

    void m_set_ports_state();

    void m_read_switch_info();

    void m_add_tagged_port_to_vlan(uint16_t vlan_id, fm_int port_id);

    void m_add_untagged_port_to_vlan(uint16_t vlan_id, fm_int port_id);

    void m_log_error_and_throw(fm_status retval, const std::string& msg);

    /*!
     * List of autonegotiation modes supported by fm6000 (Alta) switch.
     * We do not support sharing a MAC between ports (P0 ports) in Clause 73 mode.
     * */
    std::map<std::string, fm_uint64> m_port_autoneg_map {
        { "default",    FM_PORT_AUTONEG_NONE },
        { "sgmii",      FM_PORT_AUTONEG_SGMII },
        { "clause_37",  FM_PORT_AUTONEG_CLAUSE_37 },
        { "clause_73",  FM_PORT_AUTONEG_CLAUSE_73 }
    };

    /*!
     * Autonegotiation basepages values are defined by FocalPoint API.
     * In case of Clause 73 we support only "non-P0" ports. See FocalPoint API
     * documetation for more details, section 5.12 Port Attributes, rev. 4.1.1
     * */
    std::map<fm_uint64, fm_uint64> m_port_basepage_map {
        { FM_PORT_AUTONEG_SGMII,        0x00004001 },
        { FM_PORT_AUTONEG_CLAUSE_37,    0x000040a0 },
        { FM_PORT_AUTONEG_CLAUSE_73,    0x00a04001 }
    };

    std::map<std::string, fm_uint64> m_port_eth_mode_map {
        { "default",      0 },
        { "1000BASE_KX",  FM_ETH_MODE_1000BASE_KX },
        { "1000BASE_X",   FM_ETH_MODE_1000BASE_X },
        { "10GBASE_CR",   FM_ETH_MODE_10GBASE_CR },
        { "10GBASE_CX4",  FM_ETH_MODE_10GBASE_CX4 },
        { "10GBASE_KR",   FM_ETH_MODE_10GBASE_KR },
        { "10GBASE_KX4",  FM_ETH_MODE_10GBASE_KX4 },
        { "10GBASE_SR",   FM_ETH_MODE_10GBASE_SR },
        { "24GBASE_CR4",  FM_ETH_MODE_24GBASE_CR4 },
        { "24GBASE_KR4",  FM_ETH_MODE_24GBASE_KR4 },
        { "2500BASE_X",   FM_ETH_MODE_2500BASE_X },
        { "40GBASE_CR4",  FM_ETH_MODE_40GBASE_CR4 },
        { "40GBASE_KR4",  FM_ETH_MODE_40GBASE_KR4 },
        { "40GBASE_SR4",  FM_ETH_MODE_40GBASE_SR4 },
        { "6GBASE_CR",    FM_ETH_MODE_6GBASE_CR },
        { "6GBASE_KR",    FM_ETH_MODE_6GBASE_KR },
        { "AN_73",        FM_ETH_MODE_AN_73 },
        { "DISABLED",     FM_ETH_MODE_DISABLED },
        { "SGMII",        FM_ETH_MODE_SGMII },
        { "XAUI",         FM_ETH_MODE_XAUI },
        { "XLAUI",        FM_ETH_MODE_XLAUI }
    };

};

}
}
}
}
#endif

#endif /* AGENT_NETWORK_HW_FM6000_NETWORK_CONTROLLER_HPP */
