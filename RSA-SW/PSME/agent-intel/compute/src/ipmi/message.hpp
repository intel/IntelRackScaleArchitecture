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
 * @file message.hpp
 *
 * @brief ...
 * */

#ifndef AGENT_IPMI_MESSAGE_HPP
#define AGENT_IPMI_MESSAGE_HPP

#include <cstdint>

using std::uint8_t;

namespace agent {
namespace compute {
namespace ipmi {
/*!
 * @brief Represents Message used to communicate through IPMI. It's a base class for Request and Response.
 */
class Message {
public:
    /*!
     * @brief Represents codes for Network Function tailored to IPMI spec.
     */
    enum class NetFn: uint8_t {
        CHASSIS = 0x00,
        APP = 0x06,
        INTEL = 0x38,
    };
    /*!
     * @brief Represents codes for IPMI Command.
     */
    enum class Cmd: uint8_t {
        GET_CHASSIS_STATUS = 0x01,
        GET_DEVICE_ID = 0x01,
        GET_FAN_PWM = 0x01,
        CHASSIS_CONTROLL_COMMAND = 0x02,
        SET_ISCSI_FIELD = 0x02,
        GET_ISCSI_FIELD = 0x03,
        SET_ID_FIELD = 0x06,
        GET_ID_FIELD = 0x07,
        GET_THERMAL_STATE = 0x08,
        SET_BOOT_ORDER = 0x08,
        GET_SLOT_ID = 0x09,
        GET_NODE_INFO = 0x0A,
        SET_COOLING_MODE = 0x0B,
        GET_COOLING_MODE = 0x0C,
        SET_CONFIG_COMPLETE = 0x0D,
        SET_CONFIG_PCIE_RETIMER = 0x0E,
        GET_BIOS_VERSION_INTEL = 0xE0,
        GET_DIMM_INFO_INTEL = 0xE1,
        GET_PROCESSOR_INFO_INTEL = 0xE2,
        GET_SYSTEM_GUID_INFO = 0x37,
        GET_SYSTEM_MAC_INFO = 0x19
    };

    /*!
     * @brief Default constructor.
     */
    Message(NetFn fn, Cmd cmd);

    /*!
     * @brief Copy constructor.
     * @param orig reference to object to be copied.
     */
    Message(const Message& orig);

    /*!
     * Default destructor.
     */
    virtual ~Message();

    /*!
     * Sets Network Function for command.
     *
     * @param fn Network Function code
     */
    void set_network_function(NetFn fn);

    /**
     * Gets Network Function code.
     * @return NetFn
     */
    NetFn get_network_function() const;

    /*!
     * Sets Command code.
     * @param cmd Command code.
     */
    void set_command(Cmd cmd);

    /*!
     * Gets Command code
     * @return command code.
     */
    Cmd get_command() const;

private:
    NetFn network_function {};
    Cmd command_code {};
};

}
}
}

#endif	/* AGENT_IPMI_MESSAGE_HPP */

