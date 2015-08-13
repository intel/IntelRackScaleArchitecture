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

#include "psme/rest/node/crud/fabric_modules.hpp"
#include "psme/rest/node/crud/switches.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char FabricModules::TYPE[];

FabricModules::FabricModules(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(FabricModules::TYPE)))
{ }

FabricModules::~FabricModules() { }

constexpr const char FabricModule::TYPE[];

FabricModule::FabricModule(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(FabricModule::TYPE)))
{ }

FabricModule::~FabricModule() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<FabricModules> g_fms(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
                "#RSAFabricModule.1.0.0.RSAFabricModuleCollection"},
        {Resource::NAME, "Fabric Module Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<FabricModule> g_fm(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSAFabricModule.1.0.0.RSAFabricModule"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Fabric Module"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(Switches::TYPE, json::Value::Type::OBJECT),
                Pair(Resource::CONTAINED_BY, json::Value::Type::OBJECT),
                Pair(Resource::MANAGED_BY, json::Value::Type::ARRAY),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })},
        {Resource::ACTIONS, json::Value::Type::OBJECT}
    }});
