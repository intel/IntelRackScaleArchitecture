/*!
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
 *
 * @file error_codes.hpp
 *
 * @brief Error codes specification
 * */

#ifndef AGENT_FRAMEWORK_EXCEPTIONS_ERROR_CODES_HPP
#define AGENT_FRAMEWORK_EXCEPTIONS_ERROR_CODES_HPP

namespace agent_framework {
namespace exceptions {

/*! Error codes */
enum ErrorCode {
    InvalidParameter = -32602,
    NoError = 0,
    NotFound = 10000,

    ISCSI = 20000,
    LVM = 30000
};

}
}
#endif	/* AGENT_FRAMEWORK_EXCEPTIONS_ERROR_CODES_HPP */

