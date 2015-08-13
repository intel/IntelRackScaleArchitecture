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

#ifndef COMPUTE_CHASSIS_INFO_DTO_HPP
#define COMPUTE_CHASSIS_INFO_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/status_dto.hpp"
#include "core/dto/fru_info_dto.hpp"
#include "core/dto/oem_data_dto.hpp"
#include "core/dto/collections_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! compute namespace */
namespace compute {

/*! chassis information request object */
class ChassisInfoDTO {
public:
    /*! Copy constructor */
    ChassisInfoDTO(const ChassisInfoDTO &) = default;

    /*! Assigment constructor */
    ChassisInfoDTO& operator=(const ChassisInfoDTO &) = default;

    /*! Default constructor */
    ChassisInfoDTO() {}

    /*! chassis information DTO request */
    class Request : public RequestDTO {
        std::string m_chassis{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets chassis id
         *
         * @param chassis chassis id
         * */
        void set_chassis(const std::string& chassis) {
            m_chassis = chassis;
        }

        /*!
         * Gets chassis id
         *
         * @return chassis id
         * */
        const std::string& get_chassis() const {
            return m_chassis;
        }

        virtual ~Request();
    };

    /*! chassis info information DTO response */
    class Response : public ResponseDTO {
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes chassis info information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Set status
         * @param status chassis info status
         */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * Gets status
         * @return chassis info status
         * */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * @brief Set chassis type
         * @param type Chassis type
         */
        void set_type(const std::string& type) {
            m_type = type;
        }

        /*!
         * @brief Get chassis type
         * @return Chassis type
         */
        const std::string& get_type() const {
            return m_type;
        }

        /*!
         * @brief Set size
         * @param size Size
         */
        void set_size(std::uint32_t size) {
            m_size = size;
        }

        /*!
         * @brief Get chassis size
         * @return Chassis type
         */
        std::uint32_t get_size() const {
            return m_size;
        }

        /*!
         * @brief Set location offset
         * @param location_offset Location offset
         */
        void set_location_offset(std::uint32_t location_offset) {
            m_location_offset = location_offset;
        }

        /*!
         * @brief Get location offset
         * @return Location offset
         */
        std::uint32_t get_location_offset() const {
            return m_location_offset;
        }

        /*!
         * @brief Set FRUInfo structure
         * @param fru_info chassis info FRUInfo
         */
        void set_fru_info(const FRUInfoDTO::Response& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * Gets FRU info
         *
         * @return chassis info FRU info
         * */
        const FRUInfoDTO::Response& get_fru_info() const {
            return m_fru_info;
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
         * @param oem chassis info OEM data
         */
        void set_oem(const OEMDataDTO::Response& oem) {
            m_oem_data = oem;
        }

        /*!
         * Gets OEM specific data
         *
         * @return chassis info OEM specific data
         * */
        const OEMDataDTO::Response& get_oem_data() const {
            return m_oem_data;
        }

        virtual ~Response();

    private:
        StatusDTO::Response m_status{};
        std::string m_type{};
        std::uint32_t m_size{};
        std::uint32_t m_location_offset{};
        FRUInfoDTO::Response m_fru_info{};
        CollectionsDTO::Response m_collections{};
        OEMDataDTO::Response m_oem_data{};
    };

    ~ChassisInfoDTO();
};

}
}
}
}

#endif // chassis_CHASSIS_INFO_DTO_HPP
