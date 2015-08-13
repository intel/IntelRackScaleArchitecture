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
 * @file core/dto/network/switch_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_SWITCH_INFO_DTO_HPP
#define PSME_CORE_DTO_NETWORK_SWITCH_INFO_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/status_dto.hpp"
#include "core/dto/fru_info_dto.hpp"
#include "core/dto/oem_data_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Network namespace */
namespace network {

/*! Switch information request object */
class SwitchInfoDTO {
public:
    /*! Copy constructor */
    SwitchInfoDTO(const SwitchInfoDTO &) = default;

    /*! Assigment constructor */
    SwitchInfoDTO& operator=(const SwitchInfoDTO &) = default;

    /*! Default constructor */
    SwitchInfoDTO() {}

    /*! Switch information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets component UUID
         *
         * @param component Component UUID
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets component UUID
         *
         * @return Component UUID
         * */
        const std::string& get_component() const {
            return m_component;
        }

        virtual ~Request();
    };

    /*! Switch information DTO response */
    class Response : public ResponseDTO {
        StatusDTO::Response m_status{};
        std::string m_technology{};
        std::string m_mac_address{};
        FRUInfoDTO::Response m_fru_info{};
        OEMDataDTO::Response m_oem{};

    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes switch information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Sets component status
         *
         * @param status component status
         */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * @brief Gets component status
         *
         * @return component status
         */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * @brief Sets component technology
         *
         * @param technology component technology
         */
        void set_technology(const std::string& technology) {
            m_technology = technology;
        }

        /*!
         * @brief Gets component technology
         *
         * @return component technology
         */
        const std::string& get_technology() const {
            return m_technology;
        }
        /*!
         * @brief Sets MAC address
         *
         * @param mac_address MAC address
         */
        void set_mac_address(const std::string& mac_address) {
            m_mac_address = mac_address;
        }

        /*!
         * @brief Gets MAC address
         *
         * @return MAC address
         */
        const std::string& get_mac_address() const {
            return m_mac_address;
        }

        /*!
         * @brief Sets fru info
         *
         * @param fru_info fru info
         */
        void set_fru_info(const FRUInfoDTO::Response& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * @brief Gets fru info
         *
         * @return fru info
         */
        const FRUInfoDTO::Response& get_fru_info() const {
            return m_fru_info;
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

    ~SwitchInfoDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_SWITCH_INFO_DTO_HPP */
