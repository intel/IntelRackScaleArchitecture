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
 * @file core/dto/network/port_vlans_id_dto.hpo
 * @brief GetPortVlansId response transfer obejct declaration.
 *
 * */

#ifndef PSME_CORE_DTO_NETWORK_PORT_VLANS_ID_DTO_HPP
#define PSME_CORE_DTO_NETWORK_PORT_VLANS_ID_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include <string>

namespace Json {
class Value;
}

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {
/*! Network namespace */
namespace network {

/*! PortVLANsId transfer object */
class PortVlansIdDTO {
public:
    /*! Copy constructor */
    PortVlansIdDTO(const PortVlansIdDTO&) = default;

    /*! Assigment constructor */
    PortVlansIdDTO& operator=(const PortVlansIdDTO&) = default;

    /*! Default constructor */
    PortVlansIdDTO() {}

    /*! VlanIdentifier class */
    class VlanIdentifier {
    public:
        /*! Constructor */
        VlanIdentifier(const std::string& id)
            : m_id(id) {}

        /*!
         * @brief Set VLAN id
         * @param[in] id VLAN id
         */
        void set_id(const std::string& id) {
            m_id = id;
        }

        /*!
         * @brief Get VLAN id
         * @return VLAN id
         */
        const std::string& get_id() const {
            return m_id;
        }

    private:
        std::string m_id{};
    };

    /*! PortVlansId DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_port_identifier{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*!
         * @brief Sets switch UUID
         * @param[in] component Switch UUID
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * @brief Gets component UUID
         * @return Component UUID
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Sets port identifier
         * @param[in] port_identifier Port identifier
         * */
        void set_port_identifier(const std::string& port_identifier) {
            m_port_identifier = port_identifier;
        }

        /*!
         * @brief Gets port identifier
         * @return Port identifier
         * */
        const std::string& get_port_identifier() const {
            return m_port_identifier;
        }

        /*!
         * @brief Serializes object to JSON
         * @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        virtual ~Request();
    };

    class Response : public ResponseDTO {
        std::vector<VlanIdentifier> m_vlan_identifiers{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO() {}

        /*!
         * Desarializes collection data response object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Add VLAN identifier to collection
         * @param vlan_identifier VLAN identifier
         */
        void add_vlan_identifier(const VlanIdentifier& vlan_identifier) {
            m_vlan_identifiers.emplace_back(vlan_identifier);
        }

        /*!
         * @brief Set VLAN identifiers
         * @param vlan_identifiers VLAN identifiers array
         */
        void set_vlan_identifiers(
            const std::vector<VlanIdentifier>& vlan_identifiers) {
            m_vlan_identifiers = vlan_identifiers;
        }

        /*!
         * @brief Get VLAN identifiers
         * @return VLAN identifiers array
         */
        const std::vector<VlanIdentifier>& get_vlan_identifiers() const {
            return m_vlan_identifiers;
        }

        virtual ~Response();
    };

    virtual ~PortVlansIdDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_PORT_VLANS_ID_DTO_HPP */

