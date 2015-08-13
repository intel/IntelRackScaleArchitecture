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
 * @file core/dto/storage/storage_services_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_STORAGE_STORAGE_SERVICES_INFO_DTO_HPP
#define PSME_CORE_DTO_STORAGE_STORAGE_SERVICES_INFO_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/collections_dto.hpp"
#include "core/dto/status_dto.hpp"
#include "core/dto/oem_data_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Storage namespace */
namespace storage {

/*! StorageServicesInfo request object */
class StorageServicesInfoDTO {
public:
    /*! Copy constructor */
    StorageServicesInfoDTO(const StorageServicesInfoDTO&) = default;

    /*! Assigment constructor */
    StorageServicesInfoDTO& operator=(const StorageServicesInfoDTO&) = default;

    /*! Default constructor */
    StorageServicesInfoDTO() {}

    /*! StorageServicesInfo DTO request */
    class Request : public RequestDTO {
        std::string m_services{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*!
         * Sets services UUID
         *
         * @param services Services UUID
         * */
        void set_services(const std::string& services) {
            m_services = services;
        }

        /*!
         * Gets services UUID
         *
         * @return Services UUID
         * */
        const std::string& get_services() const {
            return m_services;
        }

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        virtual ~Request();
    };

    /*! StorageServicesInfo DTO response */
    class Response : public ResponseDTO {
    public:

        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes drives response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Adds collection
         *
         * @param collection Collection
         * */
        void add_collection(const CollectionsDTO::Collection& collection) {
            m_collections.add_collection(collection);
        }

        /*!
         * Gets collections
         *
         * @return Collections
         * */
        const CollectionsDTO::Response& get_collections() const {
            return m_collections;
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

    private:
        CollectionsDTO::Response m_collections{};
        StatusDTO::Response m_status{};
        OEMDataDTO::Response m_oem_data{};
    };

    ~StorageServicesInfoDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_STORAGE_STORAGE_SERVICES_INFO_DTO_HPP */
