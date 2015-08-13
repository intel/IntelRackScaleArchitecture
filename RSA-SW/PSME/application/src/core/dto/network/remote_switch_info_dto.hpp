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
 * @file core/dto/network/remote_switch_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_REMOTE_SWITCH_INFO_DTO_HPP
#define PSME_CORE_DTO_NETWORK_REMOTE_SWITCH_INFO_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/network/network_hop_dto.hpp"
#include "core/dto/oem_data_dto.hpp"
#include "core/dto/status_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Network namespace */
namespace network {

/*! Remote Switch Info information DTO object */
class RemoteSwitchInfoDTO {
public:
    /*! Copy constructor */
    RemoteSwitchInfoDTO(const RemoteSwitchInfoDTO &) = default;

    /*! Assigment constructor */
    RemoteSwitchInfoDTO& operator=(const RemoteSwitchInfoDTO &) = default;

    /*! Default constructor */
    RemoteSwitchInfoDTO() {}

    /*! Destructor */
    ~RemoteSwitchInfoDTO();

    /*! Known Switches ID information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_switch_identifier{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets switch UUID.
         *
         * @param component Switch UUID
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets switch UUID.
         *
         * @return Switch UUID
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * Sets switch identifier.
         *
         * @param id Switch identifier.
         * */
        void set_switch_identifier(const std::string& id) {
            m_switch_identifier = id;
        }

        /*!
         * Gets switch identifier.
         *
         * @return Switch identifier.
         * */
        const std::string& get_switch_identifier() const {
            return m_switch_identifier;
        }

        virtual ~Request();
    };

    /*! Remote Switch Info information DTO object */
    class Response : public ResponseDTO {
        StatusDTO::Response m_status{};
        std::string m_mac_address{};
        NetworkHopDTO::Response m_next_hop{};
        OEMDataDTO::Response m_oem{};

    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Deserialize information response from JSON to object.
         *
         * @param data JSON to be deserialized to object.
         * */
        void to_object(const Json::Value& data);

        /*!
         * @brief Sets status.
         * @param status Status DTO.
         */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * @brief Gets Status DTO.
         * @return status DTO.
         */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * @brief Sets MAC address.
         * @param mac string with MAC address.
         */
        void set_mac_address(const std::string& mac) {
            m_mac_address = mac;
        }

        /*!
         * @brief Gets MAC address.
         * @return string with MAC address.
         */
        const std::string& get_mac_address() const {
            return m_mac_address;
        }

        /*!
         * @brief Sets next hop DTO..
         * @param hop NextHop DTO.
         */
        void set_next_hop(const NetworkHopDTO::Response& hop) {
            m_next_hop = hop;
        }

        /*!
         * @brief Gets next hop DTO..
         * @return NextHop DTO.
         */
        const NetworkHopDTO::Response& get_next_hop() const {
            return m_next_hop;
        }

        /*!
         * @brief Sets oem object.
         * @param oem oem.
         */
        void set_oem(const OEMDataDTO::Response& oem) {
            m_oem = oem;
        }

        /*!
         * @brief Gets oem object.
         * @return oem.
         */
        const OEMDataDTO::Response& get_oem() const {
            return m_oem;
        }

        virtual ~Response();
    };

};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_REMOTE_SWITCH_INFO_DTO_HPP */
