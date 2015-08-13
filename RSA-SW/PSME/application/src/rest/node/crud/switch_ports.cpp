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

#include "psme/rest/node/crud/switch_ports.hpp"
#include "psme/rest/node/crud/utils.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "psme/rest/error/error_factory.hpp"
#include "psme/rest/error/server_exception.hpp"
#include "core/service/service_factory.hpp"
#include "core/service/network_service.hpp"
#include "json/value.hpp"

using namespace psme::rest::node;
using namespace psme::rest::error;
using psme::rest::resource::ResourceUPtr;

namespace {
    constexpr const char LinkSpeedGbps[] = "LinkSpeedGbps";
    constexpr const char AdministrativeState[] = "AdministrativeState";
    constexpr const char OperationalState[] = "OperationalState";
    constexpr const char AutoSense[] = "AutoSense";
    constexpr const char FrameSize[] = "FrameSize";
}

constexpr const char SwitchPorts::TYPE[];
constexpr const char SwitchPorts::PORTS[];

SwitchPorts::SwitchPorts(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(SwitchPorts::TYPE)))
{ }

SwitchPorts::~SwitchPorts() { }

constexpr const char SwitchPort::TYPE[];
constexpr const char SwitchPort::NEIGHBOUR_PORT[];
constexpr const char SwitchPort::LINK_SPEED_GBPS[];
constexpr const char SwitchPort::OPERATIONAL_STATE[];
constexpr const char SwitchPort::ADMINISTRATIVE_STATE[];

SwitchPort::SwitchPort(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(SwitchPort::TYPE)))
{ }

SwitchPort::~SwitchPort() { }

using namespace psme::rest::resource;
using json::Pair;

namespace {
void read_switch_port_attributes(Node& node) {
     // execute JSON-RPC command
    auto service = psme::core::service::ServiceFactory::create_network(node.get_gami_id());
    auto switch_port_info =
            service.get_switch_port_info(node.get_back()->get_back()->get_uuid(),
                    node.get_uuid());

    json::Value json;
    json[AdministrativeState] = switch_port_info.get_administrative_state();
    json[LinkSpeedGbps] = switch_port_info.get_link_speed_gbps();

    node.get_resource().patch(json);
}
}

void SwitchPort::patch(const Request& request, Response& response) {
    json::Value schema({
        Pair(AdministrativeState, json::Value({
            Pair("validator", true),
            Pair("type", "string"),
            Pair("anyof", json::Value{"Up", "Down"})})
        ),
        Pair(LinkSpeedGbps, json::Value({
            Pair("validator", true),
            Pair("type", "uint"),
            Pair("min", 0)})
        ),
        Pair(FrameSize, json::Value({
            Pair("validator", true),
            Pair("type", "uint"),
            Pair("min", 0)})
        ),
        Pair(AutoSense, json::Value({
            Pair("validator", true),
            Pair("type", "bool")})
        )
    });
    auto json = validate_request_body(request, schema);

    std::uint32_t linkSpeedGbps = 1;
    if (json.is_member(LinkSpeedGbps)) {
        linkSpeedGbps = json[LinkSpeedGbps].as_uint();
    }
    string link_state = "Up";
    if (json.is_member(AdministrativeState)) {
        link_state = json[AdministrativeState].as_string();
    }
    bool auto_sense = false;
    if (json.is_member(AutoSense)) {
        auto_sense = json[AutoSense].as_bool();
    }
    std::uint32_t frame_size = 1536;
    if (json.is_member(FrameSize)) {
        frame_size = json[FrameSize].as_uint();
    }

    // execute JSON-RPC command
    auto service = core::service::ServiceFactory::create_network(get_gami_id());

    service.set_switch_port_attributes(
                             get_back()->get_back()->get_uuid(), // switch uuid
                             get_uuid(),  // switch port identifier
                             linkSpeedGbps,
                             link_state,
                             auto_sense,
                             frame_size,
                             psme::core::dto::OEMDataDTO::Request());

    // normally we would use patch, but set_switch_port_attributes need to be
    // improved
    read_switch_port_attributes(*this);
    get_resource().update_modified();

    response.set_reply(http::HttpStatusCode::OK);
}

static ResourceDefinitions::Register<SwitchPorts> g_switch_ports(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, "/rest/v1/$metadata#RSASwitchPorts"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSASwitchPort.1.0.0.RSASwtichPortsCollection"},
        {Resource::NAME, "RSA Switch Ports Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value({
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
        }});

static ResourceDefinitions::Register<SwitchPort> g_switch_port(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT,
                "/rest/v1/$metadata#RSASwitchPorts/Links/Members/$entity"},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE, "#RSASwitchPort.1.0.0.RSASwtichPort"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "RSA Switch Port"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {"LinkType", "Ethernet"},
        {OperationalState, "Unknown"},
        {AdministrativeState, "None"},
        {LinkSpeedGbps, json::Value::Type::NUMBER},
        {"NeighbourPort", json::Value::Type::STRING},
        {Location::LOCATION, json::Value::Type::OBJECT},
        {Resource::OEM, json::Value::Type::OBJECT},
        {Resource::LINKS,
            json::Value({
                Pair( Resource::CONTAINED_BY, json::Value::Type::OBJECT ),
                Pair( "VLANs", json::Value::Type::OBJECT ),
                Pair( Resource::OEM, json::Value::Type::OBJECT)
            })}
    }});
