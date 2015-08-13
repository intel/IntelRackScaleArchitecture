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

#include "psme/rest/node/crud/compute_modules.hpp"
#include "psme/rest/node/crud/blades.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"

#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char ComputeModules::TYPE[];

ComputeModules::ComputeModules(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(ComputeModules::TYPE)))
{ }

ComputeModules::~ComputeModules() { }

constexpr const char ComputeModule::TYPE[];

ComputeModule::ComputeModule(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(ComputeModule::TYPE)))
{ }

ComputeModule::~ComputeModule() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<ComputeModules> g_cms(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, "/rest/v1/$metadata#RSAComputeModules"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
                "#RSAComputeModule.1.0.0.RSAComputeModuleCollection"},
        {Resource::NAME, "Compute Module Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<ComputeModule> g_cm(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT,
                "/rest/v1/$metadata#RSAComputeModules/Links/Members/$entity"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAComputeModule.1.0.0.RSAComputeModule"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Sled"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"ChassisType", json::Value::Type::STRING},
        {"Manufacturer", json::Value::Type::STRING},
        {"Model", json::Value::Type::STRING},
        {"SerialNumber", json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Resource::ENUMERATED, json::Value::Type::STRING},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(Blades::TYPE, json::Value::Type::OBJECT),
                Pair(Resource::CONTAINED_BY, json::Value::Type::OBJECT),
                Pair(Resource::MANAGED_BY, json::Value::Type::ARRAY),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});
