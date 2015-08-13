/*
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.intel.rsa.podm.rest.representation.json.templates;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.LinkTechnology;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.NetworkIPv4AddressJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.NetworkIPv6AddressJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.NetworkIpV6AddressPolicyJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.LinkedList;
import java.util.List;

@JsonPropertyOrder({
        "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified", "description", "status", "factoryMacAddress",
        "macAddress", "linkTechnology", "speedMbps", "autosense", "fullDuplex", "frameSize", "hostName", "fqdn",
        "maxIPv6StaticAddresses", "vlanEnable", "vlanId"
})
public class NetworkInterfaceJson extends BaseJson {
    public Id id;
    public String name;
    public String description;
    public final StatusJson status = new StatusJson();
    public String factoryMacAddress;
    public String macAddress;
    public LinkTechnology linkTechnology;
    public Integer speedMbps;
    public Boolean autosense;
    public Boolean fullDuplex;
    public Integer frameSize;
    public String hostName;
    @JsonProperty("FQDN")
    public String fqdn;
    public Integer maxIPv6StaticAddresses;
    @JsonProperty("VLANEnable")
    public Boolean vlanEnable;
    @JsonProperty("VLANId")
    public Integer vlanId;
    public final List<NetworkIPv4AddressJson> iPv4Addresses = new LinkedList<>();
    public final List<NetworkIpV6AddressPolicyJson> iPv6AddressPolicyTable = new LinkedList<>();
    public final List<NetworkIPv6AddressJson> iPv6StaticAddresses = new LinkedList<>();
    public String iPv6DefaultGateway = "";
    public final List<NetworkIPv6AddressJson> iPv6Addresses = new LinkedList<>();
    public final List<String> nameServers = new LinkedList<>();
    public final Links links = new Links();

    public NetworkInterfaceJson() {
        super("#EthernetNetworkInterface.1.0.0.EthernetNetworkInterface");
    }

    public static final class Links extends BaseLinksJson {
    }
}
