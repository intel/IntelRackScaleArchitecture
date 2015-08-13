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

#include "psme/rest/node/builders/node_builder.hpp"
#include "psme/rest/node/crud/managers.hpp"
#include "psme/rest/node/crud/storage_managers.hpp"
#include "psme/rest/node/crud/ethernet_interfaces.hpp"
#include "psme/rest/node/crud/drawers.hpp"
#include "psme/rest/node/crud/fabric_modules.hpp"
#include "psme/rest/node/crud/network_service.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/resource/resource_definition.hpp"
#include "core/dto/manager_info_dto.hpp"

#include <cstring>

using namespace psme::rest::node;
using namespace psme::rest::resource;

namespace {
void
create_manager_nics(Node& manager,
        const psme::core::dto::ManagerInfoDTO::Response& manager_info) {
    // create nics
    auto nics = std::make_shared<EthernetInterfaces>();
    NodeBuilder::link_nodes(LinkType::COMPOSITION, Manager::SIMPLE_NETWORK, manager, nics);
    // create nic
    auto nic = std::make_shared<EthernetInterface>();
    NodeBuilder::link_nodes(LinkType::COMPOSITION, Resource::MEMBERS, *nics, nic);

    // populate JSON data
    auto& r = nic->get_resource();
    r.set_status({StateType::ENABLED, HealthType::OK});
    r.set_property("SpeedMbps", 100);
    json::Value ip4address = json::Value::Type::OBJECT;
    ip4address["Address"] = manager_info.get_ipv4_address();
    ip4address["SubnetMask"] = manager_info.get_ipv4_subnet_mask();
    json::Value ip4address_array = json::Value::Type::ARRAY;
    ip4address_array.push_back(std::move(ip4address));
    r.set_property("IPv4Addresses", std::move(ip4address_array));

    json::Value ipv4_mac_address = json::Value::Type::STRING;
    ipv4_mac_address = manager_info.get_ipv4_mac_address();
    r.set_property("MacAddress", std::move(ipv4_mac_address));
}

std::string get_manager_type(const Node& managed_node) {
    std::string manager_type = "BMC";
    if (0 == strcmp(Drawer::TYPE, managed_node.get_type())) {
        manager_type = "ManagementController";
    }
    else if (0 == strcmp(FabricModule::TYPE, managed_node.get_type())) {
        manager_type = "EnclosureManager";
    }
    return manager_type;
}
}

NodeBuilder::NodeBuilder() { }

NodeBuilder::~NodeBuilder() { }

void
NodeBuilder::link_nodes(LinkType link_type,
        const string& first_link_name,
        Node& first,
        NodeSharedPtr second,
        const string& second_link_name) {
    if (LinkType::COMPOSITION == link_type) {
        first.add_node(second);
    }
    first.add_link(first_link_name, *second, second_link_name);
}

NodeSharedPtr
NodeBuilder::build_manager(AgentService& service,
        const Node& managed_node, const std::string& uuid) {
    auto manager_info = service.get_manager_info(managed_node.get_uuid());
    return build_manager(manager_info, managed_node, uuid);
}

NodeSharedPtr
NodeBuilder::build_storage_manager(
        const psme::core::dto::ManagerInfoDTO::Response& manager_info) {

    auto storage_manager = std::make_shared<StorageManager>();

    auto network_service = std::make_shared<NetworkService>();
    link_nodes(LinkType::COMPOSITION, NetworkService::TYPE, *storage_manager, network_service);

    populate_networkservice(*network_service, manager_info.get_network_services());
    create_manager_nics(*storage_manager, manager_info);

    json::Value json;
    json[Status::STATUS] = Status(StateType::ENABLED, HealthType::OK).as_json();
    json["ManagerType"] = "ManagementController";
    json["Firmware"]["Current"]["VersionString"] = manager_info.get_firmware_version();
    json["GraphicalConsole"]["Enabled"] = false;
    json["SerialConsole"]["Enabled"] = false;
    json["CommandShell"]["Enabled"] = false;

    storage_manager->get_resource().patch(json);

    return storage_manager;
}

NodeSharedPtr
NodeBuilder::build_manager(const psme::core::dto::ManagerInfoDTO::Response& manager_info,
        const Node& managed_node, const std::string& uuid) {
    auto manager = std::make_shared<Manager>(uuid);
    auto network_service = std::make_shared<NetworkService>();
    link_nodes(LinkType::COMPOSITION, NetworkService::TYPE, *manager, network_service);

    populate_networkservice(*network_service, manager_info.get_network_services());
    create_manager_nics(*manager, manager_info);

    // populate JSON data
    json::Value json;
    Status status(StateType::ENABLED, HealthType::OK);
    json[Status::STATUS] = status.as_json();
    const psme::core::dto::SerialConsole& serial_console = manager_info.get_serial_console();
    json["SerialConsole"]["Enabled"] = serial_console.get_enabled();
    json["ManagerType"] = get_manager_type(managed_node);
    json["Firmware"]["Current"]["VersionString"] = manager_info.get_firmware_version();

    manager->get_resource().patch(json);

    return manager;
}

void
NodeBuilder::populate_networkservice(Node& ns,
                    const psme::core::dto::NetworkServices& network_services) {
    // populate JSON data
    json::Value json;
    Status status(StateType::ENABLED, HealthType::OK);
    json[Status::STATUS] = status.as_json();

    for (const auto& network_service : network_services) {
        const auto& service_name = network_service.get_name();
        json[service_name]["Enabled"] = network_service.get_enabled();
        json[service_name]["Port"] = network_service.get_port();
    }

    ns.get_resource().patch(json);
}

AgentNodeBuilder::~AgentNodeBuilder() { }
