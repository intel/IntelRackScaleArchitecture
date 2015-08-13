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
 * @file exception.hpp
 *
 * @brief Throw macro
 * */

#ifndef AGENT_FRAMEWORK_EXCEPTIONS_EXCEPTION_HPP
#define AGENT_FRAMEWORK_EXCEPTIONS_EXCEPTION_HPP

#include "agent-framework/exceptions/invalid_parameters.hpp"
#include "agent-framework/exceptions/not_found.hpp"
#include "agent-framework/exceptions/lvm_error.hpp"
#include "agent-framework/exceptions/iscsi_error.hpp"

#define THROW(class, logger, ...) \
    do { \
        log_error(GET_LOGGER((logger)), #class" : " << __VA_ARGS__); \
        throw class{__VA_ARGS__}; \
    } while (0)

#endif	/* AGENT_FRAMEWORK_EXCEPTIONS_EXCEPTION_HPP */
