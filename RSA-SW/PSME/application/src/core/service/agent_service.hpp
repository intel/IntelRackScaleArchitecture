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
 * @file core/service/agent_service.hpp
 * @brief Base agent service declaration.
 * */

#ifndef AGENT_SERVICE_HPP
#define AGENT_SERVICE_HPP

#include "core/agent/agent.hpp"
#include "core/agent/invoker.hpp"
#include "core/dto/components_dto.hpp"
#include "core/dto/manager_info_dto.hpp"
#include "core/dto/component_collection_dto.hpp"
#include "core/dto/collection_dto.hpp"
#include "core/dto/set_component_attributes_dto.hpp"

#include <vector>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Service namespace */
namespace service {

/*! Agent service declaration */
class AgentService {
protected:
    /*! Default constructor */
    explicit AgentService(psme::core::agent::AgentSharedPtr agent);

    /*!
     * @brief Get commands invoker
     *
     * @return Invoker
     * */
    psme::core::agent::Invoker& get_invoker() const {
        return m_agent->get_invoker();
    }

public:
    /*!
     * @brief Get reference to agent this service is talking to.
     *
     * @return Reference to Agent
     */
    const psme::core::agent::Agent& get_agent() const {
        return *m_agent;
    }

    /*!
     * @brief Execute JSON-RPC getComponents call
     *
     * @return Components structure respnse
     */
    psme::core::dto::ComponentsDTO::Response
    get_components(const std::string& component = "");

    /*!
     * @brief Execute JSON-RPC getComponentCollection call
     *
     * @return Component collection response
     */
    psme::core::dto::ComponentCollectionDTO::Response
    get_component_collection();

    /*!
     * @brief Execute JSON-RPC getCollection call
     *
     * @return Collection response
     */
    std::vector<psme::core::dto::CollectionDTO::Subcomponent>
    get_collection(const std::string& component, const std::string& name);

    /*!
     * @brief Execute JSON-RPC setComponentAttributes call
     * @param[in] component Component UUID
     * @param[in] attributes Attributes
     */
    void set_component_attributes(const std::string& component,
                                  const std::vector<dto::AttributeDTO>& attributes);

    /*!
     * @brief get_manager_info Execute manager information request
     *
     * @return ManagerInfo response
     */
    psme::core::dto::ManagerInfoDTO::Response get_manager_info(
                                                const std::string& component);

private:
    psme::core::agent::AgentSharedPtr m_agent;
};

}
}
}

#endif /* COMPUTE_SERVICE_HPP */

