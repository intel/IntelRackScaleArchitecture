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

#include "agent_service.hpp"
#include "logger/logger_factory.hpp"

using namespace psme::core::dto;
using namespace psme::core::service;

AgentService::AgentService(psme::core::agent::AgentSharedPtr agent) :
    m_agent{agent} {}

ComponentsDTO::Response
AgentService::get_components(const std::string& component) {
    ComponentsDTO::Request request_dto;
    request_dto.set_component_id(component);
    ComponentsDTO::Response response_dto;

    get_invoker().execute("getComponents", request_dto, response_dto);

    return response_dto;
}

ComponentCollectionDTO::Response
AgentService::get_component_collection() {
    ComponentCollectionDTO::Request request_dto;
    ComponentCollectionDTO::Response response_dto;

    get_invoker().execute("getComponentCollection", request_dto, response_dto);

    return response_dto;
}

std::vector<CollectionDTO::Subcomponent>
AgentService::get_collection(const std::string& component,
                             const std::string& name) {
    CollectionDTO::Request request_dto;
    request_dto.set_component(component);
    request_dto.set_name(name);
    CollectionDTO::Response response_dto;

    get_invoker().execute("getCollection", request_dto, response_dto);

    return response_dto.get_subcomponents();
}

void AgentService::set_component_attributes(const std::string& component,
                                            const std::vector<AttributeDTO>& attributes) {
    SetComponentAttributesDTO::Request request_dto;
    SetComponentAttributesDTO::Response response_dto;

    request_dto.set_component(component);
    request_dto.set_attributes(attributes);

    get_invoker().execute("setComponentAttributes", request_dto, response_dto);
}

psme::core::dto::ManagerInfoDTO::Response
AgentService::get_manager_info(const std::string& component) {

    ManagerInfoDTO::Request request_dto;
    request_dto.set_component_id(component);
    ManagerInfoDTO::Response response_dto;

    get_invoker().execute("getManagerInfo", request_dto, response_dto);

    return response_dto;
}
