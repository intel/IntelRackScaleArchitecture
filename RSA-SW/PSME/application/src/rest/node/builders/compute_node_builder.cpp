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
 * */

#include "psme/rest/node/builders/compute_node_builder.hpp"
#include "psme/rest/node/builders/builder_util.hpp"
#include "psme/rest/node/crud/drawers.hpp"
#include "psme/rest/node/crud/managers.hpp"
#include "psme/rest/node/crud/compute_modules.hpp"
#include "psme/rest/node/crud/blades.hpp"
#include "psme/rest/node/crud/processors.hpp"
#include "psme/rest/node/crud/memory_modules.hpp"
#include "psme/rest/node/crud/storage_controllers.hpp"
#include "psme/rest/node/crud/drives.hpp"
#include "psme/rest/node/crud/ethernet_interfaces.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/resource/location.hpp"
#include "psme/rest/resource/system_type.hpp"
#include "psme/rest/resource/chassis_type.hpp"
#include "core/service/service_factory.hpp"
#include "core/service/compute_service.hpp"
#include "core/dto/component_dto.hpp"
#include "core/dto/manager_info_dto.hpp"

using namespace psme::rest::node;
using namespace psme::rest::resource;
using namespace psme::rest::utils;
using namespace psme::core::dto;

using psme::core::service::ServiceFactory;
using psme::core::service::AgentService;

namespace {
    constexpr const char JSONRPC_BLADE_NAME[] = "RSABlade";
    constexpr const char JSONRPC_COMPUTE_MODULE_NAME[] = "RSAComputeModule";
    constexpr const char JSONRPC_CHASSIS_MODULE_NAME[] = "RSAChassis";
}

ComputeNodeBuilder::~ComputeNodeBuilder() { }

NodesLinkVec
ComputeNodeBuilder::build_nodes(Node& root, const string& component_id) {
    if (nullptr == root.get_next() || nullptr == root.get_next()->get_next()) {
        throw std::runtime_error("Tree is not properly initialized.");
    }
    auto* v_node = root.get_next()->get_next();
    auto* drawer = v_node->get_node_by_id(Drawers::TYPE).get_next();

    if (nullptr == drawer) {
        throw std::runtime_error("Tree is not properly initialized.");
    }

    auto& manager_collection = v_node->get_node_by_id(Managers::TYPE);
    auto& module_collection = drawer->get_node_by_id(ComputeModules::TYPE);

    auto service = ServiceFactory::create_compute(get_agent()->get_gami_id());

    NodesLinkVec nodes_to_link;
    auto components = service.get_components(component_id).get_components();
    for (const auto& component : components) {
        if (JSONRPC_COMPUTE_MODULE_NAME == component.get_type()) {
            // create compute module
            auto compute_module = build_compute_module(service, component.get_name());
            nodes_to_link.emplace_back(LinkType::COMPOSITION,
                Resource::MEMBERS, module_collection, compute_module);
            nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                ComputeModule::TYPE, *drawer,
                compute_module, Resource::CONTAINED_BY);
            // compute module's manager
            auto manager = build_manager(service, *compute_module);
            nodes_to_link.emplace_back(LinkType::COMPOSITION,
                Resource::MEMBERS, manager_collection, manager);
            nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                    Manager::MANAGER_FOR_COMPUTEMODULES, *manager,
                    compute_module, Resource::MANAGED_BY);
            auto& blade_collection = compute_module->get_node_by_id(Blades::TYPE);
            for (const auto& subcomponent : component.get_subcomponents()) {
                if (JSONRPC_BLADE_NAME == subcomponent.get_type()) {
                    auto blade = build_blade(service, subcomponent.get_name());
                    nodes_to_link.emplace_back(LinkType::COMPOSITION,
                        Resource::MEMBERS, blade_collection, blade);
                    // blade's manager
                    auto bmanager = build_manager(service, *blade);
                    nodes_to_link.emplace_back(LinkType::COMPOSITION,
                        Resource::MEMBERS, manager_collection, bmanager);
                    nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                        Manager::MANAGER_FOR_BLADES, *bmanager,
                        blade, Resource::MANAGED_BY);
                    // link blade with compute module
                    nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                        Blade::TYPE, *compute_module,
                        blade, Resource::CHASSIS);
                }
            }
        } else if (JSONRPC_CHASSIS_MODULE_NAME == component.get_type()) {
            update_drawer_location(service, drawer, component.get_name());
        }
    }
    return nodes_to_link;
}

NodeSharedPtr
ComputeNodeBuilder::build_compute_module(ComputeService& service,
        const std::string& uuid) {
    // create compute module node
    auto compute_module = std::make_shared<ComputeModule>(uuid,
                                             service.get_agent().get_gami_id());
    // create blades under compute module
    auto blades = std::make_shared<Blades>();
    link_nodes(LinkType::COMPOSITION,
        Blades::TYPE, *compute_module, blades, Resource::CHASSIS);

    // execute JSONRPC query
    auto module_info = service.get_module_info(compute_module->get_uuid());
    // populate JSON data
    json::Value json;
    json["ChassisType"] = to_string(ChassisType::SLED);
    json["Manufacturer"] = module_info.get_fru_info().get_manufacturer();
    json["Model"] = module_info.get_fru_info().get_model_number();
    json["SerialNumber"] = module_info.get_fru_info().get_serial_number();
    json[Status::STATUS] = to_resource_status(module_info.get_status()).as_json();
    json[Resource::ENUMERATED] = to_string(EnumStatus::ENUMERATED);
    json[Resource::OEM] = module_info.get_oem_data().to_json_value();
    json[Location::LOCATION][Location::MODULE] = module_info.get_slot();

    compute_module->get_resource().patch(json);

    return compute_module;
}

namespace {
    using namespace psme::core::dto::compute;
    json::Value
    boot_override_supported_to_json(
            const BladeInfoDTO::Response::BootSupportedVec& boot_supported) {
        json::Value support_json(json::Value::Type::ARRAY);
        for (const auto& boot : boot_supported) {
            support_json.push_back(boot);
        }
        return support_json;
    }
}

NodeSharedPtr
ComputeNodeBuilder::build_blade(ComputeService& service, const std::string& uuid) {
    // create blade node
    auto blade = std::make_shared<Blade>(uuid,
                                             service.get_agent().get_gami_id());
    // execute JSONRPC query
    auto blade_info = service.get_blade_info(blade->get_uuid());

    // populate JSON data
    json::Value json;
    json["SystemType"] = to_string(SystemType::PHYSICAL);
    json["Manufacturer"] = blade_info.get_fru_info().get_manufacturer();
    json["SerialNumber"] = blade_info.get_fru_info().get_serial_number();
    json["Model"] = blade_info.get_fru_info().get_model_number();
    json["Bios"]["Current"]["VersionString"] = blade_info.get_bios_version();
    json["Boot"]["BootSourceOverrideEnabled"] = blade_info.get_boot_override();
    json["Boot"]["BootSourceOverrideTarget"] =
                                        blade_info.get_boot_override_target();
    json[Resource::OEM] = blade_info.get_oem_data().to_json_value();
    json["Boot"]["BootSourceOverrideSupported"] =
    boot_override_supported_to_json(blade_info.get_boot_override_supported());
    json["Boot"]["UefiTargetBootSourceOverride"] = json::Value::Type::NIL;
    build_processors(service, *blade, json, blade_info.get_processor_count());
    build_memory(service, *blade, json, blade_info.get_dimm_count());
    json[Status::STATUS] = to_resource_status(blade_info.get_status()).as_json();
    json[Resource::ENUMERATED] = to_string(EnumStatus::ENUMERATED);

    json[Location::LOCATION][Location::BLADE] = blade_info.get_slot();

    auto count = blade_info.get_controller_count();
    json["StorageCapable"] = count > 0;
    json["StorageControllersCount"] = count;
    build_storage_controllers(service, *blade, count);
    build_ethernet_interfaces(service, *blade, blade_info.get_nic_count());

    blade->get_resource().patch(json);

    return blade;
}

NodeSharedPtr
ComputeNodeBuilder::build_storage_controllers(ComputeService& service,
                                            Node& blade, std::uint32_t count) {
    // create blade's storage controller collection
    auto storage_controllers = std::make_shared<StorageControllers>();
    link_nodes(LinkType::COMPOSITION,
        StorageControllers::TYPE, blade, storage_controllers);

    const auto& blade_id = blade.get_uuid();

    for (std::uint32_t i = 0; i < count; ++i) {
        auto controller_info = service.get_storage_controller_info(blade_id, i);
        auto controller_node = std::make_shared<StorageController>();
        link_nodes(LinkType::COMPOSITION,
            Resource::MEMBERS, *storage_controllers, controller_node);
        link_nodes(LinkType::ASSOCIATION,
            StorageController::TYPE, blade, controller_node, Resource::CONTAINED_BY);

        // populate JSON data
        json::Value json;
        json[Status::STATUS] = to_resource_status(controller_info.get_status()).as_json();
        json["Interface"] = controller_info.get_interface();
        auto drive_count = controller_info.get_drive_count();
        json["DriveCount"] = drive_count;
        json[Resource::OEM] = controller_info.get_oem_data().to_json_value();

        controller_node->get_resource().patch(json);

        build_drives(service, *controller_node, i, drive_count);
    }

    return storage_controllers;
}

NodeSharedPtr
ComputeNodeBuilder::build_drives(ComputeService& service, Node& storage_controller,
                             std::uint32_t sc_idx, std::uint32_t drive_count) {
    auto drives = std::make_shared<Drives>();
    link_nodes(LinkType::COMPOSITION,
            Drives::TYPE, storage_controller, drives);

    auto* blade = storage_controller.find_back_if([](const Node& n) {
        return n.get_type() == Blade::TYPE;
    });

    if (nullptr == blade) {
        throw std::runtime_error("Tree is not properly initialized.");
    }

    for (std::uint32_t i = 0; i < drive_count; ++i) {
        auto drive_info = service.get_drive_info(blade->get_uuid(), sc_idx, i);
        auto drive_node = std::make_shared<Drive>();
        link_nodes(LinkType::COMPOSITION,
                Resource::MEMBERS, *drives, drive_node);
        link_nodes(LinkType::ASSOCIATION,
                "", storage_controller, drive_node, Resource::CONTAINED_BY);

        // populate JSON data
        json::Value json;
        json[Status::STATUS] = to_resource_status(drive_info.get_status()).as_json();
        json["Interface"] = drive_info.get_interface();
        json["CapacityGB"] = drive_info.get_capacity_gb();
        json["Type"] = drive_info.get_type();
        json["RPM"] = drive_info.get_rpm();
        auto& fru_info = drive_info.get_fru_info();
        json["Manufacturer"] = fru_info.get_manufacturer();
        json["Model"] = fru_info.get_model_number();
        json["SerialNumber"] = fru_info.get_serial_number();
        json[Resource::OEM] = drive_info.get_oem_data().to_json_value();

        drive_node->get_resource().patch(json);
    }

    return drives;
}

NodeSharedPtr
ComputeNodeBuilder::build_processors(ComputeService& service,
              Node& blade, json::Value& blade_json, std::uint32_t slot_count) {
    // create blade's processors collection
    auto processors = std::make_shared<Processors>();
    link_nodes(LinkType::COMPOSITION,
            Processors::TYPE, blade,
            processors, Blade::TYPE);

    const auto& uid = blade.get_uuid();

    for (std::uint32_t i = 0; i < slot_count; ++i) {
        auto proc = std::make_shared<Processor>();
        link_nodes(LinkType::COMPOSITION,
                Resource::MEMBERS, *processors, proc);
        link_nodes(LinkType::ASSOCIATION,
                Processor::TYPE, blade,
                proc, Resource::CONTAINED_BY);

        auto proc_info = service.get_processor_info(uid, i);

        json::Value json;
        json[Status::STATUS] = to_resource_status(proc_info.get_status()).as_json();
        json[Resource::NAME] = "CPU" + std::to_string(i+1);
        json["Socket"] = proc_info.get_socket();
        json["Model"] = proc_info.get_model();
        json["Manufacturer"] = proc_info.get_manufacturer();
        json["ProcessorType"] = proc_info.get_processor_type();
        json["ProcessorArchitecture"] = proc_info.get_processor_architecture();
        // Code below resolves mismatch between REST API and GAMI specifications
        if (0 == proc_info.get_instruction_set().compare("x64")) {
            json["InstructionSet"] = "x86-64";
        } else {
            json["InstructionSet"] = proc_info.get_instruction_set();
        }
        json["ProcessorId"]["VendorId"] = proc_info.get_cpuid().get_vendor_id();
        json["ProcessorId"]["NumericId"] = proc_info.get_cpuid().get_numeric_id();
        json["ProcessorId"]["Family"] = proc_info.get_cpuid().get_family();
        json["ProcessorId"]["Model"] = proc_info.get_cpuid().get_model();
        json["ProcessorId"]["Step"] = proc_info.get_cpuid().get_step();
        json["ProcessorId"]["MicrocodeInfo"] = proc_info.get_cpuid().get_microcode_info();
        json["MaxSpeedMHz"] = proc_info.get_max_speed_mhz();
        json["TotalCores"] = proc_info.get_total_cores();
        json["EnabledCores"] = proc_info.get_enabled_cores();
        json["TotalThreads"] = proc_info.get_total_threads();
        json["EnabledThreads"] = proc_info.get_enabled_threads();
        json[Resource::OEM] = proc_info.get_oem_data().to_json_value();

        proc->get_resource().patch(json);
    }

    StatusWithRollup status_default(StateType::ABSENT,
                                    HealthType::OK,
                                    HealthType::OK);
    blade_json["Processors"][Status::STATUS] = status_default.as_json();

    blade_json["Processors"]["Count"] = unsigned(processors->size());
    blade_json["Processors"]["ProcessorSockets"] = unsigned(processors->size());
    auto it = processors->begin();
    if (it != processors->end()) {
        blade_json["Processors"]["ProcessorFamily"] = (*it).get_resource().as_json()["Model"];
        StatusWithRollup status(StateType::ENABLED, HealthType::OK, HealthType::OK);
        blade_json["Processors"][Status::STATUS] = status.as_json();
    }

    return processors;
}

NodeSharedPtr
ComputeNodeBuilder::build_memory(ComputeService& service,
        Node& blade, json::Value& blade_json,
        std::uint32_t slot_count) {
    // create blade's memory collection
    auto memory_modules = std::make_shared<MemoryModules>();
    link_nodes(LinkType::COMPOSITION,
            MemoryModules::TYPE, blade,
            memory_modules, Blade::TYPE);

    const auto& uid = blade.get_uuid();

    std::uint32_t total_mem_gb = 0;
    for (size_t i = 1; i <= slot_count; ++i) {
        auto memory = std::make_shared<MemoryModule>();
        link_nodes(LinkType::COMPOSITION,
                Resource::MEMBERS, *memory_modules, memory);
        link_nodes(LinkType::ASSOCIATION,
                MemoryModule::TYPE, blade,
                memory, Resource::CONTAINED_BY);

        auto mem_info = service.get_memory_info(uid, std::uint32_t(i));

        json::Value json;
        json[Status::STATUS] = to_resource_status(mem_info.get_status()).as_json();
        json["Socket"] = std::to_string(i); //mem_info.get_socket();
        json["Type"] = mem_info.get_type();
        json["SizeGB"] = mem_info.get_size_gb();
        json["SpeedMHz"] = mem_info.get_speed_mhz();
        json["VoltageVolt"] = mem_info.get_voltage_volt();
        json["FormFactor"] = mem_info.get_form_factor();
        json[Resource::OEM] = mem_info.get_oem_data().to_json_value();

        memory->get_resource().patch(json);

        total_mem_gb += mem_info.get_size_gb();
    }

    StatusWithRollup status(StateType::ENABLED, HealthType::OK, HealthType::OK);
    blade_json["Memory"][Status::STATUS] = status.as_json();
    blade_json["Memory"]["TotalSystemMemoryGB"] = total_mem_gb;
    blade_json["Memory"]["MemorySockets"] = slot_count;

    return memory_modules;
}

NodeSharedPtr
ComputeNodeBuilder::build_ethernet_interfaces(ComputeService& service,
                                              Node& blade,
                                              std::uint32_t count) {
    // create EthernetInterfaces
    auto nics = std::make_shared<EthernetInterfaces>();
    link_nodes(LinkType::COMPOSITION,
            EthernetInterfaces::TYPE, blade,
            nics, Blade::TYPE);

    for (size_t i = 0; i < count; ++i) {
        // create nic
        auto nic = std::make_shared<EthernetInterface>();
        link_nodes(LinkType::COMPOSITION,
                Resource::MEMBERS, *nics,
                nic, MemoryModules::TYPE);

        auto nic_info = service.get_network_interface_info(
                                            blade.get_uuid(), std::uint32_t(i));
        // populate JSON data
        json::Value json;
        json[Status::STATUS] = to_resource_status(nic_info.get_status()).as_json();
        json[Resource::DESCRIPTION] = "Management Network Interface";
        json["MacAddress"] = nic_info.get_mac_address();
//       json["LinkTechnology"] = nic_info.get_link_technology().empty()
//               ? "Unknown" : nic_info.get_link_technology();
        json["SpeedMbps"] = nic_info.get_speed();
        json["Autosense"] = nic_info.get_autosense();
        json["FrameSize"] = nic_info.get_frame_size();
        json["VLANEnable"] = nic_info.get_vlan_enable();
        auto ip4address_dto = nic_info.get_ipv4_address();
        json::Value ip4address;
        ip4address["Address"] = ip4address_dto.get_address();
        ip4address["SubnetMask"] = ip4address_dto.get_subnet_mask();
        ip4address["AddressOrigin"] = ip4address_dto.get_address_origin();
        ip4address["Gateway"] = ip4address_dto.get_gateway();

        json["IPv4Addresses"].push_back(std::move(ip4address));

        nic->get_resource().patch(json);
    }

    return nics;
}

void ComputeNodeBuilder::update_drawer_location(ComputeService& service,
                                             Node* drawer,
                                             const std::string& component) {
    auto response = service.get_chassis_info(component);
    auto resources = drawer->get_resource().as_json();
    resources[Location::LOCATION][Location::DRAWER] =
                                            response.get_location_offset();
    drawer->get_resource().patch(resources);
}
