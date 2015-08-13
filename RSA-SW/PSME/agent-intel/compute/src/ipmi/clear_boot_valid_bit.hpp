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
 * @file clear_boot_valid_bit.hpp
 *
 * @brief Clear Boot Valid Bit Command Declaration.
 * */

#ifndef AGENT_COMPUTE_IPMI_CLEAR_BOOT_VALID_BIT_HPP
#define AGENT_COMPUTE_IPMI_CLEAR_BOOT_VALID_BIT_HPP

#include "set_system_boot_options.hpp"

namespace agent {
namespace compute {
namespace ipmi {

namespace request {
/*!
 * @brief Request message for IPMI Clear Boot Valid Bit.
 */
class ClearBootValidBit: public request::SetSystemBootOptions {
public:

    /*!
     * @brief Default constructor.
     */
    ClearBootValidBit();

    /*!
     * @brief Copy constructor.
     * @param orig object to be copied.
     */
    ClearBootValidBit(const ClearBootValidBit& orig);

    /*!
     * @brief Default destructor.
     */
    virtual ~ClearBootValidBit();

    virtual void pack(vector<uint8_t>& data) const;

private:

};

}

namespace response {

/*!
 * @brief Response message for IPMI Clear Boot Valid Bit.
 */
class ClearBootValidBit: public response::SetSystemBootOptions {
public:

    /*!
     * @brief Default constructor.
     */
    ClearBootValidBit();

    /*!
     * @brief Copy constructor.
     * @param orig object to be copied.
     */
    ClearBootValidBit(const ClearBootValidBit& orig);

    /*!
     * @brief Default destructor.
     */
    virtual ~ClearBootValidBit();

    virtual void unpack(const vector<uint8_t>& data);

private:
    static constexpr size_t RESPONSE_SIZE = 1;
};

}

}
}
}
#endif	/* AGENT_COMPUTE_IPMI_CLEAR_BOOT_VALID_BIT_HPP */
