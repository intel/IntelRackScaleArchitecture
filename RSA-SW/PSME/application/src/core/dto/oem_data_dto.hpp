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
 * @file core/dto/oem_data_dto.hpo
 * @brief OEMData response transfer obejct declaration.
 *
 * */

#ifndef OEM_DATA_DTO_HPP
#define OEM_DATA_DTO_HPP

#include "response_dto.hpp"
#include "request_dto.hpp"

#include <json/json.h>
#include "json/json.hpp"

#include <string>
#include <sstream>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {

/*! Component transfer object */
class OEMDataDTO {
public:
    /*! Copy constructor */
    OEMDataDTO(const OEMDataDTO &) = default;

    /*! Assigment constructor */
    OEMDataDTO& operator=(const OEMDataDTO &) = default;

    /*! Default constructor */
    OEMDataDTO() {}

    /*! DTO request */
    class Request : public RequestDTO {
        Json::Value m_oem_data{Json::objectValue};
    public:
        /*! Default constructor. */
        Request() : RequestDTO(){}

        /*! Copy constructor */
        Request(const Request&) = default;

        /*! Assigment constructor */
        Request& operator=(const Request&) = default;

        /*! Serializes object to JSON
         *
         *  @return Request object serialized to JSON.
         * */
        const Json::Value to_json() const;

        /*!
         * Sets OEM specific data
         *
         * @param[in] oem_data OEM specific data
         * */
        void set_oem_data(const Json::Value& oem_data) {
            m_oem_data = oem_data;
        }

        virtual ~Request();
    };

    class Response : ResponseDTO {
        Json::Value m_oem_data{Json::objectValue};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes OEMData response object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Sets OEM specific data
         *
         * @param oem_data OEM specific data
         * */
        void set_oem_data(const Json::Value& oem_data) {
            m_oem_data = oem_data;
        }

        /*!
         * Gets OEM specific data
         *
         * @return OEM specific data
         * */
        const Json::Value& get_oem_data() const { return m_oem_data; }

        /*!
         * @brief Converts Json::Value to json::value
         *
         * @return json::Value object
         **/
        const json::Value to_json_value() const {
            std::stringstream sstream;
            json::Value result;
            sstream << m_oem_data;
            sstream.str() >> result;
            return result;
        }

        virtual ~Response();
    };

    virtual ~OEMDataDTO();
};

}
}
}

#endif /* OEM_DATA_DTO_HPP */

