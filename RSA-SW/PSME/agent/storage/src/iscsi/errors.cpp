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
*/

#include "iscsi/errors.hpp"
#include <array>

using namespace agent::storage::iscsi::tgt;

namespace {
/*! Gloabal array for error string descriptions */
std::array<const char*, 25> g_error_array = {
{
    "Success",
    "Out of memory",
    "Unknown error",
    "Can't find the driver",
    "Can't find the target",
    "Can't find the logical unit",
    "Can't find the session",
    "Can't find the conneciton",
    "Can't find the binding",
    "Target already exists",
    "Binding already exists",
    "Logical unit number already exists",
    "Access control rule already exists",
    "Access control does not exists",
    "Account already exists",
    "Can't find the account",
    "Too many account",
    "Invalid request",
    "Target already has an outgoing account",
    "Target is still active",
    "Logical unit is still active",
    "Drive is busy",
    "Operation isn't supported",
    "Unknown parameter",
    "Device has prevent removal set"
    }
};
}

std::string Errors::get_error_str(const std::uint32_t ec) {
    if (ec < g_error_array.size()) {
       return std::string(g_error_array[ec]);
    }

    return "unknown request error";
}

std::string Errors::get_error_str(const Errors::Types error) {
    return Errors::get_error_str(std::uint32_t(error));
}

