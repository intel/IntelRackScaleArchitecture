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
 * @file error.hpp
 *
 * @brief JsonRpc Error class implementation
 * */

#ifndef PSME_CORE_DTO_ERROR_HPP
#define PSME_CORE_DTO_ERROR_HPP

#include <json/json.h>
#include <string>

/*! PSME namespace */
namespace psme {
/*! Core namespace */
namespace core {
/*! Dto namespace */
namespace dto {

    /*! JsonRpc Error */
    class Error {
    public:
        /*! Error codes */
        enum Code : int {
            INVALID_PARAMS = -32602,
            CONNECTION_ERROR = -32003,
            NO_ERROR = 0,
            UNKNOWN_ERROR,
            REMOTE_TARGET_ERROR = 20000,
            LOGICAL_DRIVE_ERROR = 30000,
        };

        /*! @brief Default Constructor */
        Error();

        /*! @brief Constructor
         * @param code Error code
         * @param msg Error message
         * @param data Error additional data
         */
        Error(int code, const std::string& msg = "", const Json::Value& data = {});

        /*!
        * @brief Error code getter.
        *
        * @return Error code.
        * */
        int get_code() const;

       /*!
        * @brief Error message getter.
        *
        * @return Error message.
        * */
        const std::string& get_message() const;

        /*!
        * @brief Error data getter.
        *
        * @return Additional error data.
        * */
        const Json::Value& get_data() const;

       /*!
        * @brief Error as JSON object.
        *
        * @return JSON representation of the error.
        * */
        Json::Value as_json() const;

       /*!
        * @brief Error as string.
        *
        * Serialized JSON representation.
        *
        * @return String representation of the error.
        * */
        std::string to_string() const;

    private:
        /*! Error code */
        int m_code;
        /*! Error message */
        std::string m_message;
        /*! Error data */
        Json::Value m_data;
    };
}
}
}
#endif /* PSME_CORE_DTO_ERROR_HPP */
