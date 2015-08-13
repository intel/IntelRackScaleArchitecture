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
 * @file core/service/compute_service.hpp
 * @brief Compute service declaration.
 * */

#ifndef COMPUTE_SERVICE_HPP
#define COMPUTE_SERVICE_HPP

#include "core/service/agent_service.hpp"
#include "core/dto/compute/processor_info_dto.hpp"
#include "core/dto/compute/memory_info_dto.hpp"
#include "core/dto/compute/drive_info_dto.hpp"
#include "core/dto/compute/storage_controller_info_dto.hpp"
#include "core/dto/compute/network_interface_info_dto.hpp"
#include "core/dto/compute/module_info_dto.hpp"
#include "core/dto/compute/blade_info_dto.hpp"
#include "core/dto/compute/blade_attributes_dto.hpp"
#include "core/dto/compute/compute_info_dto.hpp"
#include "core/dto/compute/compute_zone_info_dto.hpp"
#include "core/dto/compute/chassis_info_dto.hpp"

#include <vector>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {

/*! Service namespace */
namespace service {

/*! Compute service declaration */
class ComputeService : public AgentService {
public:
    /*! Default constructor */
    explicit ComputeService(psme::core::agent::AgentSharedPtr agent);

    /*!
     * @brief get_memory_info Execute memory information request
     *
     * @return MemoryInfo response
     */
    psme::core::dto::compute::MemoryInfoDTO::Response get_memory_info(
                        const std::string& component, std::uint32_t socket);

    /*!
     * @brief get_processor_info Execute processor information request
     *
     * @return Processor information response
     */
    psme::core::dto::compute::ProcessorInfoDTO::Response get_processor_info(
                             const std::string& component, std::uint32_t socket);

    /*!
     * @brief Execute storage controller information request
     *
     * @return StorageControllerInfo response
     */
    psme::core::dto::compute::StorageControllerInfoDTO::Response
    get_storage_controller_info(
        const std::string& component, std::uint32_t controller);

    /*!
     * @brief get_drive_info Execute drive information request
     *
     * @return DriveInfo response
     */
    psme::core::dto::compute::DriveInfoDTO::Response get_drive_info(
        const std::string& component,
        std::uint32_t controller,
        std::uint32_t drive);

    /*!
     * @brief get_module_info Execute module information request
     *
     * @return ModuleInfo response
     */
    psme::core::dto::compute::ModuleInfoDTO::Response get_module_info(
                        const std::string& component);

    /*!
     * @brief get_blade_info Execute blade information request
     *
     * @return BladeInfo response
     */
    psme::core::dto::compute::BladeInfoDTO::Response get_blade_info(
                                                const std::string& component);

    /*!
     * @brief Execute network interface information request
     *
     * @return NetworkInterfaceInfo response
     */
    psme::core::dto::compute::NetworkInterfaceInfoDTO::Response
    get_network_interface_info(
        const std::string& component, std::uint32_t interface);

    /*!
     * @brief Execute Set blade attributes request to Compute Module.
     * @param request Blade Attributes request
     * @return response, empty object. Set blade attributes has no response.
     */
    psme::core::dto::compute::BladeAttributesDTO::Response
    set_blade_attributes(
            psme::core::dto::compute::BladeAttributesDTO::Request& request);

    /*!
     * @brief Execute get compute info request
     * @param compute Compute uuid
     * @return ComputeInfo response
     */
    psme::core::dto::compute::ComputeInfoDTO::Response
    get_compute_info(const std::string& compute);

    /*!
     * @brief Execute get compute zone info request
     * @param zone Compute zone uuid
     * @return ComputeZoneInfo response
     */
    psme::core::dto::compute::ComputeZoneInfoDTO::Response
    get_compute_zone_info(const std::string& zone);

    /*!
     * @brief Execute get chassis info request
     * @param chassis Chassis uuid
     * @return ChassisInfo response
     */
    psme::core::dto::compute::ChassisInfoDTO::Response
    get_chassis_info(const std::string& chassis);
};

}
}
}

#endif /* COMPUTE_SERVICE_HPP */

