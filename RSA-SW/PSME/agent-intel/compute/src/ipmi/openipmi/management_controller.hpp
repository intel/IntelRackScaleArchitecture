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
 * @file /ipmi/openipmi/management_controller.hpp
 *
 * @brief Implementation of IPMI interface using OpenIPMI library. Sends synchronus messages to MC.
 * */

#ifndef AGENT_IPMI_OPENIPMI_MANAGEMENT_CONTROLLER_HPP
#define AGENT_IPMI_OPENIPMI_MANAGEMENT_CONTROLLER_HPP

#include "../management_controller.hpp"

#include <OpenIPMI/ipmi_mc.h>
#include <OpenIPMI/ipmiif.h>
#include <OpenIPMI/ipmi_posix.h>
#include <OpenIPMI/ipmi_lan.h>
#include <OpenIPMI/ipmi_auth.h>

#include <semaphore.h>
#include <signal.h>

#include <cstring>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>

using std::uint16_t;
using std::string;
using std::to_string;
using std::runtime_error;
using std::mutex;

namespace agent {
namespace compute {
namespace ipmi {
namespace openipmi {


/*!
 * @brief Implementation of ManagementController interface basing on OpenIPMI library.
 * This class hides the public interface of Management Controller and provide new, specialized to
 * certain commands.
 * */
class ManagementController: public ipmi::ManagementController {
public:

    /*!
     * Default constructor.
     */
    ManagementController();

    /*!
     * Destructor.
     */
    virtual ~ManagementController();

    /*!
     * @brief Initializes IPMI. Need to be run once before any operations.
     */
    static void initialize();

    /*!
     * @brief Deinitializes IPMI. Need to be run at the end of the application to free resources.
     */
    static void deinitialize();

    /*!
     * @brief Checks if IPMI was initialized.
     *
     * @return true if initialize() was called successfuly, otherwise false.
     */
    static bool is_initialized();

    /*!
     * @brief Set password
     * @param[in]   password    Password
     * */
    void set_password(const string& password) { m_password = password; }

    /*!
     * @brief Set username
     * @param[in]   username    Username
     * */
    void set_username(const string& username) { m_username = username; }

    /*!
     * @brief Sets IP address of the Management Controller. Commands will be send to this address.
     * @param ip_address IP address. Example: "10.3.0.220".
     */
    void set_ip(const string& ip_address);

    /*!
     * Returns IP address.
     * @return IP address.
     */
    const string& get_ip() const;

    /*!
     * @brief Sets port number.
     * @param port_number port number. For example "623".
     */
    void set_port(const string& port_number);

    /*!
     * @brief Sets port number.
     * @param port_number port number. For example 623.
     */
    void set_port(uint32_t port_number);

    /*!
     * @brief Gets port number.
     *
     * @return port number.
     */
    const string & get_port() const;

    /*!
     * @brief Sends Request to ManagementController
     *
     * @param request reference to request object.
     * @param response reference to response object.
     */
    virtual void send(const ipmi::Request& request, ipmi::Response& response);

private:
    /*!
     * Private copy constructor. Disabled object copying.
     * @param orig object to copy.
     */
    ManagementController(const ManagementController& orig);

    /*!
     * Private assignemnt operator. Disabled object copying.
     * @param orig assaign object reference.
     * @return OpenIpmiManagementController reference.
     */
    ManagementController & operator=(const ManagementController& orig);

    static constexpr unsigned int DEFAULT_LUN_NUMBER = 0;
    static constexpr int DEFAULT_THREAD_PRIORITY = 0;

    static os_handler_t * m_os_handler;
    static bool m_is_initialized;
    static volatile bool m_is_thread_running;
    static volatile bool m_is_thread_stopped;

    static long int m_single_operation_timeout_usec;
    static long int m_single_operation_timeout_sec;

    /*!
     * @brief Thread function. Processes IPMI operations in loop.
     *
     * @param data data passed to thread: os_handler.
     */
    static void operation_loop_thread(void * data);

    static void connection_change_handler(ipmi_domain_t *domain, int err, unsigned int conn_num,
                                          unsigned int port_num, int still_connected, void *user_data);

    static void domain_fully_up_handler(ipmi_domain_t *domain, void *cb_data);
    static void iterate_mc_handler(ipmi_domain_t * domain,  ipmi_mc_t * mc, void * cb_data);
    static void ipmi_domain_pointer_handler(ipmi_domain_t *domain, void *cb_data);
    static void ipmi_domain_close_handler(void *cb_data);
    static void response_handler(ipmi_mc_t  *src, ipmi_msg_t *msg, void* rsp_data);

    bool m_is_connected = false;
    long int m_open_domain_timeout_nsec = 0;
    long int m_open_domain_timeout_sec = 10;

    static sem_t wait_for_open_domain;
    static sem_t wait_for_response;
    static sem_t wait_for_close_domain;
    static mutex m_wait_for_finish;

    string m_ip_address {};
    string m_port_number {};
    string m_username {};
    string m_password {};

    ipmi_con_t * m_connection {};
    ipmi_domain_id_t m_domain {{}};
    ipmi_mc_t * m_management_controller {};

    virtual void connect();

    virtual void disconnect();

    virtual bool is_connected() const;

    static void create_os_handler();
    static void init_ipmi_and_thread();

    static void init_semaphores();
    static void destroy_semaphores();
    static void copy_data_to_vector(vector<uint8_t> & output_vector, ipmi_msg_t* msg);
    void setup_connection();
    void open_domain();
    void wait_for_domain_or_timeout();


};

}
}
}
}
#endif	/* AGENT_IPMI_OPENIPMI_MANAGEMENT_CONTROLLER_HPP */

