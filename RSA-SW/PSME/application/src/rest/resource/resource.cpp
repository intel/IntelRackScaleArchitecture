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
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/resource/resource_utils.hpp"
#include "psme/rest/resource/resource_definition.hpp"
#include "psme/rest/node/node.hpp"
#include "psme/rest/utils/enum_chars.hpp"
#include "logger_ext.hpp"

#include <algorithm>
#include <cstring>

using namespace psme::rest::resource;
using namespace psme::rest::node;

constexpr char Resource::ODATA_CONTEXT[];
constexpr char Resource::ODATA_ID[];
constexpr char Resource::ODATA_TYPE[];
constexpr char Resource::DESCRIPTION[];
constexpr char Resource::MODIFIED[];
constexpr char Resource::NAME[];
constexpr char Resource::LINKS[];
constexpr char Resource::MEMBERS[];
constexpr char Resource::MEMBERS_ODATA_COUNT[];
constexpr char Resource::MEMBERS_NEXT_LINK[];
constexpr char Resource::ID[];
constexpr char Resource::OEM[];
constexpr char Resource::ENUMERATED[];
constexpr char Resource::ACTIONS[];
constexpr char Resource::ACTION_TARGET[];
constexpr char Resource::MANAGED_BY[];
constexpr char Resource::CONTAINED_BY[];
constexpr char Resource::CHASSIS[];

Resource::Resource(const char* type)
        : m_json(json::Value::Type::OBJECT),
          m_update_cache(true),
          m_resource_def(ResourceDefinitions::get_instance()
                                        .get_resource_def(type)) {
    for (auto property: m_resource_def.m_property_vec) {
        m_json[property.m_name] = property.m_default_value;
    }
}

void Resource::update_ids(const Node& node) {
    if (m_json.is_member(ODATA_ID)) {
        m_json[ODATA_ID] = node.get_path();
    }
    if (m_json.is_member(ID)) {
        m_json[ID] = node.get_id();
    }
}

void Resource::update_modified() {
    m_update_cache = true;
    if (m_json.is_member(MODIFIED)) {
        m_json[MODIFIED] = ResourceUtils::get_time_with_zone();
    }
}

void Resource::update_actions(Node& node) {
    if (m_json.is_member(ACTIONS)) {
        for(auto& child: node) {
            if (0 == strcmp(ACTIONS, child.get_type())) {
                for (auto& action: child) {
                    auto& r = action.get_resource();
                    r.set_property(ACTION_TARGET, action.get_path());
                    m_json[ACTIONS]["#"+string(action.get_type())] = r.as_json();
                }
            }
        }
    }
}

namespace {
void luns_drive_update(const std::string& path, json::Value& json) {
    for (auto& target_lun : json) {
        target_lun["Drive"] = path + target_lun["Drive"].as_string();
    }
}
}

void Resource::update_target_drive_path(Node& node) {
    if (!strcmp(node.get_type(), "Target")) {
        if (node.get_back() && node.get_back()->get_back()) {
            const auto& logical_drives =
                    node.get_back()->get_back()->get_node_by_id("LogicalDrives");
            const auto& path = logical_drives.get_path();
            for (auto& address : m_json["Addresses"].as_array()) {
                if (address["iSCSI"].is_object()) {
                    luns_drive_update(path, address["iSCSI"]["TargetLUN"]);
                }
            }
        }
    }
}

void Resource::set_location(const Location& location) {
    m_json[Location::LOCATION] = location.as_json();
}

void Resource::set_status(const Status& status) {
    m_json[Status::STATUS] = status.as_json();
}

void Resource::set_enumerated(EnumStatus enum_status) {
    m_json[Resource::ENUMERATED] = psme::rest::utils::to_string(enum_status);
}

void Resource::update_location(const Node& node) {
    if (m_json.is_member(Location::LOCATION)) {
        Location location;
        set_location(Location::resolve(node, location));
    }
}

void Resource::update_links(const Node& node) {
    auto idx = find_property_idx(Resource::LINKS);
    if (npos != idx) {
        const auto& property = m_resource_def.m_property_vec[idx];
        // reset
        m_json[Resource::LINKS] = property.m_default_value;
        // update
        if (node.is_collection()) {
            m_json[Resource::LINKS][Resource::MEMBERS_ODATA_COUNT]
                    = unsigned(node.size());
        }
        const auto& linked_nodes = node.get_links();
        std::for_each(std::begin(linked_nodes), std::end(linked_nodes),
            [this, &property](const Node::Link& link) {
                auto it = property.m_default_value.cbegin();
                for (; it != property.m_default_value.cend(); ++it) {
                    if (link.m_name == it.key()) {
                        json::Value item;
                        item[Resource::ODATA_ID] = link.m_node->get_path();
                        auto& container = m_json[Resource::LINKS][it.key()];
                        if (container.is_array()) {
                            container += std::move(item);
                        } else {
                            container = std::move(item);
                        }
                    }
                }
            }
        );
    }
}

/*! @brief Updates node's Target Addresses Drive property. **/
void Resource::update_json_properties(Node& node) {
    if (m_update_cache) {
        m_update_cache = false;
        update_ids(node);
        update_location(node);
        update_links(node);
        update_actions(node);
        update_target_drive_path(node);
    }
}

const json::Value& Resource::patch(const json::Value& patch_value) {
    if (!patch_value.is_object()) {
        throw std::runtime_error("JSON Value should be an OBJECT.");
    }

    for(auto it = patch_value.begin(); it != patch_value.end(); it++) {
        set_property(it.key(), *it);
    }

    return m_json;
}

size_t Resource::find_property_idx(const std::string& key) const {
    for (size_t i = 0; i < m_resource_def.m_property_vec.size(); ++i) {
        if (m_resource_def.m_property_vec[i].m_name == key) {
            return i;
        }
    }
    return npos;
}

bool
Resource::is_valid(const Property& property, const json::Value& value) const {
    return property.m_default_value.get_type() == value.get_type();
}
