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

#include "agent-framework/command/network/get_manager_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::generic;
using namespace agent_framework::command;

using std::string;
using agent_framework::command::exception::NotFound;

namespace agent {
namespace network {
namespace hw {
namespace fm6000 {

using namespace agent_framework;

/*! Dummy GetManagerInfo implementation */
class GetManagerInfo : public command::network::GetManagerInfo {
public:
    GetManagerInfo() { }

    using command::network::GetManagerInfo::execute;

    void execute(const Request& request, Response& response) {
        auto uuid = request.get_component();
        auto module = ModuleManager::get_module(uuid);
        auto submodule = ModuleManager::get_submodule(uuid);

        std::string ipv4 = "";

        if (nullptr != module) {
            ipv4 = module->get_ip_address();
        }
        else {
            if (nullptr != submodule) {
                ipv4 = submodule->get_ip_address();
            }
            else {
                throw NotFound();
            }
        }

        response.set_firmware_version("");
        response.set_ipv4_address(ipv4);
        response.set_status_state("Enabled");
        response.set_status_health("OK");
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

    ~GetManagerInfo();
};

GetManagerInfo::~GetManagerInfo() { }

}
}
}
}

static Command::Register<agent::network::hw::fm6000::GetManagerInfo> g("fm6000");
