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
 * @file hw/fm10000/network_controller.cpp
 *
 * @brief Implementation of network interface
 * */
#include "network_controller.hpp"
#include "logger/logger_factory.hpp"

using namespace agent::network;
using namespace agent::network::hw::fm10000;

bool NetworkController::m_is_initialized = false;

NetworkController::NetworkController() {}

NetworkController::NetworkController(const NetworkController & orig) {
    (void)orig;
}

NetworkController::~NetworkController() {}

NetworkController& NetworkController::operator=(const NetworkController& orig) {
    (void)orig;
    throw runtime_error("Not supported.");
}

void NetworkController::initialize() {
    // If initialized do not do this again.
    if(is_initialized()) {
        return;
    }

    m_is_initialized = true;

    log_debug(GET_LOGGER("fm10000"), "Switch is successfully initialized.");
    log_debug(GET_LOGGER("fm10000"), "Switch UUID: "
              << get_switch_uuid(DEFAULT_SWITCH_ID));
}

void NetworkController::deinitialize() {
    // If not initialized do not deinitialize.
    if(!is_initialized()) {
        return;
    }

    m_is_initialized = false;
}

bool NetworkController::is_initialized()  {
    return m_is_initialized;
}
