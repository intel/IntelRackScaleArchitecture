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

#include "compute_service.hpp"
#include "logger/logger_factory.hpp"

using namespace psme::core::dto;
using namespace psme::core::service;

ComputeService::ComputeService(psme::core::agent::AgentSharedPtr agent) :
    AgentService{agent} {}

compute::MemoryInfoDTO::Response ComputeService::get_memory_info(
    const std::string& component, std::uint32_t socket) {
    compute::MemoryInfoDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_socket(socket);
    compute::MemoryInfoDTO::Response response_dto;

    get_invoker().execute("getMemoryInfo", request_dto, response_dto);

    return response_dto;
}

compute::ProcessorInfoDTO::Response
ComputeService::get_processor_info(
    const std::string& component, std::uint32_t socket) {
    compute::ProcessorInfoDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_socket(socket);
    compute::ProcessorInfoDTO::Response response_dto;

    get_invoker().execute("getProcessorInfo", request_dto, response_dto);

    return response_dto;
}

compute::StorageControllerInfoDTO::Response
ComputeService::get_storage_controller_info(
    const std::string& component, std::uint32_t controller) {
    compute::StorageControllerInfoDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_controller(controller);
    compute::StorageControllerInfoDTO::Response response_dto;

    get_invoker().execute("getStorageControllerInfo",
                          request_dto, response_dto);

    return response_dto;
}

compute::DriveInfoDTO::Response ComputeService::get_drive_info(
    const std::string& component, std::uint32_t controller,
    std::uint32_t drive) {
    compute::DriveInfoDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_controller(controller);
    request_dto.set_drive(drive);
    compute::DriveInfoDTO::Response response_dto;

    get_invoker().execute("getDriveInfo", request_dto, response_dto);

    return response_dto;
}

compute::ModuleInfoDTO::Response ComputeService::get_module_info(
    const std::string& component) {
    compute::ModuleInfoDTO::Request request_dto;
    request_dto.set_component(component);
    compute::ModuleInfoDTO::Response response_dto;

    get_invoker().execute("getModuleInfo", request_dto, response_dto);

    return response_dto;
}

compute::BladeInfoDTO::Response ComputeService::get_blade_info(
                                            const std::string& component) {
    compute::BladeInfoDTO::Request request_dto;
    request_dto.set_component(component);
    compute::BladeInfoDTO::Response response_dto;

    get_invoker().execute("getBladeInfo", request_dto, response_dto);

    return response_dto;
}

compute::NetworkInterfaceInfoDTO::Response
ComputeService::get_network_interface_info(
    const std::string& component, std::uint32_t interface) {
    compute::NetworkInterfaceInfoDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_interface(interface);
    compute::NetworkInterfaceInfoDTO::Response response_dto;

    get_invoker().execute("getNetworkInterfaceInfo",
                          request_dto, response_dto);

    return response_dto;
}

compute::BladeAttributesDTO::Response
ComputeService::set_blade_attributes(
                            compute::BladeAttributesDTO::Request& request) {
    compute::BladeAttributesDTO::Response response;

    get_invoker().execute("setBladeAttributes", request, response);

    return response;
}

compute::ComputeInfoDTO::Response
ComputeService::get_compute_info(const std::string& compute) {
    compute::ComputeInfoDTO::Request request;
    compute::ComputeInfoDTO::Response response;

    request.set_compute(compute);

    get_invoker().execute("getComputeInfo", request, response);
    return response;
}

compute::ComputeZoneInfoDTO::Response
ComputeService::get_compute_zone_info(const std::string& zone) {
    compute::ComputeZoneInfoDTO::Request request;
    compute::ComputeZoneInfoDTO::Response response;

    request.set_zone(zone);

    get_invoker().execute("getComputeZoneInfo", request, response);
    return response;
}

psme::core::dto::compute::ChassisInfoDTO::Response
ComputeService::get_chassis_info(const std::string& chassis) {
    compute::ChassisInfoDTO::Request request;
    compute::ChassisInfoDTO::Response response;

    request.set_chassis(chassis);

    get_invoker().execute("getChassisInfo", request, response);
    return response;

}
