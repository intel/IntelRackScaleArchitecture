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

package com.intel.rsa.client;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkInterface;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkIpV4Address;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkIpV6Address;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkIpV6AddressPolicy;
import com.intel.rsa.client.api.reader.rss.RssNetworkInterface;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.client.psme.PsmeNetworkIpV4AddressImpl;
import com.intel.rsa.client.psme.PsmeNetworkIpV6AddressImpl;
import com.intel.rsa.client.psme.PsmeNetworkIpV6AddressPolicyImpl;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.LinkTechnology;
import com.intel.rsa.common.types.State;

import java.util.List;

public class RsaNetworkInterfaceImpl extends ResourceImpl implements PsmeNetworkInterface, RssNetworkInterface {
    @JsonProperty ("Description")
    private String description;
    @JsonProperty ("FQDN")
    private String fqdn;
    @JsonProperty ("HostName")
    private String hostName;
    @JsonProperty ("Status")
    private StatusDto status;
    @JsonProperty ("FactoryMacAddress")
    private String factoryMacAddress;
    @JsonProperty ("MacAddress")
    private String macAddress;
    @JsonProperty ("LinkTechnology")
    private LinkTechnology linkTechnology;
    @JsonProperty ("SpeedMbps")
    private Integer speedMbps;
    @JsonProperty ("Autosense")
    private Boolean autosense;
    @JsonProperty ("FullDuplex")
    private Boolean fullDuplex;
    @JsonProperty ("FrameSize")
    private Integer frameSize;
    @JsonProperty ("MaxIPv6StaticAddresses")
    private Integer maxIPv6StaticAddresses;
    @JsonProperty("VLANEnable")
    private Boolean vlanEnable;
    @JsonProperty("VLANId")
    private Integer vlanId;
    @JsonProperty("IPv6DefaultGateway")
    private String iPv6DefaultGateway;
    @JsonProperty("IPv4Addresses")
    private List<PsmeNetworkIpV4AddressImpl> iPv4Addresses;
    @JsonProperty("IPv6AddressPolicyTable")
    private List<PsmeNetworkIpV6AddressPolicyImpl> iPv6AddressPolicyTable;
    @JsonProperty("IPv6StaticAddresses")
    private List<PsmeNetworkIpV6AddressImpl> iPv6StaticAddresses;
    @JsonProperty("IPv6Addresses")
    private List<PsmeNetworkIpV6AddressImpl> iPv6Addresses;
    @JsonProperty("NameServers")
    private List<String> nameServers;

    @Override
    public String getDescription() {
        return description;
    }

    @Override
    public String getFqdn() {
        return fqdn;
    }

    @Override
    public String getHostName() {
        return hostName;
    }

    @Override
    public State getState() {
        return status.getState();
    }

    @Override
    public Health getHealth() {
        return status.getHealth();
    }

    @Override
    public Health getHealthRollup() {
        return status.getHealthRollup();
    }

    @Override
    public String getFactoryMacAddress() {
        return factoryMacAddress;
    }

    @Override
    public String getMacAddress() {
        return macAddress;
    }

    @Override
    public LinkTechnology getLinkTechnology() {
        return linkTechnology;
    }

    @Override
    public Integer getSpeedMbps() {
        return speedMbps;
    }

    @Override
    public Boolean isAutosense() {
        return autosense;
    }

    @Override
    public Boolean isFullDuplex() {
        return fullDuplex;
    }

    @Override
    public Integer getFrameSize() {
        return frameSize;
    }

    @Override
    public Integer getMaxIPv6StaticAddresses() {
        return maxIPv6StaticAddresses;
    }

    @Override
    public Boolean isVlanEnable() {
        return vlanEnable;
    }

    @Override
    public Integer getVlanId() {
        return vlanId;
    }

    @Override
    public String getiPv6DefaultGateway() {
        return iPv6DefaultGateway;
    }

    @Override
    public List<PsmeNetworkIpV4Address> getIpV4Addresses() {
        return (List) iPv4Addresses;
    }

    @Override
    public List<PsmeNetworkIpV6AddressPolicy> getIpV6AddressesPolicyTable() {
        return (List) iPv6AddressPolicyTable;
    }

    @Override
    public List<PsmeNetworkIpV6Address> getIpV6StaticAddresses() {
        return (List) iPv6StaticAddresses;
    }

    @Override
    public List<PsmeNetworkIpV6Address> getIpV6Addresses() {
        return (List) iPv6Addresses;
    }

    @Override
    public List<String> getNameServers() {
        return (List) nameServers;
    }
}
