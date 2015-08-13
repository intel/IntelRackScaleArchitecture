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

#include "psme/rest/node/crud/volumes.hpp"
#include "psme/rest/node/crud/drives.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char Volumes::TYPE[];

Volumes::Volumes(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Volumes::TYPE)))
{ }

Volumes::~Volumes() { }

constexpr const char Volume::TYPE[];

Volume::Volume(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Volume::TYPE)))
{ }

Volume::~Volume() { }

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<Volumes> g_volumes(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSALogicalVolumes.1.0.0.RSALogicalVolume"},
        {Resource::NAME, "Logical Volumes Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<Volume> g_volume(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSALogicalVolume.1.0.0.RSALogicalVolume"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "RSA Logical Volume"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair{Resource::MANAGED_BY, json::Value::Type::ARRAY },
                Pair{"UsedBy", json::Value::Type::ARRAY},
                Pair{Drives::TYPE, json::Value::Type::ARRAY},
                Pair{Resource::OEM, json::Value::Type::ARRAY}
            })}
    }});
