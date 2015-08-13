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
 * @file core/dto/fru_info_dto.hpo
 * @brief FRUInfo response transfer obejct declaration.
 *
 * */

#ifndef FRU_INFO_DTO_HPP
#define FRU_INFO_DTO_HPP

#include "response_dto.hpp"

#include <json/json.h>

#include <string>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {

/*! FRU info transfer object */
class FRUInfoDTO {
public:
    /*! Copy constructor */
    FRUInfoDTO(const FRUInfoDTO &) = default;

    /*! Assigment constructor */
    FRUInfoDTO& operator=(const FRUInfoDTO &) = default;

    /*! Default constructor */
    FRUInfoDTO() {}

    class Response : ResponseDTO {
        std::string m_serial_number{};
        std::string m_manufacturer{};
        std::string m_model_number{};
        std::string m_part_number{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes FRUInfo response object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Sets serial number
         *
         * @param serial_number Serial number
         * */
        void set_serial_number(const std::string& serial_number) {
            m_serial_number = serial_number;
        }

        /*!
         * Gets serial number
         *
         * @return Serial number
         * */
        const std::string& get_serial_number() const { return m_serial_number; }

        /*!
         * Sets manufacturer
         *
         * @param manufacturer Manufacturer
         * */
        void set_manufacturer(const std::string& manufacturer) {
            m_manufacturer = manufacturer;
        }

        /*!
         * Gets manufacturer
         *
         * @return Manufacturer
         * */
        const std::string& get_manufacturer() const { return m_manufacturer; }

        /*!
         * Sets model number
         *
         * @param model_number Model number
         * */
        void set_model_number(const std::string& model_number) {
            m_model_number = model_number;
        }

        /*!
         * Gets model number
         *
         * @return Model number
         * */
        const std::string& get_model_number() const { return m_model_number; }

        /*!
         * Sets part number
         *
         * @param part_number Part number
         * */
        void set_part_number(const std::string& part_number) {
            m_part_number = part_number;
        }

        /*!
         * Gets part number
         *
         * @return Part number
         * */
        const std::string& get_part_number() const { return m_part_number; }


        virtual ~Response();
    };

    virtual ~FRUInfoDTO();
};

}
}
}

#endif /* FRU_INFO_DTO_HPP */

