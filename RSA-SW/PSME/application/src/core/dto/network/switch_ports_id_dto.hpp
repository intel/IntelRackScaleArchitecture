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
 * @file core/dto/network/switch_ports_id_dto.hpo
 * @brief GetSwitchPortsId response transfer obejct declaration.
 *
 * */

#ifndef PSME_CORE_DTO_NETWORK_SWITCH_PORTS_ID_DTO_HPP
#define PSME_CORE_DTO_NETWORK_SWITCH_PORTS_ID_DTO_HPP

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

/*! SwitchPortsId transfer object */
class SwitchPortsIdDTO {
public:
    /*! Copy constructor */
    SwitchPortsIdDTO(const SwitchPortsIdDTO&) = default;

    /*! Assigment constructor */
    SwitchPortsIdDTO& operator=(const SwitchPortsIdDTO&) = default;

    /*! Default constructor */
    SwitchPortsIdDTO() {}

    /*! PortIdentifier class */
    class PortIdentifier {
    public:
        /*! Constructor */
        PortIdentifier(const std::string& id)
            : m_id(id) {}

        /*!
         * @brief Set port id
         * @param[in] id Port id
         */
        void set_id(const std::string& id) {
            m_id = id;
        }

        /*!
         * @brief Get port id
         * @return Port id
         */
        const std::string& get_id() const {
            return m_id;
        }

    private:
        std::string m_id{};
    };

    /*! SwitchPortsId DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
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
         * @brief Serializes object to JSON
         * @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        virtual ~Request();
    };

    class Response : public ResponseDTO {
        std::vector<PortIdentifier> m_port_identifiers{};
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
         * @brief Add port identifier to collection
         * @param port_identifier Port identifier
         */
        void add_port_identifier(const PortIdentifier& port_identifier) {
            m_port_identifiers.emplace_back(port_identifier);
        }

        /*!
         * @brief Set port identifiers
         * @param port_identifiers Port identifiers array
         */
        void set_port_identifiers(
            const std::vector<PortIdentifier>& port_identifiers) {
            m_port_identifiers = port_identifiers;
        }

        /*!
         * @brief Get port identifiers
         * @return Port identifiers array
         */
        const std::vector<PortIdentifier>& get_port_identifiers() const {
            return m_port_identifiers;
        }

        virtual ~Response();
    };

    virtual ~SwitchPortsIdDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_SWITCH_PORTS_ID_DTO_HPP */

