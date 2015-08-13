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

#ifndef COMPUTE_BLADE_INFO_DTO_HPP
#define COMPUTE_BLADE_INFO_DTO_HPP

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

/*! Blade information request object */
class BladeInfoDTO {
public:
    /*! Copy constructor */
    BladeInfoDTO(const BladeInfoDTO &) = default;

    /*! Assigment constructor */
    BladeInfoDTO& operator=(const BladeInfoDTO &) = default;

    /*! Default constructor */
    BladeInfoDTO() {}

    /*! Blade information DTO request */
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

    /*! Blade information DTO response */
    class Response : public ResponseDTO {
    public:
        using BootSupportedVec = std::vector<std::string>;

        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes blade information response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * @brief Set status
         * @param status blade status
         */
        void set_status(const StatusDTO::Response& status) {
            m_status = status;
        }

        /*!
         * Gets status
         *
         * @return blade status
         * */
        const StatusDTO::Response& get_status() const {
            return m_status;
        }

        /*!
         * @brief Set bios version
         * @param bios_version blade bios version
         */
        void set_bios_version(const std::string& bios_version) {
            m_bios_version = bios_version;
        }

        /*!
         * @brief Get bios version
         * @return blade bios version
         */
        const std::string& get_bios_version() const {
            return m_bios_version;
        }

        /*!
         * @brief Set boot override
         * @param boot_override Boot override value
         */
        void set_boot_override(const std::string& boot_override) {
            m_boot_override = boot_override;
        }

        /*!
         * @brief Get boot override
         * @return Boot override value
         */
        const std::string get_boot_override() const {
            return m_boot_override;
        }

        /*!
         * @brief Set boot override target
         * @param boot_override_target Boot override target value
         */
        void
        set_boot_override_target(const std::string& boot_override_target) {
            m_boot_override_target = boot_override_target;
        }

        /*!
         * @brief Get boot override target
         * @return Boot override target value
         */
        const std::string& get_boot_override_target() const {
            return m_boot_override_target;
        }

        /*!
         * @brief Set boot override supported
         * @param boot_supported Boot override supported
         */
        void
        set_boot_override_supported(const BootSupportedVec& boot_supported) {
            m_boot_override_supported = boot_supported;
        }


        /*!
         * @brief Get boot override supported
         * @return Boot override supported value
         */
        const BootSupportedVec& get_boot_override_supported() const {
            return m_boot_override_supported;
        }

        /*!
         * @brief Set processor count
         * @param processor_count blade processor count
         */
        void set_processor_count(const unsigned int processor_count) {
            m_processor_count = processor_count;
        }

        /*!
         * @brief Get processor count
         * @return blade processor count
         */
        unsigned int get_processor_count() const {
            return m_processor_count;
        }

        /*!
         * @brief Set dimm count
         * @param dimm_count blade dimm count
         */
        void set_dimm_count(const unsigned int dimm_count) {
            m_dimm_count = dimm_count;
        }

        /*!
         * @brief Get dimm count
         * @return blade dimm count
         */
        unsigned int get_dimm_count() const {
            return m_dimm_count;
        }

        /*!
         * @brief Set controller count
         * @param controller_count blade controller count
         */
        void set_controller_count(const unsigned int controller_count) {
            m_controller_count = controller_count;
        }

        /*!
         * @brief Get controller count
         * @return blade controller count
         */
        unsigned int get_controller_count() const {
            return m_controller_count;
        }

        /*!
         * @brief Set nic count
         * @param nic_count blade nic count
         */
        void set_nic_count(const unsigned int nic_count) {
            m_nic_count = nic_count;
        }

        /*!
         * @brief Get nic count
         * @return blade nic count
         */
        unsigned int get_nic_count() const {
            return m_nic_count;
        }

        /*!
         * @brief Set power state
         * @param power_state blade is powered on
         */
        void set_power_state(const std::string& power_state) {
            m_power_state = power_state;
        }

        /*!
         * @brief Get power state
         * @return blade power state
         */
        std::string get_power_state() const {
            return m_power_state;
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
         * @param fru_info blade FRUInfo
         */
        void set_fru_info(const FRUInfoDTO::Response& fru_info) {
            m_fru_info = fru_info;
        }

        /*!
         * Gets FRU info
         *
         * @return blade FRU info
         * */
        const FRUInfoDTO::Response& get_fru_info() const {
            return m_fru_info;
        }
        /*!
         * @brief Set OEM data
         * @param oem blade OEM data
         */
        void set_oem(const OEMDataDTO::Response& oem) {
            m_oem_data = oem;
        }

        /*!
         * Gets OEM specific data
         *
         * @return blade OEM specific data
         * */
        const OEMDataDTO::Response& get_oem_data() const {
            return m_oem_data;
        }

        virtual ~Response();

    private:
        StatusDTO::Response m_status{};
        std::string m_bios_version{};
        std::string m_boot_override{};
        std::string m_boot_override_target{};
        BootSupportedVec m_boot_override_supported{};
        unsigned int m_processor_count{};
        unsigned int m_dimm_count{};
        unsigned int m_controller_count{};
        unsigned int m_nic_count{};
        std::string m_power_state{};
        std::uint32_t m_slot{};
        FRUInfoDTO::Response m_fru_info{};
        OEMDataDTO::Response m_oem_data{};
    };

    ~BladeInfoDTO();
};

}
}
}
}

#endif // COMPUTE_BLADE_INFO_DTO_HPP
