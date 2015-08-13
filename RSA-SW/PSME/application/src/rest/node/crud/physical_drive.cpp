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

#include "psme/rest/node/crud/physical_drive.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char PhysicalDrive::TYPE[];

PhysicalDrive::PhysicalDrive(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(PhysicalDrive::TYPE)))
{ }

PhysicalDrive::~PhysicalDrive() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<PhysicalDrive> g_drive(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#PhysicalDrive.1.0.0.PhysicalDrive"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Simple PhysicalDrive"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"Interface", json::Value::Type::STRING},
        {"CapacityGB", json::Value::Type::NUMBER},
        {"Type", json::Value::Type::STRING},
        {"RPM", json::Value::Type::NUMBER},
        {"Manufacturer", json::Value::Type::STRING},
        {"Model", json::Value::Type::STRING},
        {"SerialNumber", json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair("UsedBy", json::Value::Type::ARRAY),
                Pair(Resource::MANAGED_BY, json::Value::Type::ARRAY),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});
