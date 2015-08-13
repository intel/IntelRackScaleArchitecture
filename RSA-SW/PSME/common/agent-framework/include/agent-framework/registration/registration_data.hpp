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
 * @file registration_data.hpp
 * @brief Registration Data
 * */

#ifndef AGENT_FRAMEWORK_REGISTRATION_REGISTRATION_DATA_HPP
#define AGENT_FRAMEWORK_REGISTRATION_REGISTRATION_DATA_HPP

#include <string>

namespace agent_framework {
namespace generic {

/*! Default registration IP */
const char APP_SERVER_REGISTARTION_DEFAULT_IP[] = "localhost";
/*! Default registartion port */
const int APP_SERVER_REGISTARTION_DEFAULT_PORT = 8383;

/*! Registration Data */
class RegistrationData {
    std::string m_gami_id{};
    std::string m_app_rpc_server_ip;
    int m_app_rpc_server_port;
public:

    /*! Default constructor */
    RegistrationData() :
        m_app_rpc_server_ip(APP_SERVER_REGISTARTION_DEFAULT_IP),
        m_app_rpc_server_port(APP_SERVER_REGISTARTION_DEFAULT_PORT) {}

    /*!
     * @brief Sets registration client id
     *
     * @param gami_id client id
     */
    void set_gami_id(const std::string& gami_id) {
        m_gami_id = gami_id;
    }

    /*!
     * @brief Gets registration client id
     *
     * @return client id
     */
    const std::string& get_gami_id() const {
        return m_gami_id;
    }

    /*! Sets registration server IP. */
    void set_app_rpc_server_ip(const std::string& ip) {
        m_app_rpc_server_ip = ip;
    }

    /*! Gets registration server IP. */
    std::string get_app_rpc_server_ip(void) const {
        return m_app_rpc_server_ip;
    }

    /*! Sets registration server port */
    void set_app_rpc_server_port(int port) {
        m_app_rpc_server_port = port;
    }

    /*! Gets registration server port */
    int get_app_rpc_server_port(void) const {
        return m_app_rpc_server_port;
    }
};

}
}

#endif /* AGENT_FRAMEWORK_REGISTRATION_REGISTRATION_DATA_HPP */


