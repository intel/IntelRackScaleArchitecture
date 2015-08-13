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

#ifndef COMPUTE_MODULE_INFO_DTO_HPP
#define COMPUTE_MODULE_INFO_DTO_HPP

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
/*! Compute namespace */
namespace compute {

/*! Memory information request object */
class ModuleInfoDTO {
public:
    /*! Copy constructor */
    ModuleInfoDTO(const ModuleInfoDTO &) = default;

    /*! Assigment constructor */
    ModuleInfoDTO& operator=(const ModuleInfoDTO &) = default;

    /*! Default constructor */
    ModuleInfoDTO() {}

    /*! Memory information DTO request */
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
         * Sets component id
         *
         * @param component Component id
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets component id
         *
         * @return Component id
         * */
        const std::string& get_component() const {
            return m_component;
        }

        virtual ~Request();
    };

    /*! Module information DTO response */
    class Response : public ResponseDTO {
        StatusDTO::Response m_status{};
        unsigned int m_blade_count{};
        unsigned int m_switch_count{};
        std::uint32_t m_slot{};
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
         * Desarializes module information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Set status
         * @param status module status
         */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * Gets status
         * @return module status
         * */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * @brief Set blade count
         * @param blade_count module blade count
         */
        void set_blade_count(const unsigned int blade_count) {
            m_blade_count = blade_count;
        }

        /*!
         * @brief Set switch count
         * @param switch_count switch count
         */
        void set_switch_count(const unsigned int switch_count) {
            m_switch_count = switch_count;
        }

        /*!
         * @brief Get blade count
         * @return module blade count
         */
        unsigned int get_blade_count() const {
            return m_blade_count;
        }

        /*!
         * @brief Set slot
         * @param slot Slot
         */
        void set_slot(const std::uint32_t slot) {
            m_slot = slot;
        }

        /*!
         * @brief Get slot
         * @return Slot
         */
        std::uint32_t get_slot() const {
            return m_slot;
        }

        /*!
         * @brief Set FRUInfo structure
         * @param fru_info module FRUInfo
         */
        void set_fru_info(const FRUInfoDTO::Response& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * Get FRU info
         * @return module FRU info
         * */
        const FRUInfoDTO::Response& get_fru_info() const {
            return m_fru_info;
        }

        /*!
         * @brief Set OEM data
         * @param oem module OEM data
         */
        void set_oem(const OEMDataDTO::Response& oem) {
            m_oem_data = oem;
        }

        /*!
         * Gets OEM specific data
         * @return module OEM specific data
         * */
        const OEMDataDTO::Response& get_oem_data() const {
            return m_oem_data;
        }

        virtual ~Response();
    };

    ~ModuleInfoDTO();
};

}
}
}
}

#endif // COMPUTE_MODULE_INFO_DTO_HPP
