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

#include "psme/rest/node/builders/network_node_builder.hpp"
#include "psme/rest/node/builders/builder_util.hpp"
#include "psme/rest/node/crud/drawers.hpp"
#include "psme/rest/node/crud/managers.hpp"
#include "psme/rest/node/crud/fabric_modules.hpp"
#include "psme/rest/node/crud/switches.hpp"
#include "psme/rest/node/crud/switch_ports.hpp"
#include "psme/rest/node/crud/vlans.hpp"
#include "psme/rest/resource/resource.hpp"
#include "core/service/service_factory.hpp"
#include "core/service/network_service.hpp"
#include "core/dto/component_dto.hpp"
#include "core/dto/manager_info_dto.hpp"

using namespace psme::rest::node;
using namespace psme::rest::resource;
using namespace psme::rest::utils;
using namespace psme::core::dto;

using psme::core::service::ServiceFactory;

NetworkNodeBuilder::~NetworkNodeBuilder() { }

NodesLinkVec
NetworkNodeBuilder::build_nodes(Node& root, const string& component_id) {
    if (nullptr == root.get_next() || nullptr == root.get_next()->get_next()) {
        throw std::runtime_error("Tree is not properly initialized.");
    }
    auto* v_node = root.get_next()->get_next();
    auto* drawer = v_node->get_node_by_id(Drawers::TYPE).get_next();

    if (nullptr == drawer) {
        throw std::runtime_error("Tree is not properly initialized.");
    }

    auto& manager_collection = v_node->get_node_by_id(Managers::TYPE);

    auto service = ServiceFactory::create_network(get_agent()->get_gami_id());

    NodesLinkVec nodes_to_link;
    auto& fabric_modules = drawer->get_node_by_id(FabricModules::TYPE);

    auto components = service.get_components(component_id).get_components();
    for (const auto& component : components) {
        auto fabric_module = build_fabric_module(service, component.get_name());
        nodes_to_link.emplace_back(LinkType::COMPOSITION,
                Resource::MEMBERS, fabric_modules, fabric_module);
        nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                "", *drawer, fabric_module, Resource::CONTAINED_BY);
        // fabric module's manager
        auto manager = build_manager(service, *fabric_module);
        nodes_to_link.emplace_back(LinkType::COMPOSITION,
                Resource::MEMBERS, manager_collection, manager);
        nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                Manager::MANAGER_FOR_FABRICMODULES, *manager, fabric_module, Resource::MANAGED_BY);
        auto& switch_collection = fabric_module->get_node_by_id(Switches::TYPE);
        for (const auto& subcomponent : component.get_subcomponents()) {
            auto switch_node = build_switch(service, subcomponent.get_name());
            nodes_to_link.emplace_back(LinkType::COMPOSITION,
                    Resource::MEMBERS, switch_collection, switch_node);
            // switch manager is same as fabric module's one
            nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                   "", *manager, switch_node, Resource::MANAGED_BY);
            // link switch with fabric module
            nodes_to_link.emplace_back(LinkType::ASSOCIATION,
                    "", *fabric_module, switch_node, Resource::CONTAINED_BY);
        }
    }
    return nodes_to_link;
}

NodeSharedPtr
NetworkNodeBuilder::build_fabric_module(
            NetworkService& /*service*/, const string& uuid) {
    auto fabric_module = std::make_shared<FabricModule>(uuid);
    // create switch collection
    auto switches = std::make_shared<Switches>();
    link_nodes(LinkType::COMPOSITION,
            Switches::TYPE, *fabric_module, switches, Resource::CONTAINED_BY);

    // populate fabric module JSON
    json::Value json;
    Status status;
    status.set_state(StateType::ENABLED);
    status.set_health(HealthType::OK);
    json[Status::STATUS] = status.as_json();

    fabric_module->get_resource().patch(json);

    return fabric_module;
}

NodeSharedPtr
NetworkNodeBuilder::build_switch(NetworkService& service, const string& uuid) {

    auto switch_node = std::make_shared<Switch>(uuid, get_agent()->get_gami_id());
    auto switch_info = service.get_switch_info(uuid);

    // populate JSON
    json::Value content;
    content[Status::STATUS] = to_resource_status(switch_info.get_status()).as_json();
    content[Resource::OEM] = switch_info.get_oem().to_json_value();

    switch_node->get_resource().patch(content);

    build_switch_ports(service, *switch_node);

    return switch_node;
}

NodeSharedPtr
NetworkNodeBuilder::build_switch_ports(NetworkService& service,
                                       Node& switch_node) {
    // switch port collection
    auto switch_ports = std::make_shared<SwitchPorts>();
    link_nodes(LinkType::COMPOSITION,
            SwitchPorts::TYPE, switch_node, switch_ports);

    const auto& switch_uuid = switch_node.get_uuid();
    const auto port_identifiers = service.get_switch_ports_id(switch_uuid);

    for (const auto& port_identifier : port_identifiers) {
        auto switch_port_info =
            service.get_switch_port_info(switch_uuid, port_identifier.get_id());

        const auto& port_id_string = port_identifier.get_id();


        auto switch_port = std::make_shared<SwitchPort>(
                        port_identifier.get_id(), get_agent()->get_gami_id());
        link_nodes(LinkType::COMPOSITION,
                Resource::MEMBERS, *switch_ports, switch_port);
        link_nodes(LinkType::ASSOCIATION,
                "", switch_node, switch_port, Resource::CONTAINED_BY);

        json::Value json;

        json["LinkType"] = switch_port_info.get_link_technology().empty()
                ? "Unknown" : switch_port_info.get_link_technology();
        json[Status::STATUS]
                = to_resource_status(switch_port_info.get_status()).as_json();
        json[SwitchPort::ADMINISTRATIVE_STATE]
                = switch_port_info.get_administrative_state();
        json[SwitchPort::OPERATIONAL_STATE   ]
                = switch_port_info.get_operational_state();
        json[SwitchPort::LINK_SPEED_GBPS     ]
                = switch_port_info.get_link_speed_gbps();
        json[SwitchPort::NEIGHBOUR_PORT      ]
                = find_neighbor(service, switch_uuid, port_id_string);

        json[Resource::OEM] = switch_port_info.get_oem().to_json_value();
        json[Resource::NAME] = port_id_string;

        switch_port->get_resource().patch(json);

        build_vlans(service, *switch_port, port_identifier.get_id());
    }

    return switch_ports;
}

const std::string
NetworkNodeBuilder::find_neighbor(NetworkService& service,
                                  const std::string& component,
                                  const std::string& port_identifier) {

    auto known_switches = service.get_known_switches_id(component);
    const auto& switches_ids = known_switches.get_switch_identifiers();

    for (const auto& switch_id : switches_ids) {

        const auto& port_read_from_remote = get_port_identifier(service,
                                                                component,
                                                                switch_id);
        if (port_identifier == port_read_from_remote) {
            return (switch_id + ":");
        }
    }
    return "";
}

const std::string
NetworkNodeBuilder::get_port_identifier(NetworkService& service,
                                        const std::string& component,
                                        const std::string& switch_id) {
    auto switch_info = service.get_remote_switch_info(component, switch_id);
    const auto& next_hop = switch_info.get_next_hop();
    return next_hop.get_port_identifier();
}


NodeSharedPtr
NetworkNodeBuilder::build_vlans(NetworkService& service, Node& switch_port,
                                const string& port_identifier) {
    // vlan collection
    auto vlans = std::make_shared<Vlans>("", get_agent()->get_gami_id());
    link_nodes(LinkType::COMPOSITION, Vlans::TYPE, switch_port, vlans);

    const auto& switch_uuid = switch_port.get_back()->get_back()->get_uuid();
    const auto vlan_identifiers =
        service.get_port_vlans_id(switch_uuid, port_identifier);

    for (const auto& vlan_identifier : vlan_identifiers) {
        auto port_vlan_info =
            service.get_port_vlan_info(switch_uuid, port_identifier,
                                       vlan_identifier.get_id());
        auto vlan = std::make_shared<Vlan>(vlan_identifier.get_id(),
                                           get_agent()->get_gami_id());
        link_nodes(LinkType::COMPOSITION,
                Resource::MEMBERS, *vlans, vlan);
        // todo: how to match NIC
        //link_nodes(LinkType::ASSOCIATION, NIC, vlan);
        // populate vlan JSON data
        json::Value json;
        json[Status::STATUS] = to_resource_status(port_vlan_info.get_status()).as_json();
        json["VLANId"] = port_vlan_info.get_vlan_id();

        vlan->get_resource().patch(json);
    }
    return vlans;
}
