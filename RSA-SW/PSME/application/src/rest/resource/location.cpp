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

#include "psme/rest/resource/location.hpp"
#include "psme/rest/node/node.hpp"
#include "psme/rest/node/crud/drawers.hpp"
#include "psme/rest/node/crud/compute_modules.hpp"
#include "psme/rest/node/crud/fabric_modules.hpp"
#include "psme/rest/node/crud/switches.hpp"
#include "psme/rest/node/crud/blades.hpp"
#include "psme/rest/resource/resource.hpp"

using namespace psme::rest::resource;

constexpr char Location::LOCATION[];
constexpr char Location::RACK[];
constexpr char Location::POD[];
constexpr char Location::DRAWER[];
constexpr char Location::MODULE[];
constexpr char Location::BLADE[];
constexpr char Location::SWITCH[];

Location::Location() : m_json(json::Value::Type::OBJECT) {
    m_json[Location::POD] = 1;
    m_json[Location::RACK] = 1;
    m_json[Location::DRAWER] = 1;
}

using psme::rest::node::Drawer;
using psme::rest::node::ComputeModule;
using psme::rest::node::FabricModule;
using psme::rest::node::Blade;
using psme::rest::node::Switch;

namespace {
int get_location_id(const Node& node, const char* const location) {
    const auto& resources = node.get_resource().as_json();
    if (resources[Location::LOCATION][location].is_number()) {
        return resources[Location::LOCATION][location].as_int();
    }
    return std::stoi(node.get_id());;
}

}

Location& Location::resolve(const Node& node, Location& location) {
    const string type = node.get_type();
    string next_type = "v1";

    if (nullptr == node.get_back() || next_type == type) {
        return location;
    }

    Node* next = node.get_back();

    if (Blade::TYPE == type) {
        location.set_module(0);
        location.set_blade(get_location_id(node, Location::BLADE));
        next_type = ComputeModule::TYPE;
    } else if (Switch::TYPE == type) {
        location.set_module(0);
        location.set_switch(get_location_id(node, Location::SWITCH));
        next_type = FabricModule::TYPE;
    } else if (ComputeModule::TYPE == type
            || FabricModule::TYPE == type) {
        location.set_module(get_location_id(node, Location::MODULE));
        next_type = Drawer::TYPE;
    } else if (Drawer::TYPE == type) {
        location.set_drawer(get_location_id(node, Location::DRAWER));
    }

    if (!node.is_collection()) {
        // find next node
        for(const auto& link: node.get_links()) {
           if (next_type == link.m_name) {
               next = link.m_node;
               break;
           }
        }
    }
    return resolve(*next, location);
}
