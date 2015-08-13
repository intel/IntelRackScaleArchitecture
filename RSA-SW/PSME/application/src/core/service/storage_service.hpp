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
 * @file core/service/storage_service.hpp
 * @brief Storage service declaration.
 * */

#ifndef STORAGE_SERVICE_HPP
#define STORAGE_SERVICE_HPP

#include "core/service/agent_service.hpp"
#include "core/agent/invoker.hpp"
#include "core/dto/storage/storage_services_info_dto.hpp"
#include "core/dto/storage/physical_drive_info_dto.hpp"
#include "core/dto/storage/drives_dto.hpp"
#include "core/dto/storage/target_info_dto.hpp"
#include "core/dto/storage/delete_target_dto.hpp"
#include "core/dto/storage/add_target_dto.hpp"
#include "core/dto/storage/logical_drive_info_dto.hpp"
#include "core/dto/storage/add_logical_drive_dto.hpp"
#include "core/dto/storage/delete_logical_drive_dto.hpp"

#include <vector>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Service namespace */
namespace service {

/*! Storage service declaration */
class StorageService : public AgentService {
public:
    /*! Default constructor */
    explicit StorageService(psme::core::agent::AgentSharedPtr agent);

    /*!
     * @brief Execute GetStorageServicesInfo request
     * @param[in] services Services UUID
     * @return StorageServicesInfo response
     */
    psme::core::dto::storage::StorageServicesInfoDTO::Response
    get_storage_services_info(const std::string& services);

    /*!
     * @brief Execute GetDrives request
     * @param[in] component Component id
     * @return Drives response
     */
    psme::core::dto::storage::DrivesDTO::Response get_drives(
        const std::string& component);

    /*!
     * @brief Execute drive information request
     * @param[in] drive Drive UUID
     * @return PhysicalDriveInfo response
     */
    psme::core::dto::storage::PhysicalDriveInfoDTO::Response
    get_physical_drive_info(const std::string& drive);

    /*!
     * @brief Execute target information request
     * @param[in] target Target uuid
     * @return TargetInfo response
     */
    psme::core::dto::storage::TargetInfoDTO::Response get_target_info(
                                                    const std::string& target);
    /*!
     * @brief Execute delete target request
     * @param[in] target Target uuid
     * @return DeleteTarget response
     */
    psme::core::dto::storage::DeleteTargetDTO::Response delete_target(
                                                    const std::string& target);
    /*!
     * @brief Execute add target request
     * @param[in] request AddTarget request
     * @return AddTarget response
     */
    psme::core::dto::storage::AddTargetDTO::Response add_target(
                    psme::core::dto::storage::AddTargetDTO::Request& request);

    /*!
     * @brief Executes GetLogicalDriveInfo through JSON-RPC
     * @param[in] drive Drive's uuid
     * @return LogicalDriveInfo response
     */
    psme::core::dto::storage::LogicalDriveInfoDTO::Response get_logical_drive_info(
                        const std::string& drive);


    /*!
     * @brief Executes AddLogicalDrive through JSON-RPC
     * @param[in] request AddLogicalDrive request
     * @return AddLogicalDrive response
     */
    psme::core::dto::storage::AddLogicalDriveDTO::Response add_logical_drive(
                        psme::core::dto::storage::AddLogicalDriveDTO::Request& request);


    /*!
     * @brief Executes DeleteLogicalDrive through JSON-RPC
     * @param[in] drive UUID of the logical drive to remove
     * @return DeleteLogicalDrive response
     */
    psme::core::dto::storage::DeleteLogicalDriveDTO::Response delete_logical_drive(
                        const std::string& drive);

};
}
}
}
#endif /* STORAGE_SERVICE_HPP */

