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
 * @file core/dto/storage/add_logical_drive_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef STORAGE_ADD_LOGICAL_DRIVE_DTO_HPP
#define STORAGE_ADD_LOGICAL_DRIVE_DTO_HPP

#include "core/dto/oem_data_dto.hpp"
#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"

#include <list>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Storage namespace */
namespace storage {

/*! Logical drive add request object */
class AddLogicalDriveDTO {
public:
    /*! Copy constructor */
    AddLogicalDriveDTO(const AddLogicalDriveDTO &) = default;

    /*! Assigment constructor */
    AddLogicalDriveDTO& operator=(const AddLogicalDriveDTO &) = default;

    /*! Default constructor */
    AddLogicalDriveDTO() {}

    /*! Logical drive add request DTO */
    class Request : public RequestDTO {
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Copy constructor. */
        Request(const Request&) = default;

        /*! Assignment operator */
        Request& operator=(const Request&) = default;

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;


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
        void set_capacity_gb(unsigned capacity_gb) {
            m_capacity_gb = capacity_gb;
        }

        /*!
         * Gets drive capacity
         *
         * @return Drive capacity in GB
         * */
        unsigned get_capacity_gb() const {
            return m_capacity_gb;
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

        /*!
         * @brief Add a logical drive to the drives list
         * @param drive_uuid UUID if the drive
         */
        void add_drive(const std::string& drive_uuid) {
            m_drives.push_back(drive_uuid);
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


        virtual ~Request();

    private:
        std::string m_type{};
        unsigned m_capacity_gb{};
        std::string m_mode{};
        std::string m_master{};
        bool m_snapshot{};
        bool m_protected{};
        std::vector<std::string> m_drives{};
        OEMDataDTO::Response m_oem_data{};
    };

    /*! Add drive DTO response */
    class Response : public ResponseDTO {
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes add drive response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

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
        std::string m_drive{};
        OEMDataDTO::Response m_oem_data{};
    };

    ~AddLogicalDriveDTO();
};

}
}
}
}

#endif /* STORAGE_ADD_LOGICAL_DRIVE_DTO_HPP */
