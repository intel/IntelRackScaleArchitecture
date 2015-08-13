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

#include "psme/rest/resource/chassis_type.hpp"
#include "psme/rest/utils/enum_chars.hpp"

namespace psme {
namespace rest {
namespace utils {

using namespace psme::rest::resource;

template<>
void get_enum_chars_array<ChassisType>(EnumChars<ChassisType>*& ptr, size_t& size) {
    static EnumChars<ChassisType> g_chassis_type_chars[] = {
        { ChassisType::RACK, "Rack"},
        { ChassisType::BLADE, "Blade"},
        { ChassisType::ENCLOSURE, "Enclosure"},
        { ChassisType::STAND_ALONE, "STAND_ALONE"},
        { ChassisType::RACK_MOUNT, "RackMount"},
        { ChassisType::CARD, "Card"},
        { ChassisType::CARTRIDGE, "Cartridge"},
        { ChassisType::ROW, "Row"},
        { ChassisType::POD, "Pod"},
        { ChassisType::EXPANSION, "Expansion"},
        { ChassisType::SIDECAR, "Sidecar"},
        { ChassisType::ZONE, "Zone"},
        { ChassisType::SLED, "Sled"},
        { ChassisType::SHELF, "Shelf"},
        { ChassisType::DRAWER, "Drawer"},
        { ChassisType::OTHER, "Other"}
    };
    ptr = g_chassis_type_chars;
    size = array_size(g_chassis_type_chars);
}

}
}
}
