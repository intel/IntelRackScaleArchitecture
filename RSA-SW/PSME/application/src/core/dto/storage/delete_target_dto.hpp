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
 * @file core/dto/storage/target_info_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_STORAGE_DELETE_TARGET_DTO_HPP
#define PSME_CORE_DTO_STORAGE_DELETE_TARGET_DTO_HPP

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
namespace storage {

/*! Target information request object */
class DeleteTargetDTO {
public:
    /*! Copy constructor */
    DeleteTargetDTO(const DeleteTargetDTO &) = default;

    /*! Assigment constructor */
    DeleteTargetDTO& operator=(const DeleteTargetDTO &) = default;

    /*! Default constructor */
    DeleteTargetDTO() {}

    /*! target information DTO request */
    class Request : public RequestDTO {
        std::string m_target{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets target uuid
         *
         * @param target target uuid
         * */
        void set_target(const std::string& target) {
            m_target = target;
        }

        /*!
         * Gets target uuid
         *
         * @return target uuid
         * */
        const std::string& get_target() const {
            return m_target;
        }

        virtual ~Request();
    };

    /*! target information DTO response */
    class Response : public ResponseDTO {
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes target information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

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
        OEMDataDTO::Response m_oem_data{};
    };

    ~DeleteTargetDTO();
};

}
}
}
}

#endif /* STORAGE_DELETE_TARGET_DTO_HPP */
