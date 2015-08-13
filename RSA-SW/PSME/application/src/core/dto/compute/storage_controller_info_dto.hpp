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
 * @file core/dto/compute/storage_controller_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef COMPUTE_STORAGE_CONTROLLER_INFO_DTO_HPP
#define COMPUTE_STORAGE_CONTROLLER_INFO_DTO_HPP

#include "core/dto/status_dto.hpp"
#include "core/dto/fru_info_dto.hpp"
#include "core/dto/oem_data_dto.hpp"
#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Storage namespace */
namespace compute {

/*! Storage controller information request object */
class StorageControllerInfoDTO {
public:
    /*! Copy constructor */
    StorageControllerInfoDTO(const StorageControllerInfoDTO &) = default;

    /*! Assigment constructor */
    StorageControllerInfoDTO& operator=(const StorageControllerInfoDTO &) = default;

    /*! Default constructor */
    StorageControllerInfoDTO() {}

    /*! Storage controller information DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::uint32_t m_controller{};
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

        /*!
         * Sets storage controller id
         *
         * @param controller Storage controller id
         * */
        void set_controller(std::uint32_t controller) {
            m_controller = controller;
        }

        /*!
         * Gets storage controller id
         *
         * @return Storage controller id
         * */
        std::uint32_t get_controller() const {
            return m_controller;
        }

        virtual ~Request();
    };

    /*! Storage controller information DTO response */
    class Response : public ResponseDTO {
        std::string m_interface{"SATA"};
        std::uint32_t m_drive_count{};
        StatusDTO::Response m_status{};
        FRUInfoDTO::Response m_fru_info{};
        OEMDataDTO::Response m_oem_data{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes storage controller information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Sets interface
         *
         * @param interface Storage controller interface
         * */
        void set_interface(const std::string& interface) {
            m_interface = interface;
        }

        /*!
         * Gets interface
         *
         * @return Storage controller interface
         * */
        const std::string& get_interface() const {
            return m_interface;
        }

        /*!
         * Sets drive count
         *
         * @param drive_count Drive count
         * */
        void set_drive_count(std::uint32_t drive_count) {
            m_drive_count = drive_count;
        }

        /*!
         * Gets drive count
         *
         * @return Drive count
         * */
        std::uint32_t get_drive_count() const {
            return m_drive_count;
        }

        /*!
         * Sets status
         *
         * @param status Status
         * */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * Gets status
         *
         * @return Status
         * */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * Sets FRU info
         *
         * @param fru_info FRU info
         * */
        void set_fru_info(const FRUInfoDTO::Response& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * Gets FRU info
         *
         * @return FRU info
         * */
        const FRUInfoDTO::Response& get_fru_info() const {
            return m_fru_info;
        }

        /*!
         * Sets OEM specific data
         *
         * @param oem_data OEM specific data
         * */
        void set_oem_data(const OEMDataDTO::Response& oem_data) {
            m_oem_data = oem_data;
        }

        /*!
         * Gets OEM specific data
         *
         * @return OEM specific data
         * */
        const OEMDataDTO::Response& get_oem_data() const {
            return m_oem_data;
        }

        virtual ~Response();
    };

    ~StorageControllerInfoDTO();
};

}
}
}
}

#endif /* COMPUTE_STORAGE_CONTROLLER_INFO_DTO_HPP */
