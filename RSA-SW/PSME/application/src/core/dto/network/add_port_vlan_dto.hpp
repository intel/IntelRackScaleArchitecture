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
 * @file core/dto/network/add_port_vlan_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_ADD_PORT_VLAN_DTO_HPP
#define PSME_CORE_DTO_NETWORK_ADD_PORT_VLAN_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/oem_data_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Network namespace */
namespace network {

/*! VLAN Port request object */
class AddPortVlanDTO {
public:
    /*! Copy constructor */
    AddPortVlanDTO(const AddPortVlanDTO &) = default;

    /*! Assigment constructor */
    AddPortVlanDTO& operator=(const AddPortVlanDTO &) = default;

    /*! Default constructor */
    AddPortVlanDTO() {}

    /*! DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_port_identifier{};
        std::uint32_t m_vlan_id{};
        bool m_tagged{};
        OEMDataDTO::Request m_oem{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets component id
         *
         * @param[in] component Component id
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets component id
         *
         * @return Component id
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * Sets port identifier
         *
         * @param[in] port_identifier Port identifier
         * */
        void set_port_identifier(const std::string& port_identifier) {
            m_port_identifier = port_identifier;
        }

        /*!
         * Gets port identifier
         *
         * @return port identifier
         * */
        const std::string& get_port_identifier() const {
            return m_port_identifier;
        }

        /*!
         * Sets VLAN identifier
         *
         * @param[in] vlan_id VLAN identifier
         * */
        void set_vlan_id(std::uint32_t vlan_id) {
            m_vlan_id = vlan_id;
        }

        /*!
         * Gets VLAN identifier
         *
         * @return VLAN identifier
         * */
        std::uint32_t get_vlan_id() const {
            return m_vlan_id;
        }

        /*!
         * @brief Sets tagged VLAN flag
         *
         * @param tagged tagged VLAN flag
         */
        void set_tagged(bool tagged) {
            m_tagged = tagged;
        }

        /*!
         * @brief Gets tagged VLAN flag
         *
         * @return tagged VLAN flag
         */
        bool get_tagged() const {
            return m_tagged;
        }

        /*!
         * @brief Sets OEM data
         *
         * @param oem OEM data
         */
        void set_oem(const OEMDataDTO::Request& oem) {
            m_oem = oem;
        }

        /*!
         * @brief Gets OEM data
         *
         * @return OEM data
         */
        const OEMDataDTO::Request& get_oem() const {
            return m_oem;
        }

        virtual ~Request();
    };

    /*! DTO response */
    class Response : public ResponseDTO {
        std::string m_vlan_identifier{};
        OEMDataDTO::Response m_oem{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes VLAN port response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Sets VLAN identifier
         *
         * @param vlan_identifier VLAN identifier
         */
        void set_vlan_identifier(const std::string& vlan_identifier) {
            m_vlan_identifier = vlan_identifier;
        }

        /*!
         * @brief Gets VLAN identifier
         *
         * @return VLAN identifier
         */
        const std::string& get_vlan_identifier() const {
            return m_vlan_identifier;
        }

        /*!
         * @brief Sets oem
         *
         * @param oem oem
         */
        void set_oem(const OEMDataDTO::Response& oem) {
            m_oem = oem;
        }

        /*!
         * @brief Gets oem
         *
         * @return oem
         */
        const OEMDataDTO::Response& get_oem() const {
            return m_oem;
        }

        virtual ~Response();
    };

    ~AddPortVlanDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_ADD_PORT_VLAN_DTO_HPP */
