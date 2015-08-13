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

#include "network_service.hpp"
#include "logger/logger_factory.hpp"

using psme::core::dto::OEMDataDTO;
using psme::core::dto::network::SwitchInfoDTO;
using psme::core::dto::network::SwitchPortsIdDTO;
using psme::core::dto::network::SwitchPortInfoDTO;
using psme::core::dto::network::SetSwitchPortAttributesDTO;
using psme::core::dto::network::PortVlansIdDTO;
using psme::core::dto::network::PortVlanInfoDTO;
using psme::core::dto::network::AddPortVlanDTO;
using psme::core::dto::network::DeletePortVlanDTO;

using namespace psme::core::service;

NetworkService::NetworkService(psme::core::agent::AgentSharedPtr agent)
    : AgentService{agent} {}

SwitchInfoDTO::Response NetworkService::get_switch_info(
    const std::string& component) {

    SwitchInfoDTO::Request request_dto;
    request_dto.set_component(component);
    SwitchInfoDTO::Response response_dto;

    get_invoker().execute("getSwitchInfo", request_dto, response_dto);

    return response_dto;
}

std::vector<SwitchPortsIdDTO::PortIdentifier>
NetworkService::get_switch_ports_id(const std::string& component) {

    SwitchPortsIdDTO::Request request_dto;
    request_dto.set_component(component);
    SwitchPortsIdDTO::Response response_dto;

    get_invoker().execute("getSwitchPortsId", request_dto, response_dto);

    return response_dto.get_port_identifiers();
}

SwitchPortInfoDTO::Response NetworkService::get_switch_port_info(
    const std::string& component,
    const std::string& port_identifier) {
    SwitchPortInfoDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_port_identifier(port_identifier);
    SwitchPortInfoDTO::Response response_dto;

    get_invoker().execute("getSwitchPortInfo", request_dto, response_dto);

    return response_dto;
}

SetSwitchPortAttributesDTO::Response NetworkService::set_switch_port_attributes(
    const std::string& component, const std::string& port_identifier,
    std::uint32_t link_speed_gbps, const std::string& administrative_state,
    bool auto_sense, std::uint32_t frame_size, const OEMDataDTO::Request& oem) {
    SetSwitchPortAttributesDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_port_identifier(port_identifier);
    request_dto.set_link_speed_gbps(link_speed_gbps);
    request_dto.set_administrative_state(administrative_state);
    request_dto.set_auto_sense(auto_sense);
    request_dto.set_frame_size(frame_size);
    request_dto.set_oem(oem);
    SetSwitchPortAttributesDTO::Response response_dto;

    get_invoker().execute("setSwitchPortAttributes", request_dto, response_dto);

    return response_dto;
}

std::vector<PortVlansIdDTO::VlanIdentifier>
NetworkService::get_port_vlans_id(const std::string& component,
                                  const std::string& port_identifier) {
    PortVlansIdDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_port_identifier(port_identifier);
    PortVlansIdDTO::Response response_dto;

    get_invoker().execute("getPortVlansId", request_dto, response_dto);

    return response_dto.get_vlan_identifiers();
}

PortVlanInfoDTO::Response NetworkService::get_port_vlan_info(
    const std::string& component,
    const std::string& port_identifier,
    const std::string& vlan_identifier) {
    PortVlanInfoDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_port_identifier(port_identifier);
    request_dto.set_vlan_identifier(vlan_identifier);
    PortVlanInfoDTO::Response response_dto;

    get_invoker().execute("getPortVlanInfo", request_dto, response_dto);

    return response_dto;
}

AddPortVlanDTO::Response NetworkService::add_port_vlan(
    const std::string& component, const std::string& port_identifier,
    std::uint32_t vlan_id, bool tagged, const OEMDataDTO::Request& oem) {
    AddPortVlanDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_port_identifier(port_identifier);
    request_dto.set_vlan_id(vlan_id);
    request_dto.set_tagged(tagged);
    request_dto.set_oem(oem);
    AddPortVlanDTO::Response response_dto;

    get_invoker().execute("addPortVlan", request_dto, response_dto);

    return response_dto;
}

DeletePortVlanDTO::Response NetworkService::delete_port_vlan(
    const std::string& component, const std::string& port_identifier,
    const std::string& vlan_identifier, const OEMDataDTO::Request& oem) {
    DeletePortVlanDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_port_identifier(port_identifier);
    request_dto.set_vlan_identifier(vlan_identifier);
    request_dto.set_oem(oem);
    DeletePortVlanDTO::Response response_dto;

    get_invoker().execute("deletePortVlan", request_dto, response_dto);

    return response_dto;
}

using psme::core::dto::network::KnownSwitchesIdDTO;

KnownSwitchesIdDTO::Response
NetworkService::get_known_switches_id(const std::string& component) {
    KnownSwitchesIdDTO::Request request{};
    KnownSwitchesIdDTO::Response response{};

    request.set_component(component);

    get_invoker().execute("getKnownSwitchesId", request, response);

    return response;
}

using psme::core::dto::network::NeighborSwitchesIdDTO;

NeighborSwitchesIdDTO::Response
NetworkService::get_neighbor_switches_id(const std::string& component,
                                         const std::string& switch_identifier) {
    NeighborSwitchesIdDTO::Request request{};
    NeighborSwitchesIdDTO::Response response{};

    request.set_component(component);
    request.set_switch_identifier(switch_identifier);

    get_invoker().execute("getNeighborSwitchesId", request, response);

    return response;
}

using psme::core::dto::network::RemoteSwitchInfoDTO;

RemoteSwitchInfoDTO::Response
NetworkService::get_remote_switch_info(const std::string& component,
                                       const std::string& switch_identifier) {
    RemoteSwitchInfoDTO::Request request{};
    RemoteSwitchInfoDTO::Response response{};

    request.set_component(component);
    request.set_switch_identifier(switch_identifier);

    get_invoker().execute("getRemoteSwitchInfo", request, response);

    return response;
}
