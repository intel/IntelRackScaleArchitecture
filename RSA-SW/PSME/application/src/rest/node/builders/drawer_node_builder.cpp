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

#include "psme/rest/node/builders/drawer_node_builder.hpp"
#include "psme/rest/node/crud/drawers.hpp"
#include "psme/rest/node/crud/managers.hpp"
#include "psme/rest/node/crud/compute_modules.hpp"
#include "psme/rest/node/crud/fabric_modules.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/resource/system_type.hpp"
#include "psme/rest/resource/chassis_type.hpp"
#include "psme/utils/network_interface_info.hpp"
#include "version.hpp"
#include <safe-string/safe_lib.hpp>
#include <exception>

using namespace psme::rest::node;
using namespace psme::rest::resource;
using namespace psme::rest::utils;
using namespace psme::utils;
using namespace psme::app;

namespace {
void
parse_url(string url, uint16_t& port, string& scheme) {
    size_t pos = 0;

    if (0 == url.find("http://")) {
        pos = strnlen_s("http://", RSIZE_MAX_STR);
        scheme = "HTTP";
    } else if (0 == url.find("https://")) {
        pos = strnlen_s("https://", RSIZE_MAX_STR);
        scheme = "HTTPS";
    }

    pos = url.find_first_of(':', pos);
    if (std::string::npos != pos) {
        size_t count;
        port = static_cast<uint16_t>(std::stoi(url.substr(pos + 1), &count));
    }
}

psme::core::dto::ManagerInfoDTO::Response
read_manager_info(const json::Value& config) {
    psme::core::dto::ManagerInfoDTO::Response manager_info;

    const auto& network_interface_name = config["server"]["network-interface-name"].as_string();
    NetworkInterfaceInfo networkInterfaceInfo(network_interface_name);
    try {
        const auto& network_interface_address = networkInterfaceInfo.get_interface_address();
        manager_info.set_ipv4_address(network_interface_address.get_ip_address());
        manager_info.set_ipv4_subnet_mask(network_interface_address.get_netmask());
        manager_info.set_ipv4_mac_address(network_interface_address.get_mac_address());
    } catch (const std::exception& ex) {
        log_error(GET_LOGGER("rest"),"Unable to read network address: "
                << ex.what());
    }

    uint16_t port = 80;
    string scheme = "HTTP";
    parse_url(config["server"]["url"].as_string(), port, scheme);

    psme::core::dto::NetworkService http_service;
    http_service.set_port(port);
    http_service.set_name(scheme);
    http_service.set_enabled(true);
    psme::core::dto::NetworkServices services{http_service};

    manager_info.set_network_services(services);
    manager_info.set_firmware_version(psme::app::Version::to_string());

    return manager_info;
}
}

DrawerNodeBuilder::~DrawerNodeBuilder() { }

NodesLinkVec
DrawerNodeBuilder::build_nodes(Node& root, const string& component_id) {
    bool is_storage_service =
                    m_config["rest-server"]["storage-service-mode"].as_bool();
    NodesLinkVec nodes_to_link;

    auto* v_node = root.get_next()->get_next();
    auto& drawer_collection = v_node->get_node_by_id(Drawers::TYPE);
    auto& manager_collection = v_node->get_node_by_id(Managers::TYPE);
    auto manager_info = read_manager_info(m_config);

    if (is_storage_service) {
        auto storage_manager = build_storage_manager(manager_info);
        nodes_to_link.emplace_back(LinkType::COMPOSITION,
            Resource::MEMBERS, manager_collection, storage_manager);
    } else {
        auto drawer = build_drawer(component_id);
        auto manager = build_manager(manager_info, *drawer);
        nodes_to_link.emplace_back(LinkType::COMPOSITION,
            Resource::MEMBERS, manager_collection, manager);
        nodes_to_link.emplace_back(LinkType::ASSOCIATION,
            Resource::MANAGED_BY, *drawer, manager, Manager::MANAGER_FOR_DRAWERS);
        nodes_to_link.emplace_back(LinkType::COMPOSITION,
            Resource::MEMBERS, drawer_collection, drawer);
    }

    return nodes_to_link;
}

NodeSharedPtr
DrawerNodeBuilder::build_drawer(const std::string& drawer_name) {
    // create drawer
    auto drawer = std::make_shared<Drawer>();
    // compute modules collection
    auto compute_modules = std::make_shared<ComputeModules>();
    NodeBuilder::link_nodes(LinkType::COMPOSITION,
            ComputeModules::TYPE, *drawer, compute_modules);
    // fabric modules collection
    auto fabric_modules = std::make_shared<FabricModules>();
    NodeBuilder::link_nodes(LinkType::COMPOSITION,
            FabricModules::TYPE, *drawer, fabric_modules);

    // populate JSON data
    auto& r = drawer->get_resource();
    r.set_property(Resource::NAME, drawer_name);
    r.set_property("ChassisType", to_string(ChassisType::SHELF));
    r.set_status({StateType::ENABLED, HealthType::OK});
    r.set_enumerated(EnumStatus::ENUMERATED);

    return drawer;
}
