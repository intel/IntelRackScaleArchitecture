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

#include "psme/rest/resource/resource_definition.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/resource/status.hpp"
#include "psme/rest/resource/location.hpp"

using namespace psme::rest::resource;

constexpr const char ResourceDefinitions::UNDEFINED[];

static ResourceDefinitions* g_resource_definition_registry = nullptr;

ResourceDefinitions& ResourceDefinitions::get_instance() {
    if (nullptr == g_resource_definition_registry) {
        g_resource_definition_registry = new ResourceDefinitions;
    }
    return *g_resource_definition_registry;
}

ResourceDefinitions::ResourceDefinitions()
    : m_resource_map() {
    m_resource_map[UNDEFINED] = ResourceDef{};
}

const ResourceDef& ResourceDefinitions::get_resource_def(const char* type) {
    ResourceDefinitions& registry = ResourceDefinitions::get_instance();
    auto it = registry.m_resource_map.find(type);
    if (it != registry.m_resource_map.end()) {
        return it->second;
    }
    return registry.m_resource_map.at(UNDEFINED);
}

void ResourceDefinitions::add(const string& type, const ResourceDef& resource_def) {
    get_instance().m_resource_map.insert(std::make_pair(type, resource_def));
}
