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
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.ArrayList;
import java.util.Collection;

@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
    "description", "status", "vlanEnabled", "vlanId", "iPv4Addresses", "iPv6Addresses", "links"
})
public final class VlanNetworkInterfaceJson extends BaseJson {

    public Id id;
    public String name;
    public String description;
    public final StatusJson status = new StatusJson();

    @JsonProperty("VLANEnable")
    public Boolean vlanEnabled;
    @JsonProperty("VLANId")
    public Integer vlanId;

    @JsonProperty("iPv4Address")
    public Collection<IpV4Address> iPv4Addresses = new ArrayList<>();
    @JsonProperty("iPv6Address")
    public Collection<IpV6Address> iPv6Addresses = new ArrayList<>();

    public final Links links = new Links();

    public VlanNetworkInterfaceJson() {
        super("#VLANNetworkInterface.1.0.0.VLANNetworkInterface");
    }

    @JsonPropertyOrder({
      "address", "subnetMask", "addressOrigin", "gateway", "hostname", "fqdn"
    })
    public static final class IpV4Address {
        @JsonProperty("Address")
        public String address;
        @JsonProperty("SubnetMask")
        public String subnetMask;
        @JsonProperty("AddressOrigin")
        public String addressOrigin;
        @JsonProperty("Gateway")
        public String gateway;
        @JsonProperty("HostName")
        public String hostname;
        @JsonProperty("FQDN")
        public String fqdn;
    }

    @JsonPropertyOrder({
            "address", "prefixLength", "addressOrigin", "scope", "gateway", "hostname", "fqdn"
    })
    public static final class IpV6Address {
        @JsonProperty("Address")
        public String address;
        @JsonProperty("PrefixLength")
        public Integer prefixLength;
        @JsonProperty("AddressOrigin")
        public String addressOrigin;
        @JsonProperty("Scope")
        public String scope;
        @JsonProperty("Gateway")
        public String gateway;
        @JsonProperty("HostName")
        public String hostname;
        @JsonProperty("FQDN")
        public String fqdn;
    }

    public static final class Links {
    }
}
