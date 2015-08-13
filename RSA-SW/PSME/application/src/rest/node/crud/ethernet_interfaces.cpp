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

#include "psme/rest/node/crud/ethernet_interfaces.hpp"
#include "psme/rest/node/crud/vlans.hpp"
#include "psme/rest/node/crud/drawers.hpp"
#include "psme/rest/node/crud/managers.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"
#include "psme/utils/network_interface_info.hpp"
#include "configuration/configuration.hpp"
#include "json/value.hpp"

#include <algorithm>

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char EthernetInterfaces::TYPE[];

EthernetInterfaces::EthernetInterfaces(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(EthernetInterfaces::TYPE)))
{ }

EthernetInterfaces::~EthernetInterfaces() { }

constexpr const char EthernetInterface::TYPE[];

EthernetInterface::EthernetInterface(const string& uuid,
        const string& gami_id,
        const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(EthernetInterface::TYPE)))
{ }

EthernetInterface::~EthernetInterface() { }

void EthernetInterface::get(const Request& request, Response& response) {
    auto* manager = find_back_if(
        [](const Node& n) { return n.get_type() == Manager::TYPE;});
    if (nullptr != manager) {
        const auto& lnks = manager->get_links();
        if (std::end(lnks) != std::find_if(std::begin(lnks), std::end(lnks),
                [] (const Link & link) {
                    return link.m_node->get_type() == Drawer::TYPE
                            && link.m_name == Manager::MANAGER_FOR_DRAWERS;
                })) {
            const json::Value& config =
                        configuration::Configuration::get_instance().to_json();
            const auto& nic_name =
                        config["server"]["network-interface-name"].as_string();
            psme::utils::NetworkInterfaceInfo nic_info(nic_name);
            try {
                const auto& nic_address = nic_info.get_interface_address();
                json::Value ip4address = json::Value::Type::OBJECT;
                ip4address["Address"] = nic_address.get_ip_address();
                ip4address["SubnetMask"] = nic_address.get_netmask();
                json::Value ip4address_array = json::Value::Type::ARRAY;
                ip4address_array.push_back(std::move(ip4address));
                get_resource().set_property("IPv4Addresses", std::move(ip4address_array));
                get_resource().set_property("MacAddress", nic_address.get_mac_address());
            } catch (const std::exception& ex) {
                log_error(GET_LOGGER("rest"), "Unable to read network address: "
                        << ex.what());
            }
        }
    }
    return Node::get(request, response);
}

using namespace psme::rest::resource;
using json::Pair;

static ResourceDefinitions::Register<EthernetInterfaces> g_nics(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
            "#EthernetNetworkInterface.1.0.0.EthernetNetworkInterfaceCollection"},
        {Resource::NAME, "Ethernet Network Interface Collection"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::LINKS,
            json::Value(
            {
                Pair(Resource::MEMBERS_ODATA_COUNT, 0),
                Pair(Resource::MEMBERS, json::Value::Type::ARRAY)
            })}
    }});

static ResourceDefinitions::Register<EthernetInterface> g_nic(ResourceDef{
    PropertyVec{
        {Resource::ODATA_CONTEXT, json::Value::Type::STRING},
        {Resource::ODATA_ID, json::Value::Type::STRING},
        {Resource::ODATA_TYPE,
            "#EthernetNetworkInterface.1.0.0.EthernetNetworkInterface"},
        {Resource::ID, json::Value::Type::STRING},
        {Resource::NAME, "Manager NIC"},
        {Resource::MODIFIED, json::Value::Type::STRING},
        {Resource::DESCRIPTION, json::Value::Type::STRING},
        {Status::STATUS, json::Value::Type::OBJECT},
        {"FactoryMacAddress", json::Value::Type::STRING},
        {"MacAddress", json::Value::Type::STRING},
        //{"LinkTechnology", "Ethernet"},
        {"SpeedMbps", json::Value::Type::NUMBER},
        {"Autosense", json::Value::Type::BOOLEAN},
        {"FullDuplex", json::Value::Type::BOOLEAN},
        {"FrameSize", json::Value::Type::NUMBER},
        {"HostName", json::Value::Type::STRING},
        {"FQDN", json::Value::Type::STRING},
        {"MaxIPv6StaticAddresses", json::Value::Type::NUMBER},
        {"VLANEnable", json::Value::Type::BOOLEAN},
        {"VLANId", json::Value::Type::NUMBER},
        {"IPv4Addresses", json::Value::Type::ARRAY},
        {"IPv6AddressPolicyTable", json::Value::Type::ARRAY},
        {"IPv6StaticAddresses", json::Value::Type::ARRAY},
        {"IPv6DefaultGateway", json::Value::Type::STRING},
        {"IPv6Addresses", json::Value::Type::ARRAY},
        {"NameServers", json::Value::Type::ARRAY},
        {Resource::LINKS, json::Value({
            Pair{Resource::OEM, json::Value::Type::OBJECT}
        })}
    }});
