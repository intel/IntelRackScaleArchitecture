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

#include "psme/rest/node/crud/memory_modules.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char MemoryModules::TYPE[];

MemoryModules::MemoryModules(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(MemoryModules::TYPE)))
{ }

MemoryModules::~MemoryModules() { }

constexpr const char MemoryModule::TYPE[];

MemoryModule::MemoryModule(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(MemoryModule::TYPE)))
{ }

MemoryModule::~MemoryModule() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<MemoryModules> g_ms(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAMemory.1.0.0.RSAMemoryCollection"},
        {Resource::NAME, "Memory Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value(
            {
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<MemoryModule> g_m(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAMemory.1.0.0.RSAMemory"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "MemoryModule"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"Manufacturer", json::Value::Type::STRING},
        {"Socket", json::Value::Type::STRING},
        {"Bank", json::Value::Type::STRING},
        {"Type", json::Value::Type::STRING},
        {"SizeGB", json::Value::Type::NUMBER},
        {"SpeedMHz", json::Value::Type::NUMBER},
        {"VoltageVolt", json::Value::Type::NUMBER},
        {"DataWidthBits", json::Value::Type::NUMBER},
        {"TotalWidthBits", json::Value::Type::NUMBER},
        {"FormFactor", json::Value::Type::STRING},
        {"SerialNumber", json::Value::Type::STRING},
        {"AssetTag", json::Value::Type::STRING},
        {"PartNumber", json::Value::Type::STRING},
        {"Rank", json::Value::Type::STRING},
        {"ConfiguredSpeedMHz", json::Value::Type::NUMBER},
        {"MinimumVoltageVolt", json::Value::Type::NUMBER},
        {"MaximumVoltageVolt", json::Value::Type::NUMBER},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::CONTAINED_BY, json::Value::Type::OBJECT),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});
