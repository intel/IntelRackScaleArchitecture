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
 * */

#ifndef COMPUTE_COMPUTE_ZONE_INFO_DTO_HPP
#define COMPUTE_COMPUTE_ZONE_INFO_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/status_dto.hpp"
#include "core/dto/oem_data_dto.hpp"
#include "core/dto/collections_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Compute namespace */
namespace compute {

/*! compute zone information request object */
class ComputeZoneInfoDTO {
public:
    /*! Copy constructor */
    ComputeZoneInfoDTO(const ComputeZoneInfoDTO &) = default;

    /*! Assigment constructor */
    ComputeZoneInfoDTO& operator=(const ComputeZoneInfoDTO &) = default;

    /*! Default constructor */
    ComputeZoneInfoDTO() {}

    /*! compute zone information DTO request */
    class Request : public RequestDTO {
        std::string m_zone{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets zone id
         *
         * @param zone zone id
         * */
        void set_zone(const std::string& zone) {
            m_zone = zone;
        }

        /*!
         * Gets zone id
         *
         * @return zone id
         * */
        const std::string& get_zone() const {
            return m_zone;
        }

        virtual ~Request();
    };

    /*! compute zone information DTO response */
    class Response : public ResponseDTO {
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes compute zone information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Set status
         * @param status compute zone status
         */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * Gets status
         * @return compute zone status
         * */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * Sets the collections
         * @param collections Collections to set
         * */
        void set_collections(const CollectionsDTO::Response& collections) {
            m_collections = collections;
        }

        /*!
         * Gets the collections
         * @return Collections reference
         * */
        const CollectionsDTO::Response& get_collections() const {
            return m_collections;
        }


        /*!
         * @brief Set OEM data
         * @param oem compute zone OEM data
         */
        void set_oem(const OEMDataDTO::Response& oem) {
            m_oem_data = oem;
        }

        /*!
         * Gets OEM specific data
         * @return compute zone OEM specific data
         * */
        const OEMDataDTO::Response& get_oem_data() const {
            return m_oem_data;
        }

        virtual ~Response();

    private:
        StatusDTO::Response m_status{};
        CollectionsDTO::Response m_collections{};
        OEMDataDTO::Response m_oem_data{};
    };

    ~ComputeZoneInfoDTO();
};

}
}
}
}

#endif // COMPUTE_COMPUTE_ZONE_INFO_DTO_HPP
