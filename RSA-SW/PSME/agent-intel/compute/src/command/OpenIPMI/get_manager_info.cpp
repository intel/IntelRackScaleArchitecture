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
 * */

#include "agent-framework/command/compute/get_manager_info.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "ipmi/openipmi/management_controller.hpp"
#include "ipmi/get_device_id.hpp"

using namespace agent::compute;
using namespace agent_framework::generic;
using namespace agent_framework::command;

using std::string;
using agent_framework::command::exception::NotFound;

/*! Dummy GetManagerInfo implementation */
class GetManagerInfo : public compute::GetManagerInfo {
public:
    GetManagerInfo() { }

    using compute::GetManagerInfo::execute;

    void execute(const Request& request, Response& response) {
        (void)request;

        auto uuid = request.get_component();
        auto module = ModuleManager::get_module(uuid);
        auto submodule = ModuleManager::get_submodule(uuid);

        std::string ipv4 = "";
        std::string username = "";
        std::string password = "";
        uint32_t port = 0;

        log_debug(GET_LOGGER("rpc"), "Getting manager information of component: " << uuid);

        if (nullptr != module) {
            ipv4 = module->get_ip_address();
            username = module->get_username();
            password = module->get_password();
            port = module->get_port();
        }
        else {
            if (nullptr != submodule) {
                ipv4 = submodule->get_ip_address();
                username = submodule->get_username();
                password = submodule->get_password();
                port = submodule->get_port();
            }
            else {
                throw NotFound();
            }
        }

        log_debug(GET_LOGGER("rpc"), "Module details: ip=" << ipv4 << ", port=" << port);

        ipmi::request::GetDeviceId ipmi_request;
        ipmi::response::GetDeviceId ipmi_response;

        ipmi::openipmi::ManagementController mc;

        mc.set_ip(ipv4);
        mc.set_port(std::to_string(port));
        mc.set_username(username);
        mc.set_password(password);

        try {
            mc.send(ipmi_request, ipmi_response);

            std::string version = ipmi_response.get_firmware_version();

            log_debug(GET_LOGGER("rpc"), "Data received: firmware version=" << version);

            response.set_firmware_version(version);
            response.set_ipv4_address(ipv4);
            response.set_status_state("Enabled");
            response.set_status_health("OK");
            response.add_network_service({"IPMI", 0, false});
            response.add_network_service({"SSH", 0, false});
            response.add_network_service({"Telnet", 0, false});
            response.set_serial_console_bitrate(115200);
            response.set_serial_console_databits(8);
            response.set_serial_console_enabled(true);
            response.set_serial_console_flow_control("None");
            response.set_serial_console_parity("None");
            response.set_serial_console_pinout("Cisco");
            response.set_serial_console_stopbits(1);
            response.set_serial_console_type("Rs232");
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("rpc"), "Cannot get information about manager.");
            log_debug(GET_LOGGER("rpc"), error.what());
        }
    }

    ~GetManagerInfo();
};

GetManagerInfo::~GetManagerInfo() { }

static Command::Register<GetManagerInfo> g("Intel");
