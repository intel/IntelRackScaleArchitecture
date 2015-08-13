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
 * @file core/dto/storage/logical_drive_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef STORAGE_LOGICAL_DRIVE_INFO_DTO_HPP
#define STORAGE_LOGICAL_DRIVE_INFO_DTO_HPP

#include "core/dto/status_dto.hpp"
#include "core/dto/oem_data_dto.hpp"
#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/collections_dto.hpp"

#include <list>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Storage namespace */
namespace storage {

/*! Drive information request object */
class LogicalDriveInfoDTO {
public:
    /*! Copy constructor */
    LogicalDriveInfoDTO(const LogicalDriveInfoDTO &) = default;

    /*! Assigment constructor */
    LogicalDriveInfoDTO& operator=(const LogicalDriveInfoDTO &) = default;

    /*! Default constructor */
    LogicalDriveInfoDTO() {}

    /*! Drive information DTO request */
    class Request : public RequestDTO {
        std::string m_drive{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets drive uuid
         *
         * @param drive Drive uuid
         * */
        void set_drive(const std::string& drive) {
            m_drive = drive;
        }

        /*!
         * Gets drive number
         *
         * @return Drive number
         * */
        const std::string& get_drive() const {
            return m_drive;
        }

        virtual ~Request();
    };

    /*! Drive information DTO response */
    class Response : public ResponseDTO {
        std::string m_mode{};
        std::string m_image{};
        std::string m_type{};
        std::string m_master{};
        std::uint32_t m_capacity_gb{};
        bool m_protected{};
        bool m_bootable{};
        bool m_snapshot{};
        StatusDTO::Response m_status{};
        OEMDataDTO::Response m_oem_data{};
        CollectionsDTO::Response m_collections{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes drive information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Sets drive type
         *
         * @param type Drive type
         * */
        void set_type(const std::string& type) {
            m_type = type;
        }

        /*!
         * Gets drive type
         *
         * @return Drive type
         * */
        const std::string& get_type() const {
            return m_type;
        }

        /*!
         * Sets master
         *
         * @param master UUID of the master drive used as an image source.
         * */
        void set_master(const std::string& master) {
            m_master = master;
        }

        /*!
         * Gets master
         *
         * @return UUID of the master drive used as an image source.
         * */
        const std::string& get_master() const {
            return m_master;
        }

        /*!
         * Sets drive capacity
         *
         * @param capacity_gb Drive capacity in GB
         * */
        void set_capacity_gb(std::uint32_t capacity_gb) {
            m_capacity_gb = capacity_gb;
        }

        /*!
         * Gets drive capacity
         *
         * @return Drive capacity in GB
         * */
        std::uint32_t get_capacity_gb() const {
            return m_capacity_gb;
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

        /*!
         * Sets image.
         *
         * @param image Image to set.
         * */
        void set_image(const std::string& image) {
            m_image = image;
        }

        /*!
         * Gets the image.
         *
         * @return Image
         * */
        const std::string& get_image() const {
            return m_image;
        }

        /*!
         * Sets protected field
         *
         * @param protected_ Protected value
         * */
        void set_protected(bool protected_) {
            m_protected = protected_;
        }

        /*!
         * Gets protected value
         *
         * @return Protected value
         * */
        bool get_protected() const {
            return m_protected;
        }

        /*!
         * Sets bootable field
         *
         * @param bootable Bootable value
         * */
        void set_bootable(bool bootable) {
            m_bootable = bootable;
        }

        /*!
         * Gets bootable value
         *
         * @return Bootable value
         * */
        bool get_bootable() const {
            return m_bootable;
        }

        /*!
         * Sets snapshot field
         *
         * @param snapshot Snapshot value
         * */
        void set_snapshot(bool snapshot) {
            m_snapshot = snapshot;
        }

        /*!
         * Gets snapshot value
         *
         * @return Snapshot value
         * */
        bool get_snapshot() const {
            return m_snapshot;
        }

        /*!
         * Sets the collections
         *
         * @param collections Collections to set
         * */
        void set_collections(const CollectionsDTO::Response& collections) {
            m_collections = collections;
        }

        /*!
         * Gets the collections
         *
         * @return Collections reference
         * */
        const CollectionsDTO::Response& get_collections() const {
            return m_collections;
        }

        /*!
         * Sets the mode
         *
         * @param mode Mode to set
         * */
        void set_mode(const std::string& mode) {
            m_mode = mode;
        }

        /*!
         * Gets the mode
         *
         * @return Mode
         * */
        const std::string& get_mode() const {
            return m_mode;
        }

        virtual ~Response();
    };

    ~LogicalDriveInfoDTO();
};

}
}
}
}

#endif /* STORAGE_LOGICAL_DRIVE_INFO_DTO_HPP */
