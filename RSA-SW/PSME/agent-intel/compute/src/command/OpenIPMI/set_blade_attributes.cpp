
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
 * @brief Set Blade Attributes JSONRPC command implementation.
 * */

#include "agent-framework/command/compute/set_blade_attributes.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "ipmi/openipmi/management_controller.hpp"
#include "ipmi/clear_boot_valid_bit.hpp"
#include "ipmi/force_pxe_boot.hpp"
#include "ipmi/force_hdd_boot.hpp"
#include "ipmi/chassis_control_command.hpp"

#include <functional>
#include <map>

using namespace agent::compute::ipmi;
using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! SetBladeAttributes implementation */
class SetBladeAttributes : public compute::SetBladeAttributes {
public:
    SetBladeAttributes() {}

    using compute::SetBladeAttributes::execute;

    void execute(const Request& request, Response& response) {
        log_info(GET_LOGGER("rpc"), "Trying to execute setBladeAttributes.");
        try {
            try_execute(request, response);
        }
        catch (runtime_error& error) {
            log_error(GET_LOGGER("rpc"), "Cannot execute setBladeAttributes.");
            log_debug(GET_LOGGER("rpc"), error.what());
        }
    }

    ~SetBladeAttributes();

private:
    using DispatchMethodProto = void (openipmi::ManagementController& mc,
                                      const Request& request);

    using Manager = agent::compute::ipmi::openipmi::ManagementController;

    using DispatcherMap = std::map <std::string,
                                    std::function <DispatchMethodProto>>;

    // JSONRPC field names.
    static constexpr char BOOT_OVERRIDE_NONE[]       = "Disabled";
    static constexpr char BOOT_OVERRIDE_ONCE[]       = "Once";
    static constexpr char BOOT_OVERRIDE_CONTINUOUS[] = "Continuous";

    static constexpr char BOOT_TARGET_NONE[]       = "None";
    static constexpr char BOOT_TARGET_PXE[]        = "Pxe";
    static constexpr char BOOT_TARGET_CD[]         = "Cd";
    static constexpr char BOOT_TARGET_USB[]        = "Usb";
    static constexpr char BOOT_TARGET_HDD[]        = "Hdd";
    static constexpr char BOOT_TARGET_BIOS_SETUP[] = "BiosSetup";
    static constexpr char BOOT_TARGET_UTILITIES[]  = "Utilities";
    static constexpr char BOOT_TARGET_DIAGS[]      = "Diags";

    static constexpr char RESET_TYPE_ON[]            = "On";
    static constexpr char RESET_TYPE_FORCE_OFF[]     = "ForceOff";
    static constexpr char RESET_TYPE_SOFT_SHUTDOWN[] = "SoftShutdown";

    static constexpr const char POWER_STATE_NO_CHANGE[] = "NoChange";

    /**
     * Set Blade Attributes will take different actions depends on what Boot
     * Override Option has been sent.
     * Dispatcher selects the appropriate method relying on Boot Override
     * option. To add support for new Boot Override Options create method
     * and add it this map.
     */
    DispatcherMap m_boot_override_dispatch {
        {
            BOOT_OVERRIDE_NONE,
            [](Manager&, const Request&) { /* Do nothing. */ }
        },
        {
            BOOT_OVERRIDE_ONCE,
            [this](Manager& mc, const Request& request) {
                boot_override_once_handler(mc, request);
            }
        },
        {
            BOOT_OVERRIDE_CONTINUOUS,
            [this](Manager& mc, const Request& request) {
                boot_override_continuous_handler(mc, request);
            }
        }
    };

    /**
     * Set Blade Attributes will send different IPMI commands depends on what
     * Boot Target Option has been sent.
     * Dispatcher selects the appropriate method relying on Boot Target
     * option. To add support for new Boot Target Options just create method
     * and add it this map with appropriate key -> Boot Override Target option
     * read from Request.
     */
    DispatcherMap m_boot_target_dispatch {
        {
            BOOT_TARGET_NONE,
            [](Manager&, const Request&) { /* Do nothing. */ }
        },
        {
            BOOT_TARGET_PXE,
            [this](Manager& mc, const Request& request) {
                /* Force iPXE through PXE boot. */
                boot_target_pxe_handler(mc, request);
            }
        },
        {
            BOOT_TARGET_HDD,
            [this](Manager& mc, const Request& request) {
                /* Force local HDD on the Module's board. */
                boot_target_hdd_handler(mc, request);
            }
        }
    };


    /**
     * Main logic.
     */
    void try_execute(const Request& request, Response&) const {
        Manager mc;
        auto* submodule = try_get_submodule(request.get_component());

        set_mc_address(mc, *submodule);

        // Dispatches Boot Override option.
        const auto& boot_override = request.get_boot_override();

        if (!key_exists(m_boot_override_dispatch, boot_override)) {
            throw std::runtime_error("Unrecognized boot override option: " +
                                     boot_override);
        }

        const auto& handler = m_boot_override_dispatch.at(boot_override);

        // Executes desired method resposible for dispatch boot target.
        handler(mc, request);

        const auto& power_state = request.get_power_state();

        // NoChange is a special Power State symbol to indicate that
        // there is no reason to change power state.
        // Currently single JSON-RPC command is used for boot order and
        // power transition.
        if (power_state != POWER_STATE_NO_CHANGE) {
            // Changes power state of the Module.
            send_power_state(mc, request);
        }
    }

    Submodule* try_get_submodule(const std::string& uuid) const {
        auto* submodule = ModuleManager::get_submodule(uuid);
        if (nullptr == submodule) {
            log_warning(GET_LOGGER("rpc"), "Cannot get Submodule with uuid: "
                                           << uuid);
            throw exception::NotFound();
        }
        return submodule;
    }

    void set_mc_address(openipmi::ManagementController& mc,
                               const Submodule& submodule) const {
        mc.set_ip(submodule.get_ip_address());
        mc.set_port(submodule.get_port());
        mc.set_username(submodule.get_username());
        mc.set_password(submodule.get_password());
    }


    void boot_override_once_handler(Manager& mc, const Request& request) {
        // Dispatch Boot Override Target option.
        const auto& boot_target = request.get_boot_override_target();

        if (!key_exists(m_boot_target_dispatch, boot_target)) {
            throw std::runtime_error("Unrecognized boot override option: " +
                                     boot_target);
        }

        const auto& handler = m_boot_target_dispatch.at(boot_target);
        handler(mc, request);
    }

    void boot_override_continuous_handler(Manager& mc, const Request& request) {
        // Continues not supported yet. Behaves like BootOverride==Once.
        boot_override_once_handler(mc, request);
    }


    void boot_target_pxe_handler(Manager& mc, const Request& ) {

        request::ForcePxeBoot ipmi_request{};
        response::ForcePxeBoot ipmi_response{};

        mc.send(ipmi_request, ipmi_response);

        const auto cc = ipmi_response.get_completion_code();

        if (cc) {
            throw std::runtime_error("Force PXE boot completion code: " +
                                     to_string(unsigned(cc)));
        }
    }

    void boot_target_hdd_handler(Manager& mc, const Request&) {

        request::ForceHddBoot ipmi_request{};
        response::ForceHddBoot ipmi_response{};

        mc.send(ipmi_request, ipmi_response);

        const auto cc = ipmi_response.get_completion_code();

        if (cc) {
            throw std::runtime_error("Force local HDD boot completion code: " +
                                     to_string(unsigned(cc)));
        }

    }

    void send_power_state(openipmi::ManagementController& mc,
                          const Request& request) const {
        using std::to_string;

        request::ChassisControlCommand ipmi_request{};
        response::ChassisControlCommand ipmi_response{};

        const auto& power_state = request.get_power_state();

        ipmi_request.set_power_state(power_state);
        mc.send(ipmi_request, ipmi_response);

        const auto cc = ipmi_response.get_completion_code();

        if (cc) {
            throw std::runtime_error("Chassis Control Command "
                                     "bad completion code: "    +
                                     to_string(unsigned(cc)));
        }
    }

    bool key_exists(const DispatcherMap& my_map,
                    const std::string& option) const {
        return (my_map.find(option) != my_map.end());
    }


};

constexpr char SetBladeAttributes::BOOT_OVERRIDE_NONE[];
constexpr char SetBladeAttributes::BOOT_OVERRIDE_ONCE[];
constexpr char SetBladeAttributes::BOOT_OVERRIDE_CONTINUOUS[];

constexpr char SetBladeAttributes::BOOT_TARGET_NONE[];
constexpr char SetBladeAttributes::BOOT_TARGET_PXE[];
constexpr char SetBladeAttributes::BOOT_TARGET_CD[];
constexpr char SetBladeAttributes::BOOT_TARGET_USB[];
constexpr char SetBladeAttributes::BOOT_TARGET_HDD[];
constexpr char SetBladeAttributes::BOOT_TARGET_BIOS_SETUP[];
constexpr char SetBladeAttributes::BOOT_TARGET_UTILITIES[];
constexpr char SetBladeAttributes::BOOT_TARGET_DIAGS[];
constexpr char SetBladeAttributes::POWER_STATE_NO_CHANGE[];

SetBladeAttributes::~SetBladeAttributes() {}

static Command::Register<SetBladeAttributes> g("Intel");
