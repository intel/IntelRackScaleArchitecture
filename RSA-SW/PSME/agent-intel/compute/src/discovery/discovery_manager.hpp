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
 * @file DiscoveryManager.hpp
 *
 * @brief Initial discovery implementation.
 * */

#ifndef DISCOVERYMANAGER_HPP
#define	DISCOVERYMANAGER_HPP

#include "ipmi/openipmi/management_controller.hpp"
#include "ipmi/get_device_id.hpp"
#include "ipmi/get_node_info.hpp"
#include "agent-framework/discovery/discovery_manager.hpp"
#include "agent-framework/module/module_manager.hpp"
#include "agent-framework/logger_ext.hpp"

using agent_framework::generic::Module;
using agent_framework::generic::Submodule;
using agent_framework::generic::ModuleManager;

namespace agent {
namespace compute {
namespace discovery {

using namespace agent::compute::ipmi;

/*!
 * @brief Implementation of initial discovery. Gets component id, translates it to IP and port number. After that
 * performs discovery of the module.
 */
class DiscoveryManager: public agent_framework::generic::DiscoveryManager {
public:

    /*!
     * @brief Default constructor.
     */
    DiscoveryManager();

    /*! Copy constructor */
    DiscoveryManager(const DiscoveryManager&) = default;

    /*! Assignment operator */
    DiscoveryManager& operator=(const DiscoveryManager&) = default;

    /*!
     * @brief Default destructor.
     */
    virtual ~DiscoveryManager();

    virtual void discover(Module & module) const;

private:

    void try_discover(Module & module) const;

    /**
     * @brief Checks is it Intel Atom or Intel Xeon module
     * @param module reference to module to be checked.
     * @return PRODUCT_ID which indicates module's type.
     */
    response::GetDeviceId::PRODUCT_ID discover_module_type(openipmi::ManagementController & mc, Module & module) const;

    void set_connection_data(openipmi::ManagementController & mc, const Module & module) const;
    void discover_xeon(const Module & module) const;
    void discover_atom(openipmi::ManagementController & mc, const Module & module) const;

    bool is_xeon(response::GetDeviceId::PRODUCT_ID id) const {
        return ((response::GetDeviceId::PRODUCT_ID_INTEL_XEON_BDC_A == id)
            || (response::GetDeviceId::PRODUCT_ID_INTEL_XEON_BDC_R == id));
    }

    bool is_atom(response::GetDeviceId::PRODUCT_ID id) const {
        return (response::GetDeviceId::PRODUCT_ID_INTEL_ATOM == id);
    }

    static constexpr unsigned long int XEON_SUBMODULES_NUMBER = 1;

    inline void set_to_present_avaiable_submodules(const Module & module, const response::GetNodeInfo & response) const;
    inline void set_to_absent_all_submodules(const Module & module) const;
    inline void set_to_present_xeon_virtual_submodule(const Module & module) const;
};

}
}
}
#endif	/* DISCOVERYMANAGER_HPP */

