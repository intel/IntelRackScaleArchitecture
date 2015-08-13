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

#include "storage_service.hpp"
#include "logger/logger_factory.hpp"

using namespace psme::core::dto;
using namespace psme::core::service;

StorageService::StorageService(psme::core::agent::AgentSharedPtr agent) :
    AgentService{agent} {}

storage::StorageServicesInfoDTO::Response
StorageService::get_storage_services_info(const std::string& services) {
    storage::StorageServicesInfoDTO::Request request_dto;
    request_dto.set_services(services);
    storage::StorageServicesInfoDTO::Response response_dto;

    get_invoker().execute("getStorageServicesInfo", request_dto, response_dto);

    return response_dto;
}

storage::DrivesDTO::Response StorageService::get_drives(
    const std::string& component) {
    storage::DrivesDTO::Request request_dto;
    request_dto.set_component(component);
    storage::DrivesDTO::Response response_dto;

    get_invoker().execute("getDrives", request_dto, response_dto);

    return response_dto;
}

storage::PhysicalDriveInfoDTO::Response
StorageService::get_physical_drive_info(const std::string& drive) {
    storage::PhysicalDriveInfoDTO::Request request_dto;
    request_dto.set_drive(drive);
    storage::PhysicalDriveInfoDTO::Response response_dto;

    get_invoker().execute("getPhysicalDriveInfo", request_dto, response_dto);

    return response_dto;
}

storage::TargetInfoDTO::Response
StorageService::get_target_info(const std::string& target) {
    storage::TargetInfoDTO::Request request_dto;
    request_dto.set_target(target);
    storage::TargetInfoDTO::Response response_dto;

    get_invoker().execute("getiSCSITargetInfo", request_dto, response_dto);

    return response_dto;
}

storage::DeleteTargetDTO::Response
StorageService::delete_target(const std::string& target) {
    storage::DeleteTargetDTO::Request request_dto;
    request_dto.set_target(target);
    storage::DeleteTargetDTO::Response response_dto;

    get_invoker().execute("deleteiSCSITarget", request_dto, response_dto);

    return response_dto;
}

storage::AddTargetDTO::Response
StorageService::add_target(storage::AddTargetDTO::Request& request_dto) {
    storage::AddTargetDTO::Response response_dto;

    get_invoker().execute("addiSCSITarget", request_dto, response_dto);

    return response_dto;
}

psme::core::dto::storage::LogicalDriveInfoDTO::Response
StorageService::get_logical_drive_info(const std::string& drive) {
    storage::LogicalDriveInfoDTO::Request request_dto;
    storage::LogicalDriveInfoDTO::Response response_dto;

    request_dto.set_drive(drive);

    get_invoker().execute("getLogicalDriveInfo", request_dto, response_dto);

    return response_dto;
}

storage::AddLogicalDriveDTO::Response
StorageService::add_logical_drive(storage::AddLogicalDriveDTO::Request& request_dto){
    storage::AddLogicalDriveDTO::Response response_dto;

    get_invoker().execute("addLogicalDrive", request_dto, response_dto);

    return response_dto;

}

storage::DeleteLogicalDriveDTO::Response
StorageService::delete_logical_drive(const std::string& drive){
storage::DeleteLogicalDriveDTO::Request request_dto;
storage::DeleteLogicalDriveDTO::Response response_dto;

request_dto.set_drive(drive);

get_invoker().execute("deleteLogicalDrive", request_dto, response_dto);

return response_dto;
}

