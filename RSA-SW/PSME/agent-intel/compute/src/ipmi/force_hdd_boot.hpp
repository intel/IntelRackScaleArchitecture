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
 * @file force_hdd_boot.hpp
 *
 * @brief Force local HDD boot command declaration.
 * */

#ifndef AGENT_COMPUTE_IPMI_FORCE_HDD_BOOT_HPP
#define AGENT_COMPUTE_IPMI_FORCE_HDD_BOOT_HPP

#include "set_system_boot_options.hpp"

namespace agent {
namespace compute {
namespace ipmi {

namespace request {

/*!
 * @brief Request message for IPMI Force PXE Boot.
 */
class ForceHddBoot: public request::SetSystemBootOptions {
public:
    /*!
     * @brief Default constructor.
     */
    ForceHddBoot();

    /*! Copy constructor */
    ForceHddBoot(const ForceHddBoot&) = default;

    /*! Assginment operator */
    ForceHddBoot& operator=(const ForceHddBoot&) = default;


    /*!
     * @brief Default destructor.
     */
    virtual ~ForceHddBoot();

    virtual void pack(vector<uint8_t>& data) const;

private:
    static constexpr std::uint8_t OPTION_BOOT_FLAGS_VALID = 0x80;
    static constexpr std::uint8_t OPTION_BOOT_DEVICE_FORCE_HDD = 0x08;
    static constexpr std::uint8_t OPTION_SYSTEM_DEFAULT = 0x00;
    static constexpr std::uint8_t OPTION_BIOS_DEFAULT = 0x00;
    static constexpr std::uint8_t OPTION_BOOT_DEVICE_INSTANCE_FORCE_HDD = 0x02;
};

}

namespace response {

/*!
 * @brief Response message for IPMI Force HDD Boot.
 */
class ForceHddBoot: public response::SetSystemBootOptions  {
public:

    /*!
     * @brief Default constructor.
     */
    ForceHddBoot();

    /*!
     * @brief Copy constructor.
     * @param orig object to be copied.
     */
    ForceHddBoot(const ForceHddBoot& orig);

    /*!
     * @brief Default destructor.
     */
    virtual ~ForceHddBoot();

    virtual void unpack(const vector<uint8_t>& data);

private:
    static constexpr size_t RESPONSE_SIZE = 1;
};

}

}
}
}
#endif	/* AGENT_COMPUTE_IPMI_FORCE_HDD_BOOT_HPP */
