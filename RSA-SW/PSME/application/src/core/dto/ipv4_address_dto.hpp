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
 * @file core/dto/ipv4_address_dto.hpo
 * @brief IPv4 address transfer object declaration.
 *
 * */

#ifndef IPV4_ADDRESS_DTO_HPP
#define IPV4_ADDRESS_DTO_HPP

#include "response_dto.hpp"

#include <json/json.h>
#include <string>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {

/*! IPv4Address transfer object */
class IPv4AddressDTO {
public:
    /*! Copy constructor */
    IPv4AddressDTO(const IPv4AddressDTO &) = default;

    /*! Assigment constructor */
    IPv4AddressDTO& operator=(const IPv4AddressDTO &) = default;

    /*! Default constructor */
    IPv4AddressDTO() {}


    class Response : ResponseDTO {
        std::string m_address{};
        std::string m_subnet_mask{};
        std::string m_address_origin{};
        std::string m_gateway{};

    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * @brief Sets address
         *
         * @param address address
         */
        void set_address(const std::string& address) {
            m_address = address;
        }

        /*!
         * @brief Gets address
         *
         * @return address
         */
        const std::string& get_address() const {
            return m_address;
        }

        /*!
         * @brief Sets subnet mask
         *
         * @param subnet_mask subnet mask
         *
         */
        void set_subnet_mask(const std::string& subnet_mask) {
            m_subnet_mask = subnet_mask;
        }

        /*!
         * @brief Gets subnet mask
         *
         * @return subnet mask
         */
        const std::string& get_subnet_mask() const {
            return m_subnet_mask;
        }

        /*!
         * @brief Sets address origin
         *
         * @param address_origin Address origin
         */
        void set_address_origin(const std::string& address_origin) {
            m_address_origin = address_origin;
        }

        /*!
         * @brief Gets address origin
         *
         * @return Address origin
         */
        const std::string& get_address_origin() const {
            return m_address_origin;
        }

        /*!
         * @brief Sets gateway
         *
         * @param gateway gateway
         *
         */
        void set_gateway(const std::string& gateway) {
            m_gateway = gateway;
        }

        /*!
         * @brief Gets gateway
         *
         * @return gateway
         */
        const std::string& get_gateway() const {
            return m_gateway;
        }

        /*!
         * Desarializes IPv4Address object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        virtual ~Response();
    };

    virtual ~IPv4AddressDTO();
};

}
}
}

#endif // IPV4_ADDRESS_DTO_HPP
