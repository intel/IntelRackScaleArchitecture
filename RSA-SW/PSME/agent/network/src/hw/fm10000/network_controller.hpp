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
 * @file hw/fm10000/network_controller.hpp
 *
 * @brief Implementation of network interface
 * */

#ifndef AGENT_NETWORK_FM10000_NETWORK_CONTROLLER_HPP
#define AGENT_NETWORK_FM10000_NETWORK_CONTROLLER_HPP

#include "hw/network_controller.hpp"

#include <cstring>
#include <vector>
#include <string>

using std::string;
using std::to_string;
using std::runtime_error;

namespace agent {
namespace network {
namespace hw {
namespace fm10000 {


/*!
 * @brief Implementation of NetworkController interface for fm10000 switch
 * This class hides the public interface of Network Controller and provide new, specialized to
 * certain commands.
 * */
class NetworkController: public hw::NetworkController {
public:

    /*!
     * Default constructor.
     */
    NetworkController();

    /**
     * Copy constructor.
     * @param orig object to copy.
     */
    NetworkController(const NetworkController& orig);

    /*!
     * @brief Initializes fm10000 switch. Need to be run once before any operations.
     */
    void initialize();

    /*!
     * @brief Deinitializes fm10000 switch. Need to be run at the end of the application to free resources.
     */
    void deinitialize();

    /*!
     * @brief Checks if fm10000 switch was initialized.
     *
     * @return true if initialize() was called successfuly, otherwise false.
     */
    bool is_initialized();

    /*!
     * Assignemnt operator.
     * @param orig assaign object reference.
     * @return fm10000 NetworkController reference.
     */
    [[ noreturn ]]NetworkController & operator=(const NetworkController& orig);

    /*!
     * Destructor.
     */
    virtual ~NetworkController();

private:
    static bool m_is_initialized;
};

}
}
}
}
#endif	/* AGENT_NETWORK_FM10000_NETWORK_CONTROLLER_HPP */
