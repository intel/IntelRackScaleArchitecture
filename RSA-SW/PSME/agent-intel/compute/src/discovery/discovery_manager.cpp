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
 * @file DiscoveryManager.cpp
 *
 * @brief ...
 * */
#include "discovery_manager.hpp"

using namespace agent::compute::discovery;
using namespace agent::compute::ipmi;

DiscoveryManager::DiscoveryManager() {}
DiscoveryManager::~DiscoveryManager() {}

void DiscoveryManager::discover(Module & module) const {
    try {
        try_discover(module);
    }    catch (runtime_error & error) {
        log_warning(GET_LOGGER("discovery"), "Cannot perform discovery: " << error.what());
    }
}

void DiscoveryManager::try_discover(Module & module) const {
    openipmi::ManagementController mc;
    openipmi::ManagementController::initialize();

    set_connection_data(mc, module);
    auto module_type = discover_module_type(mc, module);

    if (is_xeon(module_type)) {
        discover_xeon(module);
    }
    else if (is_atom(module_type)) {
        discover_atom(mc, module);
    }
    else {
        throw runtime_error("Module's type not recognized. Cannot perform deeper discovery.");
    }
}

void DiscoveryManager::set_connection_data(openipmi::ManagementController& mc, const Module& module) const {
    auto& ip_address = module.get_ip_address();
    auto port_number = module.get_port();
    auto& username = module.get_username();
    auto& password = module.get_password();

    mc.set_ip(ip_address);
    mc.set_port(port_number);
    mc.set_username(username);
    mc.set_password(password);
}

response::GetDeviceId::PRODUCT_ID DiscoveryManager::discover_module_type(openipmi::ManagementController& mc,
        Module& module) const {

    request::GetDeviceId request;
    response::GetDeviceId response;

    mc.send(request, response);

    auto completion_code = response.get_completion_code();
    if (response.COMPLETION_CODE_NORMAL != completion_code) {
        throw runtime_error("Bad completion code in Get Device Id response: " + to_string(uint32_t(completion_code)));
    }

    module.set_product_name(response.get_product_name());
    module.set_manufacturer_name(response.get_manufacturer_name());

    return response.get_product_id();
}

void DiscoveryManager::discover_atom(openipmi::ManagementController& mc, const Module& module) const {
    request::GetNodeInfo request;
    response::GetNodeInfo response;

    mc.send(request, response);

    auto completion_code = response.get_completion_code();
    if (response.COMPLETION_CODE_NORMAL != completion_code) {
        throw runtime_error("Bad completion code in Get Device Id response: " + to_string(uint32_t(completion_code)));
    }

    set_to_present_avaiable_submodules(module, response);
}

void DiscoveryManager::set_to_present_avaiable_submodules(const Module & module,
                                                          const response::GetNodeInfo & response) const {
    for (uint32_t index = 0; index < response.get_nodes_number(); index++) {
        auto& submodule = module.get_submodule(index);
        submodule->reset_port();
        submodule->set_presence(response.is_present(index));
    }
}

void DiscoveryManager::discover_xeon(const Module& module) const {
    set_to_absent_all_submodules(module);
    set_to_present_xeon_virtual_submodule(module);
}

void DiscoveryManager::set_to_absent_all_submodules(const Module& module) const {
    auto submodules_count = module.get_submodules().size();

    for (uint32_t index = 0; index < submodules_count; index++) {
        auto& submodule = module.get_submodule(index);
        submodule->set_presence(false);
    }
}

void DiscoveryManager::set_to_present_xeon_virtual_submodule(const Module& module) const {
    /*
     * Changes submodule default port to Xeon default port. Xeon has only one virtual blade which is mapped directly to
     * Intel Xeon module's BMC.
     */
    auto& virtual_submodule = module.get_submodules().front();
    virtual_submodule->set_presence(true);
    virtual_submodule->set_port(module.get_port());
}
