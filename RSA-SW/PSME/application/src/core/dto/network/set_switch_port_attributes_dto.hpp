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
 * @file core/dto/network/set_swtich_port_attributes_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_SET_SWITCH_PORT_ATTRIBUTES_DTO_HPP
#define PSME_CORE_DTO_NETWORK_SET_SWITCH_PORT_ATTRIBUTES_DTO_HPP

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

/*! Switch port attributes request object */
class SetSwitchPortAttributesDTO {
public:
    /*! Copy constructor */
    SetSwitchPortAttributesDTO(const SetSwitchPortAttributesDTO&) = default;

    /*! Assigment constructor */
    SetSwitchPortAttributesDTO& operator=(const SetSwitchPortAttributesDTO&) = default;

    /*! Default constructor */
    SetSwitchPortAttributesDTO() {}

    /*! DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_port_identifier{};
        std::uint32_t m_link_speed_gbps{};
        std::string m_administrative_state{};
        bool m_auto_sense{};
        std::uint32_t m_frame_size{};
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
         * @brief Sets link speed Gbps
         *
         * @param link_speed_gbps link speed Gbps
         */
        void set_link_speed_gbps(uint32_t link_speed_gbps) {
            m_link_speed_gbps = link_speed_gbps;
        }

        /*!
         * @brief Gets link speed Gbps
         *
         * @return link speed Gbps
         */
        uint32_t get_link_speed_gbps() const {
            return m_link_speed_gbps;
        }

        /*!
         * @brief Sets administrative state
         *
         * @param administrative_state Link state
         */
        void set_administrative_state(const std::string& administrative_state) {
            m_administrative_state = administrative_state;
        }

        /*!
         * @brief Gets administrative state
         *
         * @return Administrative state
         */
        const std::string& get_administrative_state() const {
            return m_administrative_state;
        }

        /*!
         * @brief Sets auto sense
         *
         * @param auto_sense Auto sense
         */
        void set_auto_sense(bool auto_sense) {
            m_auto_sense = auto_sense;
        }

        /*!
         * @brief Gets auto sense
         *
         * @return Auto sense
         */
        bool get_auto_sense() const {
            return m_auto_sense;
        }

        /*!
         * @brief Sets frame size
         *
         * @param frame_size frame size
         */
        void set_frame_size(uint32_t frame_size) {
            m_frame_size = frame_size;
        }

        /*!
         * @brief Gets frame size
         *
         * @return frame size
         */
        uint32_t get_frame_size() const {
            return m_frame_size;
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

    /*! Switch attributes DTO response */
    class Response : public ResponseDTO {
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

        virtual ~Response();
    };

    ~SetSwitchPortAttributesDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_SET_SWITCH_PORT_ATTRIBUTES_DTO_HPP */
