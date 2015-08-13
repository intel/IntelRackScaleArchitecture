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
 * @file core/dto/ipv6_address_dto.hpo
 * @brief IPv6 address transfer object declaration.
 *
 * */

#ifndef IPV6_ADDRESS_DTO_HPP
#define IPV6_ADDRESS_DTO_HPP

#include "response_dto.hpp"

#include <json/json.h>
#include <string>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {

/*! IPv6Address transfer object */
class IPv6AddressDTO {
public:
    /*! Copy constructor */
    IPv6AddressDTO(const IPv6AddressDTO &) = default;

    /*! Assigment constructor */
    IPv6AddressDTO& operator=(const IPv6AddressDTO &) = default;

    /*! Default constructor */
    IPv6AddressDTO() {}


    class Response : ResponseDTO {
        std::string m_address{};
        std::uint32_t m_prefix_length{};
        std::string m_address_origin{};
        std::string m_address_state{"Failed"};
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
         * @param address Address
         */
        void set_address(const std::string& address) {
            m_address = address;
        }

        /*!
         * @brief Gets address
         *
         * @return Address
         */
        const std::string& get_address() const {
            return m_address;
        }

        /*!
         * @brief Sets prefix length
         *
         * @param prefix_length Prefix length
         *
         */
        void set_prefix_length(std::uint32_t prefix_length) {
            m_prefix_length = prefix_length;
        }

        /*!
         * @brief Gets prefix length
         *
         * @return Prefix length
         */
        std::uint32_t get_prefix_length() const {
            return m_prefix_length;
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
         * @brief Sets address state
         *
         * @param address_state Address state
         *
         */
        void set_address_state(const std::string& address_state) {
            m_address_state = address_state;
        }

        /*!
         * @brief Gets address state
         *
         * @return Address state
         */
        const std::string& get_address_state() const {
            return m_address_state;
        }

        /*!
         * Desarializes IPv6Address object from JSON.
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        virtual ~Response();
    };

    virtual ~IPv6AddressDTO();
};

}
}
}

#endif // IPV6_ADDRESS_DTO_HPP
