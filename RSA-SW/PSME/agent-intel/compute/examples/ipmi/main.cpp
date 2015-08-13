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
 * @file main.cpp
 *
 * @brief Example of IPMI lib use.
 * */
#include "ipmi/openipmi/management_controller.hpp"
#include "ipmi/get_device_id.hpp"
#include "ipmi/get_node_info.hpp"

#include <iostream>

using namespace std;
using namespace agent::compute::ipmi;

/*
 *
 */
int main(int argc, char ** argv) {

    constexpr int ARGS_NO = 3;
    openipmi::ManagementController mc;

    if (argc < ARGS_NO) {
        cerr << "error: to few arguments." << endl;
        cerr << "usage: " << argv[0] << " <ip_address> <port_number>" << endl;
        return -1;
    }

    mc.set_ip(argv[1]);
    mc.set_port(argv[2]);
    mc.set_username("USERID");
    mc.set_password("PASSW0RD");

    request::GetDeviceId device_req;
    response::GetDeviceId device_rsp;

    try {
        cout << "Started." << endl;
        openipmi::ManagementController::initialize();

        cout << "Before send." << endl;
        mc.send(device_req, device_rsp);
        cout << "Get Device ID." << endl;
        cout << "Completion code: " << uint32_t(device_rsp.get_completion_code()) << endl;
        cout << "FW Version: " << device_rsp.get_firmware_version() << endl;
        cout << "Product ID: " << uint32_t(device_rsp.get_product_id()) << endl;

        request::GetNodeInfo node_req;
        response::GetNodeInfo node_rsp;
        mc.send(node_req, node_rsp);
        cout << "Completion code: " << uint32_t(node_rsp.get_completion_code()) << endl;
        cout << "Number of nodes avaible: " << uint32_t(node_rsp.get_nodes_number()) << endl;
    } catch (runtime_error & error) {
        cout << "error: " << error.what() << endl;
    }

    openipmi::ManagementController::deinitialize();
    cout << "Done." << endl;
    return 0;
}
