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
 * @file core/dto/storage/drives_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_STORAGE_DRIVES_DTO_HPP
#define PSME_CORE_DTO_STORAGE_DRIVES_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Storage namespace */
namespace storage {

/*! Drives request object */
class DrivesDTO {
public:
    /*! Copy constructor */
    DrivesDTO(const DrivesDTO &) = default;

    /*! Assigment constructor */
    DrivesDTO& operator=(const DrivesDTO &) = default;

    /*! Default constructor */
    DrivesDTO() {}

    /*! Drives DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

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

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        virtual ~Request();
    };

    /*! Drives DTO response */
    class Response : public ResponseDTO {
    public:

        class Drive {
            std::string m_name{};
        public:
            /*! Default constructor */
            Drive() { }

            /*! Copy constructor */
            Drive(const Drive&) = default;

            /*! Assignment operator */
            Drive& operator=(const Drive&) = default;

            /*!
             * @brief Get drive name
             * @return Drive name
             * */
            const std::string& get_name() const {
                return m_name;
            }

            /*!
             * @brief Set drive name
             * @param[in] name Drive name
             * */
            void set_name(const std::string& name) {
                m_name = name;
            }

            virtual ~Drive();
        };

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
         * Adds new drive
         *
         * @param drive New drive
         * */
        void add_drive(const Drive& drive) {
            m_drives.push_back(drive);
        }

        /*!
         * Gets vector of drives
         *
         * @return List of drives.
         * */
        const std::vector<Drive>& get_drives()
            const { return m_drives; }

        virtual ~Response();

    private:
        std::vector<Drive> m_drives{};
    };

    ~DrivesDTO();
};

}
}
}
}

#endif /* PSME_CORE_DTO_STORAGE_DRIVES_DTO_HPP */
