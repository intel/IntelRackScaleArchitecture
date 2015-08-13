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

#include "psme/rest/node/crud/switches.hpp"
#include "psme/rest/node/crud/switch_ports.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char Switches::TYPE[];

Switches::Switches(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Switches::TYPE)))
{ }

Switches::~Switches() { }

constexpr const char Switch::TYPE[];

Switch::Switch(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(Switch::TYPE)))
{ }

Switch::~Switch() { }

using namespace psme::rest::resource;

static ResourceDefinitions::Register<Switches> g_switches(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSASwitch.1.0.0.RSASwtichCollection"},
        {Resource::NAME, "RSA Switches Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                json::Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<Switch> g_switch(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSASwitch.1.0.0.RSASwtich"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "RSA Switch"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::ACTIONS, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                json::Pair(SwitchPorts::TYPE, json::Value::Type::OBJECT),
                json::Pair(Resource::CONTAINED_BY, json::Value::Type::OBJECT),
                json::Pair(Resource::MANAGED_BY, json::Value::Type::ARRAY),
                json::Pair(Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});

