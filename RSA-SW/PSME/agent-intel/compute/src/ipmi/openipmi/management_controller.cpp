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
 * @file /ipmi/openipmi/management_controller.cpp
 *
 * @brief Implementation of IPMI interface using OpenIPMI library. Sends synchronus messages to MC.
 * */
#include "management_controller.hpp"

using namespace agent::compute;
using namespace agent::compute::ipmi::openipmi;

os_handler_t * ManagementController::m_os_handler = nullptr;

bool ManagementController::m_is_initialized = false;
volatile bool ManagementController::m_is_thread_running = false;
volatile bool ManagementController::m_is_thread_stopped = true;

long int ManagementController::m_single_operation_timeout_usec = 0;
long int ManagementController::m_single_operation_timeout_sec = 3;

sem_t ManagementController::wait_for_response {};
sem_t ManagementController::wait_for_open_domain{};
sem_t ManagementController::wait_for_close_domain{};
mutex ManagementController::m_wait_for_finish{};

ManagementController::ManagementController() {}
ManagementController::~ManagementController() {}

void ManagementController::initialize() {
    std::unique_lock<std::mutex> lk(m_wait_for_finish);
    // If initialized do not do this again.
    if(is_initialized()) {
        return;
    }
    init_semaphores();
    create_os_handler();
    init_ipmi_and_thread();

    m_is_initialized = true;
}


void ManagementController::create_os_handler() {
    m_os_handler = ipmi_posix_thread_setup_os_handler(SIGUSR1);

    if (nullptr == m_os_handler) {
        throw runtime_error("Cannot create OS handler.");
    }
}

void ManagementController::init_ipmi_and_thread() {
    int retval = ipmi_init(m_os_handler);

    if (retval) {
        throw runtime_error("Cannot initialize IPMI.");
    }

    retval = m_os_handler->create_thread(m_os_handler, DEFAULT_THREAD_PRIORITY,
                                         operation_loop_thread, m_os_handler);

    if (retval) {
        throw runtime_error("Cannot create worker.");
    } else {
        /* Wait for thread */
        while (!m_is_thread_running);
    }
}

void ManagementController::operation_loop_thread(void* data) {
    os_handler_t *os_hnd = static_cast<os_handler_t*>(data);
    struct timeval ts;

    ts.tv_sec = m_single_operation_timeout_sec;
    ts.tv_usec = m_single_operation_timeout_usec;

    m_is_thread_stopped = false;
    m_is_thread_running = true;

    while (m_is_thread_running) {
        os_hnd->perform_one_op(os_hnd, &ts);
    }

    m_is_thread_stopped = true;

    os_hnd->thread_exit(os_hnd);
}

void ManagementController::deinitialize() {
    std::unique_lock<std::mutex> lk(m_wait_for_finish);
    // If not initialized do not deinitialize.
    if(!is_initialized()) {
        return;
    }
    // After sending a message connection and domain will be released.
    // So we need only to shutdown ipmi library and OS handler.
    m_is_thread_running = false;

    /* Wait for all finished jobs in the thread */
    while (!m_is_thread_stopped);

    ipmi_shutdown();

    if(m_os_handler) {
        m_os_handler->free_os_handler(m_os_handler);
    }

    m_is_initialized = false;
    destroy_semaphores();
}
void ManagementController::destroy_semaphores() {
    sem_destroy(&wait_for_close_domain);
    sem_destroy(&wait_for_response);
    sem_destroy(&wait_for_open_domain);
}


void ManagementController::iterate_mc_handler(ipmi_domain_t* domain,
                                              ipmi_mc_t* mc, void* cb_data) {
    (void)domain;
    // Gets reference to object.
    ManagementController * current_mc = reinterpret_cast<ManagementController *>
                                        (cb_data);
    // Assigns ipmi_mc_t to attribute of called object.
    current_mc->m_management_controller = mc;
}

void ManagementController::ipmi_domain_pointer_handler(ipmi_domain_t* domain,
                                                               void* cb_data) {
    (void)cb_data;
    int retval = ipmi_domain_close(domain, ipmi_domain_close_handler, nullptr);

    if (retval) {
        throw runtime_error("Cannot ipmi_domain_close: " + to_string(retval));
    }
}

void ManagementController::ipmi_domain_close_handler(void *cb_data) {
    (void)cb_data;
    sem_post(&wait_for_close_domain);
}

void ManagementController::connection_change_handler(ipmi_domain_t* domain,
                                                     int err,
                                                     unsigned int conn_num,
                                                     unsigned int port_num,
                                                     int still_connected,
                                                     void* user_data) {
    (void)domain;
    (void)err;
    (void)conn_num;
    (void)port_num;
    (void)still_connected;
    (void)user_data;
}

void ManagementController::domain_fully_up_handler(ipmi_domain_t* domain,
                                                           void* cb_data) {
    (void)domain;
    (void)cb_data;
    sem_post(&wait_for_open_domain);
}

void ManagementController::response_handler(ipmi_mc_t* src,
                                            ipmi_msg_t* msg,
                                            void * rsp_data) {
    (void)src;
    vector<uint8_t> * output_vector = static_cast<vector<uint8_t>*>(rsp_data);

    copy_data_to_vector(*output_vector, msg);
    sem_post(&wait_for_response);
}

void ManagementController::copy_data_to_vector(vector<uint8_t>& output_vector,
                                               ipmi_msg_t* msg) {
    output_vector.reserve(msg->data_len);
    for (uint32_t index = 0 ; index < msg->data_len ; index++) {
        output_vector.push_back(msg->data[index]);
    }
}


void ManagementController::send(const ipmi::Request& request,
                                ipmi::Response& response) {
    std::unique_lock<std::mutex> lk(m_wait_for_finish);

    ipmi_msg_t message_to_send = {0, 0, 0, nullptr};
    vector<uint8_t> data_to_send{};
    vector<uint8_t> data_to_receive{};

    if(!is_initialized()) {
        throw runtime_error("OpenIPMI Management Controller must be initialized before send!");
    }

    // We connect and disconnect on every request.
    connect();

    // Saves data in vector.
    request.pack(data_to_send);

    message_to_send.cmd = uint8_t(request.get_command());
    message_to_send.netfn = uint8_t(request.get_network_function());
    message_to_send.data = data_to_send.data();
    message_to_send.data_len = uint16_t(data_to_send.size());

    int retval = ipmi_mc_send_command(m_management_controller,
                                      DEFAULT_LUN_NUMBER,
                                      &message_to_send,
                                      response_handler,
                                      &data_to_receive);

    if(retval) {
        throw runtime_error("Cannot send command: " + to_string(retval));
    }

    sem_wait(&wait_for_response);
    response.unpack(data_to_receive);

    disconnect();
}

void ManagementController::connect() {
    setup_connection();
    open_domain();

    // Gets ipmi_mc_t pointer to enable of sending commands.
    int retval = ipmi_domain_iterate_mcs(m_domain.domain, iterate_mc_handler, this);

    if (retval) {
        throw runtime_error("Cannot ipmi_domain_iterate_mcs: " + to_string(retval));
    }
}

void ManagementController::init_semaphores() {
    sem_init(&wait_for_open_domain, 0, 0);
    sem_init(&wait_for_response, 0, 0);
    sem_init(&wait_for_close_domain, 0, 0);
}

void ManagementController::setup_connection() {
    // Just to pass as parameters without warnings.
    char * const ip_address[] =  {const_cast<char * const>(m_ip_address.c_str())};
    char * const port_number[] = {const_cast<char * const>(m_port_number.c_str())};

    int retval = ipmi_ip_setup_con(ip_address,
                                   port_number, 1,
                                   IPMI_AUTHTYPE_RMCP_PLUS,
                                   IPMI_PRIVILEGE_ADMIN,
                                   const_cast<char *>(m_username.c_str()),
                                   static_cast<unsigned int>(m_username.size()),
                                   const_cast<char *>(m_password.c_str()),
                                   static_cast<unsigned int>(m_password.size()),
                                   m_os_handler,
                                   nullptr,
                                   &m_connection);
    if (retval) {
        throw runtime_error("Cannot ipmi_ip_setup_con: " + to_string(retval));
    }
}

void ManagementController::open_domain() {
    ipmi_open_option_t option = { IPMI_OPEN_OPTION_ALL, { 0 }};

    int retval = ipmi_open_domain("domain", &m_connection, 1,
                                  connection_change_handler, nullptr,
                                  domain_fully_up_handler,
                                  m_os_handler,
                                  &option, 1,
                                  &m_domain);
    if (retval) {
        throw runtime_error("Cannot ipmi_open_domain: " + to_string(retval));
    }

    wait_for_domain_or_timeout();
}

void ManagementController::wait_for_domain_or_timeout() {
    int retval = 0;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += m_open_domain_timeout_nsec;
    ts.tv_sec  += m_open_domain_timeout_sec;

    // Waits for creation of domain or timeout.
    while ((retval = sem_timedwait(&wait_for_open_domain, &ts)) == -1 && errno == EINTR) {
        continue;
    }

    if (-1 == retval) {
        disconnect(); // Cleanup everything.
        if (ETIMEDOUT == errno) {
            throw runtime_error("Connection timeout occured.");
        }
        else {
            throw runtime_error("Error sem_timedwait: "
                                     + to_string(errno)
                                     + " "
                                     + strerror(errno));
        }
    }
}

void ManagementController::disconnect() {
    int retval = ipmi_domain_pointer_cb(m_domain, ipmi_domain_pointer_handler, nullptr);

    if (retval) {
        throw runtime_error("Cannot ipmi_domain_pointer_cb: " + to_string(retval));
    }
}

const string & ManagementController::get_ip() const {
    return m_ip_address;
}

const string & ManagementController::get_port() const {
    return m_port_number;
}

void ManagementController::set_ip(const string& ip_address) {
    m_ip_address = ip_address;
}

void ManagementController::set_port(const string& port_number) {
    m_port_number = port_number;
}

void ManagementController::set_port(uint32_t port_number) {
    set_port(to_string(port_number));
}

bool ManagementController::is_connected() const {
    return m_is_connected;
}

bool ManagementController::is_initialized()  {
    return m_is_initialized;
}
