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

#include "psme/rest/resource/system_type.hpp"
#include "psme/rest/utils/enum_chars.hpp"

namespace psme {
namespace rest {
namespace utils {

using namespace psme::rest::resource;

template<>
void get_enum_chars_array<SystemType>(EnumChars<SystemType>*& ptr, size_t& size) {
    static EnumChars<SystemType> g_system_type_chars[] = {
        { SystemType::PHYSICAL, "Physical"},
        { SystemType::VIRTUAL, "Virtual"},
        { SystemType::OS, "OS"},
        { SystemType::PHYSICALLY_PARTITIONED, "PhysicallyPartitioned"},
        { SystemType::VIRTUALLY_PARTITIONED, "VirtuallyPartitioned"}
    };
    ptr = g_system_type_chars;
    size = array_size(g_system_type_chars);
}

}
}
}
