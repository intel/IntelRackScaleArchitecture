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

#include "collections_dto.hpp"

using namespace psme::core::dto;

void CollectionsDTO::Response::to_object(const Json::Value& response) {
    if (response.isArray()) {
        for (const auto& val : response) {
            Collection entry{val["name"].asString(),
                                  val["type"].asString()};
            entry.set_slot_mask(val["slotMask"].asString());
            add_collection(std::move(entry));
        }
    }
}

void CollectionsDTO::Collection::set_name(const std::string& name) {
    m_name = name;
}

const std::string& CollectionsDTO::Collection::get_name() const {
    return m_name;
}

void CollectionsDTO::Collection::set_type(const std::string& type) {
    m_type = type;
}

const std::string& CollectionsDTO::Collection::get_type() const {
    return m_type;
}

void CollectionsDTO::Collection::set_slot_mask(const std::string& slot_mask) {
    m_slot_mask = slot_mask;
}

const std::string& CollectionsDTO::Collection::get_slot_mask() const {
    return m_slot_mask;
}

void CollectionsDTO::Response::set_collections(
    const CollectionsDTO::Response::collections_t& collections) {
    m_collections = collections;
}

const CollectionsDTO::Response::collections_t&
CollectionsDTO::Response::get_collections() const {
    return m_collections;
}

void CollectionsDTO::Response::add_collection(
    const CollectionsDTO::Collection& entry) {
    m_collections.emplace_back(entry);
}

CollectionsDTO::~CollectionsDTO(){}

CollectionsDTO::Response::~Response(){}
