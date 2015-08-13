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
 * @file core/dto/network/network_hop_dto.hpp
 * @brief Request transfer object declaration.
 * */

#ifndef PSME_CORE_DTO_NETWORK_NETWORK_HOP_DTO_HPP
#define PSME_CORE_DTO_NETWORK_NETWORK_HOP_DTO_HPP

#include "core/dto/request_dto.hpp"
#include "core/dto/response_dto.hpp"

#include <cstdint>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! DTO namespace */
namespace dto {
/*! Network namespace */
namespace network {

/*! Network Hop DTO object */
class NetworkHopDTO {
public:
    /*! Copy constructor */
    NetworkHopDTO(const NetworkHopDTO &) = default;

    /*! Assigment constructor */
    NetworkHopDTO& operator=(const NetworkHopDTO &) = default;

    /*! Default constructor */
    NetworkHopDTO() {}

    /*! Destructor */
    ~NetworkHopDTO();

    /*! DTO response */
    class Response : public ResponseDTO {
        std::int32_t m_metric{};
        std::string m_port_identifier{};
        std::string m_ipv4_address{};
        std::string m_ipv6_address{};
    public:
        /*! Copy constructor */
        Response(const Response &) = default;

        /*! Assigment constructor */
        Response& operator=(const Response &) = default;

        /*! Default constructor */
        Response() : ResponseDTO(){}

        /*!
         * Desarializes VLAN port response object to JSON
         *
         * @param response Response JSON
         * */
        void to_object(const Json::Value& response);

        /*!
         * Sets metric. Cost associated with a given path to remote switch.
         * @param metric value represetns cost.
         */
        void set_metric(const std::int32_t metric) {
            m_metric = metric;
        }

        /*!
         * Gets metric. Cost associated with a given path to remote switch.
         * @return value represetns metric cost.
         */
        std::int32_t get_metric() const {
            return m_metric;
        }

        /*!
         * Sets identifier of a port through which a given remote switch is
         * accessible.
         * @param id port identifier. (not uuid).
         */
        void set_port_identifier(const std::string& id) {
            m_port_identifier = id;
        }

        /*!
         * Gets identifier of a port through which a given remote switch is
         * accessible.
         * @return id port identifier. (not uuid).
         */
        const std::string& get_port_identifier() const {
            return m_port_identifier;
        }

        /*!
         * Sets IP v4 through which a given remote switch is
         * accessible.
         * @param ip IPv4 to set.
         */
        void set_ipv4_address(const std::string& ip) {
            m_ipv4_address = ip;
        }

        /*!
         * Gets IP v4 through which a given remote switch is
         * accessible.
         * @return string with IP version 4.
         */
        const std::string& get_ipv4_address() const {
            return m_ipv4_address;
        }

        /*!
         * Sets IP v6 through which a given remote switch is
         * @param ip string with IP version 6.
         */
        void set_ipv6_address(const std::string& ip) {
            m_ipv6_address = ip;
        }

        /*!
         * Gets IP v6 through which a given remote switch is
         * accessible.
         * @return string with IP version 6.
         */
        const std::string& get_ipv6_address() const {
            return m_ipv6_address;
        }


        virtual ~Response();
    };


};

}
}
}
}

#endif /* PSME_CORE_DTO_NETWORK_ADD_PORT_VLAN_DTO_HPP */
