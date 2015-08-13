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
 * @file core/dto/storage/add_target_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_STORAGE_ADD_TARGET_DTO_HPP
#define PSME_CORE_DTO_STORAGE_ADD_TARGET_DTO_HPP

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

/*! Add target request object */
class AddTargetDTO {
public:
    /*! Copy constructor */
    AddTargetDTO(const AddTargetDTO &) = default;

    /*! Assigment constructor */
    AddTargetDTO& operator=(const AddTargetDTO &) = default;

    /*! Default constructor */
    AddTargetDTO() {}

    /*! Lun, drive object mapper */
    class TargetLun {
    public:
        /*!
         * @brief Construct lun to drive mapper object
         * @param drive Drive UUID
         * @param lun Lun number
         */
         TargetLun(const std::string& drive, std::uint64_t lun) :
             m_drive{drive}, m_lun{lun} {}

        /*!
         * @brief Get drive uuid
         * @return Drive UUID
         */
        const std::string& get_drive() const {
            return m_drive;
        }

        /*!
         * @brief Get lun number
         * @return Lun number
         */
        std::uint64_t get_lun() const {
            return m_lun;
        }

    private:
        std::string m_drive{};
        std::uint64_t m_lun{};
    };
    using TargetLunVec = std::vector<TargetLun>;

    /*! target information DTO request */
    class Request : public RequestDTO {
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}
        Request(const Request&) = default;
        Request& operator=(const Request&) = default;
        Request(Request&&) = default;
        Request& operator=(Request&&) = default;

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * @brief Set request luns
         * @param luns Request luns
         */
        void set_luns(const TargetLunVec& luns) {
            m_luns = luns;
        }

        /*!
         * @brief Get request luns
         * @return Request luns
         */
        const TargetLunVec& get_luns() const {
            return m_luns;
        }

        /*!
         * @brief Add lun number
         * @param lun Lun number
         */
        void add_lun(const TargetLun& lun) {
            m_luns.emplace_back(lun);
        }

        /*!
         * @brief Get request iqn
         * @return Request iqn
         */
        const std::string& get_target_iqn() const {
            return m_target_iqn;
        }

        /*!
         * @brief Set request iqn
         * @param target_iqn Request iqn
         */
        void set_target_iqn(const std::string& target_iqn) {
            m_target_iqn = target_iqn;
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
        TargetLunVec m_luns{};
        std::string m_target_iqn{};
        std::string m_initiator_iqn{};
        OEMDataDTO::Response m_oem_data{};
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
         * @brief Set target uuid
         * @param target Target uuid
         */
        void set_target(const std::string& target) {
            m_target = target;
        }

        /*!
         * @brief Get target uuid
         * @return Target uuid
         */
        const std::string& get_target() const {
            return m_target;
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
         * @brief Is valid response
         * @return If response is valid true, otherwise false
         */
        bool is_valid() const {
            return !m_target.empty();
        }

        virtual ~Response();
    private:
        std::string m_target{};
        std::string m_target_address{};
        std::uint32_t m_target_port{};
        OEMDataDTO::Response m_oem_data{};
    };

    ~AddTargetDTO();
};

}
}
}
}

#endif /* STORAGE_ADD_TARGET_DTO_HPP */
