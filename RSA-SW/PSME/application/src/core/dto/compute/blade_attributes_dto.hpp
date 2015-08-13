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

#ifndef COMPUTE_BLADE_ATTRIBUTES_DTO_HPP
#define COMPUTE_BLADE_ATTRIBUTES_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"
#include "core/dto/oem_data_dto.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Compute namespace */
namespace compute {

/*! Blade attributes request object */
class BladeAttributesDTO {
public:
    /*! Copy constructor */
    BladeAttributesDTO(const BladeAttributesDTO &) = default;

    /*! Assigment constructor */
    BladeAttributesDTO& operator=(const BladeAttributesDTO &) = default;

    /*! Default constructor */
    BladeAttributesDTO() {}

    /*! Blade attributes DTO request */
    class Request : public RequestDTO {
        std::string m_component{};
        std::string m_boot_override{};
        std::string m_boot_override_target{};
        std::string m_power_state{"NoChange"};
        OEMDataDTO::Request m_oem_data{};
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
         * @param component Component id
         * */
        void set_component(const std::string& component) {
            m_component = component;
        }

        /*!
         * Gets component id
         * @return Component id
         * */
        const std::string& get_component() const {
            return m_component;
        }

        /*!
         * @brief Set boot override
         * @param boot_override Boot override value
         */
        void set_boot_override(const std::string& boot_override) {
            m_boot_override = boot_override;
        }

        /*!
         * @brief Get boot overrride
         * @return Boot override value
         */
        const std::string& get_boot_override() const {
            return m_boot_override;
        }

        /*!
         * @brief Set boot override target
         * @param boot_override_target Boot override target value
         */
        void set_boot_override_target(const std::string& boot_override_target) {
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
         * @brief Set desired power state of the Blade.
         * @param power_state string which represents desired power state.
         * Formatted compatible with PSME API.
         */
        void set_power_state(const std::string& power_state) {
            m_power_state = power_state;
        }

        /*!
         * @brief Get set power state of the Blade
         * @return string represents current power state of the Blade.
         */
        const std::string& get_power_state() const {
            return m_power_state;
        }

        /*!
         * @brief Set OEM data
         * @param oem blade OEM data
         */
        void set_oem(const OEMDataDTO::Request& oem) {
            m_oem_data = oem;
        }

        /*!
         * Gets OEM specific data
         * @return blade OEM specific data
         * */
        const OEMDataDTO::Request& get_oem_data() const {
            return m_oem_data;
        }

        virtual ~Request();
    };

    /*! Blade information DTO response */
    class Response : public ResponseDTO {
    public:
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

        virtual ~Response();
    };

    ~BladeAttributesDTO();
};

}
}
}
}

#endif // COMPUTE_BLADE_ATTRIBUTES_DTO_HPP
