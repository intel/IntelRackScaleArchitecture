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
 * @file core/dto/request/request_dto.hpp
 * @brief Parnet request transfer obejct declaration.
 *
 * */

#ifndef REQUEST_DTO_HPP
#define REQUEST_DTO_HPP

#include <json/json.h>

/*! PSME namespace */
namespace psme {
/*! core namespace */
namespace core {
/*! dto namespace */
namespace dto {

/*! Generic request transfer object */
class RequestDTO {
    int m_id{};
public:
    /*! Copy constructor */
    RequestDTO(const RequestDTO &) = default;

    /*! Default constructor */
    RequestDTO() {}

    /*! Assigment constructor */
    RequestDTO& operator=(const RequestDTO&) = default;

    /*!
     * Sets request ID.
     *
     * @param id request unique ID.
     * */
    void set_id(const int id) { m_id = id; }

    /*!
     * Gets request ID.
     *
     * @return request unique id.
     * */
    int get_id() const { return m_id; }

    /*!
     * Serializes request object to JSON.
     *
     * @return Serialzied object in JSON format,
     * */
    virtual const Json::Value to_json() const = 0;

    virtual ~RequestDTO();
};

}
}
}

#endif /* COMPUTE_POOL_HPP */
