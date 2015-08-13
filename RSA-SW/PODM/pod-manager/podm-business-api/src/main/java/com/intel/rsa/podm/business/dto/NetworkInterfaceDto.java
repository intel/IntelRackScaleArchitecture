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

package com.intel.rsa.podm.business.dto;

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.LinkTechnology;
import com.intel.rsa.common.types.State;

import java.time.OffsetDateTime;
import java.util.List;

/**
 * Class transferring NetworkInterface data. Fields compliant with PSME API 20150330 fixed.
 */
public final class NetworkInterfaceDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final String description;
    private final State state;
    private final Health health;
    private final Health healthRollup;
    private final String factoryMacAddress;
    private final String macAddress;
    private final LinkTechnology linkTechnology;
    private final Integer speedMbps;
    private final Boolean autosense;
    private final Boolean fullDuplex;
    private final Integer frameSize;
    private final String hostName;
    private final String fqdn;
    private final Integer maxIPv6StaticAddresses;
    private final Boolean vlanEnable;
    private final Integer vlanId;
    private final String ipv6DefaultGateway;

    private List<NetworkIpV4AddressDto> iPv4Addresses;
    private List<NetworkIpV6AddressPolicyDto> ipV6AddressPolicyTable;
    private List<NetworkIpV6AddressDto> iPv6StaticAddresses;
    private List<NetworkIpV6AddressDto> iPv6Addresses;

    private List<String> nameServers;

    private NetworkInterfaceDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        description = builder.description;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
        factoryMacAddress = builder.factoryMacAddress;
        macAddress = builder.macAddress;
        linkTechnology = builder.linkTechnology;
        speedMbps = builder.speedMbps;
        autosense = builder.autosense;
        fullDuplex = builder.fullDuplex;
        frameSize = builder.frameSize;
        hostName = builder.hostName;
        fqdn = builder.fqdn;
        maxIPv6StaticAddresses = builder.maxIPv6StaticAddresses;
        vlanEnable = builder.vlanEnable;
        vlanId = builder.vlanId;
        ipv6DefaultGateway = builder.ipv6DefaultGateway;
        iPv4Addresses = builder.iPv4Addresses;
        ipV6AddressPolicyTable = builder.ipV6AddressPolicyTable;
        iPv6StaticAddresses = builder.iPv6StaticAddresses;
        iPv6Addresses = builder.iPv6Addresses;
        nameServers = builder.nameServers;
    }

    public Id getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public OffsetDateTime getModified() {
        return modified;
    }

    public String getDescription() {
        return description;
    }

    public State getState() {
        return state;
    }

    public Health getHealth() {
        return health;
    }

    public Health getHealthRollup() {
        return healthRollup;
    }

    public String getFactoryMacAddress() {
        return factoryMacAddress;
    }

    public String getMacAddress() {
        return macAddress;
    }

    public LinkTechnology getLinkTechnology() {
        return linkTechnology;
    }

    public Integer getSpeedMbps() {
        return speedMbps;
    }

    public Boolean isAutosense() {
        return autosense;
    }

    public Boolean isFullDuplex() {
        return fullDuplex;
    }

    public Integer getFrameSize() {
        return frameSize;
    }

    public String getHostName() {
        return hostName;
    }

    public String getFqdn() {
        return fqdn;
    }

    public Integer getMaxIPv6StaticAddresses() {
        return maxIPv6StaticAddresses;
    }

    public Boolean isVlanEnable() {
        return vlanEnable;
    }

    public Integer getVlanId() {
        return vlanId;
    }

    public String getIpv6DefaultGateway() {
        return ipv6DefaultGateway;
    }

    public List<NetworkIpV4AddressDto> getiPv4Addresses() {
        return iPv4Addresses;
    }

    public List<NetworkIpV6AddressPolicyDto> getIpV6AddressPolicyTable() {
        return ipV6AddressPolicyTable;
    }

    public List<NetworkIpV6AddressDto> getiPv6StaticAddresses() {
        return iPv6StaticAddresses;
    }

    public List<NetworkIpV6AddressDto> getiPv6Addresses() {
        return iPv6Addresses;
    }

    public Boolean getAutosense() {
        return autosense;
    }

    public Boolean getFullDuplex() {
        return fullDuplex;
    }

    public Boolean getVlanEnable() {
        return vlanEnable;
    }

    public List<String> getNameServers() {
        return nameServers;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private String description;
        private State state;
        private Health health;
        private Health healthRollup;
        private String factoryMacAddress;
        private String macAddress;
        private LinkTechnology linkTechnology;
        private Integer speedMbps;
        private Boolean autosense;
        private Boolean fullDuplex;
        private Integer frameSize;
        private String hostName;
        private String fqdn;
        private Integer maxIPv6StaticAddresses;
        private Boolean vlanEnable;
        private Integer vlanId;
        private String ipv6DefaultGateway;
        private List<NetworkIpV4AddressDto> iPv4Addresses;
        private List<NetworkIpV6AddressPolicyDto> ipV6AddressPolicyTable;
        private List<NetworkIpV6AddressDto> iPv6StaticAddresses;
        private List<NetworkIpV6AddressDto> iPv6Addresses;
        private List<String> nameServers;

        private Builder() {
        }

        public Builder id(Id id) {
            this.id = id;
            return this;
        }

        public Builder name(String name) {
            this.name = name;
            return this;
        }

        public Builder modified(OffsetDateTime modified) {
            this.modified = modified;
            return this;
        }

        public Builder description(String description) {
            this.description = description;
            return this;
        }

        public Builder state(State state) {
            this.state = state;
            return this;
        }

        public Builder health(Health health) {
            this.health = health;
            return this;
        }

        public Builder healthRollup(Health healthRollup) {
            this.healthRollup = healthRollup;
            return this;
        }

        public Builder factoryMacAddress(String factoryMacAddress) {
            this.factoryMacAddress = factoryMacAddress;
            return this;
        }

        public Builder macAddress(String macAddress) {
            this.macAddress = macAddress;
            return this;
        }

        public Builder linkTechnology(LinkTechnology linkTechnology) {
            this.linkTechnology = linkTechnology;
            return this;
        }

        public Builder speedMbps(Integer speedMbps) {
            this.speedMbps = speedMbps;
            return this;
        }

        public Builder autosense(Boolean autosense) {
            this.autosense = autosense;
            return this;
        }

        public Builder fullDuplex(Boolean fullDuplex) {
            this.fullDuplex = fullDuplex;
            return this;
        }

        public Builder frameSize(Integer frameSize) {
            this.frameSize = frameSize;
            return this;
        }

        public Builder hostName(String hostName) {
            this.hostName = hostName;
            return this;
        }

        public Builder fqdn(String fqdn) {
            this.fqdn = fqdn;
            return this;
        }

        public Builder maxIPv6StaticAddresses(Integer maxIPv6StaticAddresses) {
            this.maxIPv6StaticAddresses = maxIPv6StaticAddresses;
            return this;
        }

        public Builder vlanEnable(Boolean vlanEnable) {
            this.vlanEnable = vlanEnable;
            return this;
        }

        public Builder vlanId(Integer vlanId) {
            this.vlanId = vlanId;
            return this;
        }

        public Builder ipv6DefaultGateway(String ipv6DefaultGateway) {
            this.ipv6DefaultGateway = ipv6DefaultGateway;
            return this;
        }

        public Builder iPv4Addresses(List<NetworkIpV4AddressDto> iPv4Addresses) {
            this.iPv4Addresses = iPv4Addresses;
            return this;
        }

        public Builder ipV6AddressPolicyTable(List<NetworkIpV6AddressPolicyDto> ipV6AddressPolicyTable) {
            this.ipV6AddressPolicyTable = ipV6AddressPolicyTable;
            return this;
        }

        public Builder iPv6StaticAddresses(List<NetworkIpV6AddressDto> iPv6StaticAddresses) {
            this.iPv6StaticAddresses = iPv6StaticAddresses;
            return this;
        }

        public Builder iPv6Addresses(List<NetworkIpV6AddressDto> iPv6Addresses) {
            this.iPv6Addresses = iPv6Addresses;
            return this;
        }

        public Builder nameServers(List<String> nameServers) {
            this.nameServers = nameServers;
            return this;
        }

        public NetworkInterfaceDto build() {
            return new NetworkInterfaceDto(this);
        }
    }
}
