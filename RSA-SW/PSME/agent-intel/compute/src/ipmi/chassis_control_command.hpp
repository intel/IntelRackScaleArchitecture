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
 * @file chassis_control_command.hpp
 *
 * @brief IPMI Chassis Control Command.
 * */

#ifndef AGENT_IPMI_CHASSIS_CONTROL_COMMAND_HPP
#define	AGENT_IPMI_CHASSIS_CONTROL_COMMAND_HPP

#include "request.hpp"
#include "response.hpp"

#include <map>

namespace agent {
namespace compute {
namespace ipmi {

namespace request {
/*!
 * @brief Request message for IPMI Chassis Control Command. It's needed to set PowerStatus you expected to enforce.
 * Default behavior is to power down chassis/platform.
 */
class ChassisControlCommand: public Request {
public:

    /*!
     * @brief Represents Power Status to be set.
     */
    enum class PowerState: uint8_t {
        POWER_DOWN = 0x00,
        POWER_UP,
        POWER_CYCLE,
        HARD_RESET,
        DIAGNOSTIC_INTERUPT,
        ACPI_SOFT_SHUTDOWN,
        LAST,
        UNKNOWN
    };

    /*!
     * @brief Default constructor.
     */
    ChassisControlCommand();

    /*! Copy constructor. */
    ChassisControlCommand(const ChassisControlCommand&) = default;

    /*! Assginment operator */
    ChassisControlCommand& operator=(const ChassisControlCommand&) = default;

    /*!
     * @brief Default destructor.
     */
    virtual ~ChassisControlCommand();

    /*!
     * @brief Sets requested power status to be send to MC.
     * @param status new power status to enforce on chassis.
     */
    void set_power_state(PowerState status);

    /*!
     * @brief Sets requested power status to be send to MC.
     * @param power_state new power status to enforce on chassis. String
     * contains text compatible wih GAMI spec.
     */
    void set_power_state(const std::string& power_state);

    virtual void pack(vector<uint8_t>& data) const;

private:
    PowerState m_power_state = PowerState::POWER_DOWN;

    static const std::map<std::string, PowerState> m_map;
};

}

namespace response {
/*!
 * @brief Request message for IPMI Chassis Control Command.
 */
class ChassisControlCommand: public Response {
public:
    /*!
     * @brief Default constructor.
     */
    ChassisControlCommand();

    /*! Copy constructor. */
    ChassisControlCommand(const ChassisControlCommand&) = default;

    /*! Assginment operator */
    ChassisControlCommand& operator=(const ChassisControlCommand&) = default;

    /*!
     * @brief Default destructor.
     */
    virtual ~ChassisControlCommand();

    virtual void unpack(const vector<uint8_t>& data);

private:
    static constexpr size_t RESPONSE_SIZE = 1;
};

}

}
}
}

#endif	/* AGENT_IPMI_CHASSIS_CONTROL_COMMAND_HPP */

