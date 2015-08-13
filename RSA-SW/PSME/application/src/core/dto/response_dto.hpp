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
 * @file core/dto/response/response_dto.hpp
 * @brief Response transfer obejct declaration.
 * */

#ifndef RESPONSE_DTO_HPP
#define RESPONSE_DTO_HPP

#include <json/json.h>

#include "error.hpp"

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {

/*! Generic response transfer object */
class ResponseDTO {
    int m_id{};
    Error m_error{};

public:
    /*! Copy constructor */
    ResponseDTO(const ResponseDTO &) = default;

    /*! Assigment constructor */
    ResponseDTO& operator=(const ResponseDTO &) = default;

    /*! Default constructor */
    ResponseDTO() {}

    /*!
     * Sets response ID.
     *
     * @param id response unique ID.
     * */
    void set_id(const int id) { m_id = id; }

    /*!
     * Gets response ID.
     *
     * @return response unique ID.
     * */
    int get_id() const { return m_id; }

    /*!
     * Sets error.
     *
     * @param error Error which occurred when processing request.
     * */
    void set_error(const Error& error) { m_error = error; }

    /*!
     * Gets error object.
     *
     * @return Error object.
     * */
    const Error& get_error() const { return m_error; }

    /*!
     * Checks if error(s) occurred.
     *
     * @return True if error occurred, false otherwise.
     * */
    bool is_valid() const { return m_error.get_code() == Error::Code::NO_ERROR; }

    /*!
     * Deserializes response object to JSON
     *
     * @param response Response JSON
     * */
    virtual void to_object(const Json::Value& response) = 0;

    virtual ~ResponseDTO();
};

}
}
}

#endif /* RESPONSE_DTO_HPP */
