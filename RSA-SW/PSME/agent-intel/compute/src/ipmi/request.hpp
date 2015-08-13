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
 * @file request.hpp
 *
 * @brief Request object from IPMI.
 * */

#ifndef AGENT_IPMI_REQUEST_HPP
#define AGENT_IPMI_REQUEST_HPP

#include "message.hpp"

#include <cstdint>
#include <vector>

using std::uint8_t;
using std::vector;

namespace agent {
namespace compute {
namespace ipmi {
/*!
 * @brief Request object to IPMI.
 * */
class Request: public Message {
public:
    /*!
     * @brief Default constructor.
     */
    Request(NetFn fn, Cmd cmd);

    /*!
     * @brief Copy constructor.
     * @param orig object to copy.
     */
    Request(const Request & orig);

    /*!
     * @brief Destructor.
     */
    virtual ~Request();

    /**
     * Packs data from Request object into vector of bytes.
     *
     * @param data reference to vector where data will be stored.
     */
    virtual void pack(vector<uint8_t> & data) const = 0;
};

}
}
}

#endif	/* AGENT_IPMI_REQUEST_HPP */
