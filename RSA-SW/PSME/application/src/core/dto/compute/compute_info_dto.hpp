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

#ifndef COMPUTE_COMPUTE_INFO_DTO_HPP
#define COMPUTE_COMPUTE_INFO_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/status_dto.hpp"
#include "core/dto/fru_info_dto.hpp"
#include "core/dto/oem_data_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Compute namespace */
namespace compute {

/*! Compute information request object */
class ComputeInfoDTO {
public:
    /*! Copy constructor */
    ComputeInfoDTO(const ComputeInfoDTO &) = default;

    /*! Assigment constructor */
    ComputeInfoDTO& operator=(const ComputeInfoDTO &) = default;

    /*! Default constructor */
    ComputeInfoDTO() {}

    /*! Compute information DTO request */
    class Request : public RequestDTO {
        std::string m_compute{};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets compute id
         *
         * @param compute Compute id
         * */
        void set_compute(const std::string& compute) {
            m_compute = compute;
        }

        /*!
         * Gets compute id
         *
         * @return compute id
         * */
        const std::string& get_compute() const {
            return m_compute;
        }

        virtual ~Request();
    };

    /*! compute info information DTO response */
    class Response : public ResponseDTO {
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes compute info information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Set status
         * @param status compute info status
         */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * Gets status
         *
         * @return compute info status
         * */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }


        /*!
         * @brief Set FRUInfo structure
         * @param fru_info compute info FRUInfo
         */
        void set_fru_info(const FRUInfoDTO::Response& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * Gets FRU info
         *
         * @return compute info FRU info
         * */
        const FRUInfoDTO::Response& get_fru_info() const {
            return m_fru_info;
        }

        /*!
         * @brief Set power zone
         * @param power_zone Power zone
         */
        void set_power_zone(const std::string power_zone) {
            m_power_zone = power_zone;
        }

        /*!
         * @brief Get power zone
         * @return Power zone
         */
        const std::string& get_power_zone() const {
            return m_power_zone;
        }

        /*!
         * @brief Set thermal zone
         * @param thermal_zone Thermal zone
         */
        void set_thermal_zone(const std::string& thermal_zone) {
            m_thermal_zone = thermal_zone;
        }

        /*!
         * @brief Get thermal zone
         * @return Thermal zone
         */
        const std::string& get_thermal_zone() const {
            return m_thermal_zone;
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
         * @brief Set OEM data
         * @param oem compute info OEM data
         */
        void set_oem(const OEMDataDTO::Response& oem) {
            m_oem_data = oem;
        }

        /*!
         * Gets OEM specific data
         *
         * @return compute info OEM specific data
         * */
        const OEMDataDTO::Response& get_oem_data() const {
            return m_oem_data;
        }

        virtual ~Response();

    private:
        StatusDTO::Response m_status{};
        FRUInfoDTO::Response m_fru_info{};
        std::string m_power_zone{};
        std::string m_thermal_zone{};
        std::uint32_t m_slot{};
        OEMDataDTO::Response m_oem_data{};
    };

    ~ComputeInfoDTO();
};

}
}
}
}

#endif // COMPUTE_COMPUTE_INFO_DTO_HPP
