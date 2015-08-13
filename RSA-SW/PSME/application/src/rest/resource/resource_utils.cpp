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
 * */
#include "psme/rest/resource/resource_utils.hpp"
#include "bits/basic_string.h"

#include <stdexcept>
#include <ctime>
#include <locale>

using namespace psme::rest::resource;

static constexpr size_t TIME_BUFFER_SIZE = 26;

std::string ResourceUtils::get_time() {
    return get_time("%FT%H:%M");
}

std::string ResourceUtils::get_time_with_zone() {
    std::string time = get_time("%FT%T%z");
    if (2 < time.length())
        return time.insert(time.length()-2, ":");
    else
        return time;
}

std::string ResourceUtils::get_time(const char* format) {
    char time_buffer[TIME_BUFFER_SIZE]{};
    time_t t = time(nullptr);
    struct tm * local_tm = std::localtime(&t);

    if (nullptr == local_tm) {
        throw std::runtime_error("cannot get local time");
    }

    std::strftime(time_buffer, TIME_BUFFER_SIZE, format, local_tm);
    return time_buffer;
}
