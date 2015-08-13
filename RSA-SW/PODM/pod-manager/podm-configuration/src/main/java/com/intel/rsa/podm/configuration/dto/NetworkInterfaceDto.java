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

package com.intel.rsa.podm.configuration.dto;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.LinkTechnology;
import com.intel.rsa.common.types.State;

import java.util.List;

public class NetworkInterfaceDto {
    @JsonProperty("Name")
    private String name;
    @JsonProperty("Description")
    private String description;
    @JsonProperty ("FQDN")
    private String fqdn;
    @JsonProperty ("HostName")
    private String hostName;
    @JsonProperty ("State")
    private State state;
    @JsonProperty ("Health")
    private Health health;
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
    @JsonProperty ("IPv6DefaultGateway")
    private String iPv6DefaultGateway;
    @JsonProperty ("IPv4Address")
    private NetworkIpV4AddressDto iPv4Address;

    private List<NetworkIpV4AddressDto> iPv4Addresses;

    public String getName() {
        return name;
    }

    public String getDescription() {
        return description;
    }

    public String getFqdn() {
        return fqdn;
    }

    public String getHostName() {
        return hostName;
    }

    public State getState() {
        return state;
    }

    public Health getHealth() {
        return health;
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

    public Boolean getAutosense() {
        return autosense;
    }

    public Boolean getFullDuplex() {
        return fullDuplex;
    }

    public Integer getFrameSize() {
        return frameSize;
    }

    public Integer getMaxIPv6StaticAddresses() {
        return maxIPv6StaticAddresses;
    }

    public Boolean getVlanEnable() {
        return vlanEnable;
    }

    public Integer getVlanId() {
        return vlanId;
    }

    public String getiPv6DefaultGateway() {
        return iPv6DefaultGateway;
    }

    public List<NetworkIpV4AddressDto> getiPv4Addresses() {
        return iPv4Addresses;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public void setFqdn(String fqdn) {
        this.fqdn = fqdn;
    }

    public void setHostName(String hostName) {
        this.hostName = hostName;
    }

    public void setState(State state) {
        this.state = state;
    }

    public void setHealth(Health health) {
        this.health = health;
    }

    public void setFactoryMacAddress(String factoryMacAddress) {
        this.factoryMacAddress = factoryMacAddress;
    }

    public void setMacAddress(String macAddress) {
        this.macAddress = macAddress;
    }

    public void setLinkTechnology(LinkTechnology linkTechnology) {
        this.linkTechnology = linkTechnology;
    }

    public void setSpeedMbps(Integer speedMbps) {
        this.speedMbps = speedMbps;
    }

    public void setAutosense(Boolean autosense) {
        this.autosense = autosense;
    }

    public void setFullDuplex(Boolean fullDuplex) {
        this.fullDuplex = fullDuplex;
    }

    public void setFrameSize(Integer frameSize) {
        this.frameSize = frameSize;
    }

    public void setMaxIPv6StaticAddresses(Integer maxIPv6StaticAddresses) {
        this.maxIPv6StaticAddresses = maxIPv6StaticAddresses;
    }

    public void setVlanEnable(Boolean vlanEnable) {
        this.vlanEnable = vlanEnable;
    }

    public void setVlanId(Integer vlanId) {
        this.vlanId = vlanId;
    }

    public void setiPv6DefaultGateway(String iPv6DefaultGateway) {
        this.iPv6DefaultGateway = iPv6DefaultGateway;
    }

    public void setiPv4Addresses(List<NetworkIpV4AddressDto> iPv4Addresses) {
        this.iPv4Addresses = iPv4Addresses;
    }

    public NetworkIpV4AddressDto getiPv4Address() {
        return iPv4Address;
    }
}
