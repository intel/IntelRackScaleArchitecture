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
 * @file core/service/network_service.hpp
 * @brief Network service declaration.
 * */

#ifndef PSME_CORE_SERVICE_NETWORK_SERVICE_HPP
#define PSME_CORE_SERVICE_NETWORK_SERVICE_HPP

#include "core/service/agent_service.hpp"
#include "core/dto/oem_data_dto.hpp"
#include "core/dto/network/switch_info_dto.hpp"
#include "core/dto/network/set_switch_port_attributes_dto.hpp"
#include "core/dto/network/switch_ports_id_dto.hpp"
#include "core/dto/network/switch_port_info_dto.hpp"
#include "core/dto/network/port_vlans_id_dto.hpp"
#include "core/dto/network/port_vlan_info_dto.hpp"
#include "core/dto/network/add_port_vlan_dto.hpp"
#include "core/dto/network/delete_port_vlan_dto.hpp"
#include "core/dto/network/known_switches_id_dto.hpp"
#include "core/dto/network/neighbor_switches_id_dto.hpp"
#include "core/dto/network/remote_switch_info_dto.hpp"

#include <vector>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Service namespace */
namespace service {

/*! Network service declaration */
class NetworkService : public AgentService {
public:
    /*! Default constructor */
    explicit NetworkService(psme::core::agent::AgentSharedPtr agent);

    /*!
     * @brief Execute switch information request
     * @param[in] component Switch UUID
     * @return SwitchInfo
     */
    dto::network::SwitchInfoDTO::Response
    get_switch_info(const std::string& component);

    /*!
     * @brief Execute GetSwitchPortsId request
     * @param[in] component Switch UUID
     * @return Port identifiers
     */
    std::vector<dto::network::SwitchPortsIdDTO::PortIdentifier>
    get_switch_ports_id(const std::string& component);

    /*!
     * @brief get_switch_port_info Execute switch port information request
     *
     * @return SwitchPortInfo response
     */
    psme::core::dto::network::SwitchPortInfoDTO::Response
    get_switch_port_info(const std::string& component,
                         const std::string& port_identifier);

    /*!
     * @brief Execute SetSwitchPortAttributes request
     *
     * @return SetSwitchPortAttributes response
     */
    psme::core::dto::network::SetSwitchPortAttributesDTO::Response
    set_switch_port_attributes(const std::string& component,
                               const std::string& port_identifier,
                               std::uint32_t linkSpeedGbps,
                               const std::string& administrative_state,
                               bool auto_sense, std::uint32_t frame_size,
                               const psme::core::dto::OEMDataDTO::Request& oem);

    /*!
     * @brief Execute GetPortVlansId request
     * @param[in] component Switch UUID
     * @param[in] port_identifier Port identifier
     * @return VLAN identifiers
     */
    std::vector<dto::network::PortVlansIdDTO::VlanIdentifier>
    get_port_vlans_id(const std::string& component,
                      const std::string& port_identifier);

    /*!
     * @brief Execute GetPortVlanInfo request
     *
     * @return PortVlanInfo response
     */
    psme::core::dto::network::PortVlanInfoDTO::Response
    get_port_vlan_info(const std::string& component,
                       const std::string& port_identifier,
                       const std::string& vlan_identifier);

    /*!
     * @brief Execute AddPortVlan request
     *
     * @return AddPortVlan response
     */
    psme::core::dto::network::AddPortVlanDTO::Response
    add_port_vlan(const std::string& component,
                  const std::string& port_identifier,
                  std::uint32_t vlan_id, bool tagged,
                  const psme::core::dto::OEMDataDTO::Request& oem);

    /*!
     * @brief Execute DeletePortVlan request
     *
     * @return DeletePortVlan response
     */
    psme::core::dto::network::DeletePortVlanDTO::Response
    delete_port_vlan(const std::string& component,
                     const std::string& port_identifier,
                     const std::string& vlan_identifier,
                     const psme::core::dto::OEMDataDTO::Request& oem);

    /*!
     * @brief Executes Get Known Switches Id command.
     * @param component uuid of the component.
     * @return Get Known Switches Id Response DTO object with filled data.
     */
    dto::network::KnownSwitchesIdDTO::Response
    get_known_switches_id(const std::string& component);

    /*!
     * @brief Executes Get Neighbor Switches Id command.
     * @param component uuid of the component.
     * @param switch_identifier switch identifiers. Origin: i.e KnownSwitchesId.
     * @return Get Neighbor Switches Id Response DTO object with filled data.
     */
    dto::network::NeighborSwitchesIdDTO::Response
    get_neighbor_switches_id(const std::string& component,
                             const std::string& switch_identifier);

    /*!
     * @brief Executes Get Remote Switch Info command.
     * @param component uuid of the component.
     * @param switch_identifier switch identifiers. Origin: i.e KnownSwitchesId.
     * @return Get Remote Switch Info Response DTO object with filled data.
     */
    dto::network::RemoteSwitchInfoDTO::Response
    get_remote_switch_info(const std::string& component,
                           const std::string& switch_identifier);




};

}
}
}

#endif /* PSME_CORE_SERVICE_NETWORK_SERVICE_HPP */

