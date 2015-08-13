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

#include "psme/rest/node/crud/drives.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char Drives::TYPE[];

Drives::Drives(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Drives::TYPE)))
{ }

Drives::~Drives() { }

std::string Drives::generate_child_id() const {
    static std::uint32_t id{0};
    return std::to_string(++id);
}

constexpr const char Drive::TYPE[];

Drive::Drive(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Drive::TYPE)))
{ }

Drive::~Drive() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<Drives> g_drives(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#Drive.1.0.0.DrivesCollection"},
        {Resource::NAME, "Drives Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<Drive> g_drive(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#Drive.1.0.0.Drive"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Simple Drive"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"Interface", json::Value::Type::STRING},
        {"CapacityGB", json::Value::Type::NUMBER},
        {"Type", json::Value::Type::STRING},
        {"RPM", json::Value::Type::NUMBER},
        {"Manufacturer", json::Value::Type::STRING},
        {"Model", json::Value::Type::STRING},
        {"SerialNumber", json::Value::Type::STRING},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::CONTAINED_BY, json::Value::Type::OBJECT),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});
