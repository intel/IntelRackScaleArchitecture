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

#ifndef STORAGE_TARGET_INFO_DTO_HPP
#define STORAGE_TARGET_INFO_DTO_HPP

#include "core/dto/status_dto.hpp"
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
class TargetInfoDTO {
public:
    /*! Copy constructor */
    TargetInfoDTO(const TargetInfoDTO &) = default;

    /*! Assigment constructor */
    TargetInfoDTO& operator=(const TargetInfoDTO &) = default;

    /*! Default constructor */
    TargetInfoDTO() {}

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

        /*! Class represents lun data */
        class Lun {
        public:
            /*!
             * @brief Construct lun object
             * @param lun Lun number
             * @param logical_drive_uuid Logica drive uuid
             */
            Lun(const std::uint64_t lun, const std::string& logical_drive_uuid):
                m_lun{lun}, m_logical_drive_uuid{logical_drive_uuid} {}

            /*!
             * @brief Get lun number
             * @return Lun number
             */
            std::uint64_t get_lun() const {
                return m_lun;
            }

            /*!
             * @brief Get logical drive uuid
             * @return Logical drive uuid
             */
            const std::string& get_logical_drive_uuid() const {
                return m_logical_drive_uuid;
            }

        private:
            std::uint64_t m_lun{};
            std::string m_logical_drive_uuid{};
        };
        using LunVec = std::vector<Lun>;

        /*!
         * Desarializes target information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Set target luns
         * @param luns Target luns
         */
        void set_target_luns(const LunVec& luns) {
            m_luns = luns;
        }

        /*!
         * @brief Get target luns
         * @return Target luns
         */
        const LunVec& get_target_luns() const {
            return m_luns;
        }

        /*!
         * @brief Get target iqn
         * @return Target iqn
         */
        const std::string& get_target_iqn() const {
            return m_target_iqn;
        }

        /*!
         * @brief Set target iqn
         * @param target_iqn Target iqn
         */
        void set_target_iqn(const std::string& target_iqn) {
            m_target_iqn = target_iqn;
        }

        /*!
         * @brief Get target address
         * @return Target address
         */
        const std::string& get_target_address() const {
            return m_target_address;
        }

        /*!
         * @brief Set target address
         * @param target_address Target address
         */
        void set_target_address(const std::string& target_address) {
            m_target_address = target_address;
        }

        /*!
         * @brief Get target port
         * @return Target port
         */
        std::uint32_t get_target_port() const {
            return m_target_port;
        }

        /*!
         * @brief Set target port
         * @param port Target port
         */
        void set_target_port(const std::uint32_t port) {
            m_target_port = port;
        }

        /*!
         * @brief Get initiator iqn
         * @return Initiator iqn
         */
        const std::string& get_initiator_iqn() const {
            return m_initiator_iqn;
        }

        /*!
         * @brief Set target initiator iqn
         * @param initiator_iqn Target initiator iqn
         */
        void set_initiator_iqn(const std::string& initiator_iqn) {
            m_initiator_iqn = initiator_iqn;
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
        LunVec m_luns{};
        std::string m_target_iqn{};
        std::string m_target_address{};
        std::uint32_t m_target_port{};
        std::string m_initiator_iqn{};
        StatusDTO::Response m_status{};
        OEMDataDTO::Response m_oem_data{};
    };

    ~TargetInfoDTO();
};

}
}
}
}

#endif /* STORAGE_TARGET_INFO_DTO_HPP */
