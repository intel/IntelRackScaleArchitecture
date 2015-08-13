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
 * @file chassis_type.hpp
 *
 * @brief Definition of ChassisType enum class
 * */

#ifndef CHASSIS_TYPE_HPP
#define CHASSIS_TYPE_HPP

/*! Psme namespace */
namespace psme {
/*! Rest namespace */
namespace rest {
/*! Resource namespace */
namespace resource {

/*!
 * @enum ChassisType
 * @brief Type of asset
 *
 * @var ChassisType RACK
 * Rack chassis type.
 *
 * @var ChassisType BLADE
 * Blade chassis type.
 *
 * @var ChassisType ENCLOSURE
 * Enclosure chassis type.
 *
 * @var ChassisType STAND_ALONE
 * StandAlone chassis type.
 *
 * @var ChassisType RACK_MOUNT
 * Rack RackMount.
 *
 * @var ChassisType CARD
 * Card chassis type.
 *
 * @var ChassisType CARTRIDGE
 * Cartridge chassis type.
 *
 * @var ChassisType ROW
 * Row chassis type.
 *
 * @var ChassisType POD
 * Pod chassis type.
 *
 * @var ChassisType EXPANSION
 * Expansion chassis type.
 *
 * @var ChassisType SIDECAR
 * Sidecar chassis type.
 *
 * @var ChassisType ZONE
 * Zone chassis type.
 *
 * @var ChassisType SLED
 * Sled chassis type.
 *
 * @var ChassisType SHELF
 * Shelf chassis type.
 *
 * @var ChassisType DRAWER
 * Drawer chassis type.
 *
 * @var ChassisType OTHER
 * Other chassis type.
 * */
enum class ChassisType {
    RACK,
    BLADE,
    ENCLOSURE,
    STAND_ALONE,
    RACK_MOUNT,
    CARD,
    CARTRIDGE,
    ROW,
    POD,
    EXPANSION,
    SIDECAR,
    ZONE,
    SLED,
    SHELF,
    DRAWER,
    OTHER
};

} /*! Resource namespace */
} /*! Rest namespace */
} /*! Psme namespace */
#endif	/* CHASSIS_TYPE_HPP */
