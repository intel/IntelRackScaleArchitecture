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

#include "psme/rest/node/crud/drawers.hpp"
#include "psme/rest/node/crud/compute_modules.hpp"
#include "psme/rest/node/crud/fabric_modules.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

using psme::rest::http::HttpHeaders;

constexpr const char Drawers::TYPE[];

Drawers::Drawers(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Drawers::TYPE)))
{ }

Drawers::~Drawers() { }

constexpr const char Drawer::TYPE[];

Drawer::Drawer(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Drawer::TYPE)))
{ }

Drawer::~Drawer() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<Drawers> g_drawers(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, "/rest/v1/$metadata#RSADrawers"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSADrawer.1.0.0.RSASDrawerCollection"},
        {Resource::NAME, "Drawers Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<Drawer> g_drawer(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT,
                "/rest/v1/$metadata#RSADrawers/Links/Members/$entity"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSADrawer.1.0.0.RSASDrawer"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "RSA Drawer"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {"ChassisType", json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Resource::ENUMERATED, json::Value::Type::STRING},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair(ComputeModules::TYPE, json::Value::Type::OBJECT),
                Pair(FabricModules::TYPE, json::Value::Type::OBJECT),
                Pair(Resource::MANAGED_BY, json::Value::Type::ARRAY),
                Pair(Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});

