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

package com.intel.rsa.podm.business.entities.assets;

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.LinkTechnology;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.entities.assets.links.ComposedNodeLink;
import com.intel.rsa.podm.business.entities.assets.links.NetworkInterfaceLink;
import com.intel.rsa.podm.business.entities.assets.properties.NameServer;
import com.intel.rsa.podm.business.entities.assets.properties.NetworkInterfaceIpV4Address;
import com.intel.rsa.podm.business.entities.assets.properties.NetworkInterfaceIpV6Address;
import com.intel.rsa.podm.business.entities.assets.properties.NetworkInterfaceIpV6AddressPolicy;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.net.URI;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.singleOrNull;
import static com.intel.rsa.podm.business.entities.assets.links.AssetLink.CONTAINED_BY;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.booleanProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

/**
 * Network interface of Manager.
 */
@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class NetworkInterface extends Asset implements Discoverable {
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<String> DESCRIPTION = stringProperty("description");
    public static final DomainObjectProperty<String> FAC_MAC_ADDR = stringProperty("factoryMacAddress");
    public static final DomainObjectProperty<String> MAC_ADDR = stringProperty("macAddress");
    public static final DomainObjectProperty<LinkTechnology> LINK_TECH = enumProperty("linkTechnology", LinkTechnology.class);
    public static final DomainObjectProperty<Integer> SPEED_MBPS = integerProperty("speedMbps");
    public static final DomainObjectProperty<Boolean> AUTOSENSE = booleanProperty("autosense");
    public static final DomainObjectProperty<Boolean> FULL_DUPLEX = booleanProperty("fullDuplex");
    public static final DomainObjectProperty<Integer> FRAME_SIZE = integerProperty("frameSize");
    public static final DomainObjectProperty<String> HOST_NAME = stringProperty("hostName");
    public static final DomainObjectProperty<String> FQDN = stringProperty("fqdn");
    public static final DomainObjectProperty<Integer> MAX_IP_V6_STATIC_ADDR = integerProperty("maxIPv6StaticAddr");
    public static final DomainObjectProperty<Boolean> VLAN_ENABLE = booleanProperty("vlanEnable");
    public static final DomainObjectProperty<Integer> VLAN_ID = integerProperty("vlanId");
    public static final DomainObjectProperty<String> IP_V6_DEFAULT_GATEWAY = stringProperty("ipv6DefaultGateway");

    public String getDescription() {
        return getProperty(DESCRIPTION);
    }

    public void setDescription(String description) {
        setProperty(DESCRIPTION, description);
    }

    public Health getHealth() {
        return getProperty(HEALTH);
    }

    public void setHealth(Health health) {
        setProperty(HEALTH, health);
    }

    public Health getHealthRollup() {
        return getProperty(HEALTH_ROLLUP);
    }

    public void setHealthRollup(Health healthRollup) {
        setProperty(HEALTH_ROLLUP, healthRollup);
    }

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public String getFactoryMacAddress() {
        return getProperty(FAC_MAC_ADDR);
    }

    public void setFactoryMacAddress(String factoryMacAddress) {
        setProperty(FAC_MAC_ADDR, factoryMacAddress);
    }

    public String getMacAddress() {
        return getProperty(MAC_ADDR);
    }

    public void setMacAddress(String macAddress) {
        setProperty(MAC_ADDR, macAddress);
    }

    public LinkTechnology getLinkTechnology() {
        return getProperty(LINK_TECH);
    }

    public void setLinkTechnology(LinkTechnology linkTechnology) {
        setProperty(LINK_TECH, linkTechnology);
    }

    public Integer getSpeedMbps() {
        return getProperty(SPEED_MBPS);
    }

    public void setSpeedMbps(Integer speedMbps) {
        setProperty(SPEED_MBPS, speedMbps);
    }

    public Boolean isAutosense() {
        return getProperty(AUTOSENSE);
    }

    public void setAutosense(Boolean autosense) {
        setProperty(AUTOSENSE, autosense);
    }

    public Boolean isFullDuplex() {
        return getProperty(FULL_DUPLEX);
    }

    public void setFullDuplex(Boolean fullDuplex) {
        setProperty(FULL_DUPLEX, fullDuplex);
    }

    public Integer getFrameSize() {
        return getProperty(FRAME_SIZE);
    }

    public void setFrameSize(Integer frameSize) {
        setProperty(FRAME_SIZE, frameSize);
    }

    public String getHostName() {
        return getProperty(HOST_NAME);
    }

    public void setHostName(String hostName) {
        setProperty(HOST_NAME, hostName);
    }

    public String getFqdn() {
        return getProperty(FQDN);
    }

    public void setFqdn(String fqdn) {
        setProperty(FQDN, fqdn);
    }

    public Integer getMaxIPv6StaticAddresses() {
        return getProperty(MAX_IP_V6_STATIC_ADDR);
    }

    public void setMaxIpV6StaticAddresses(Integer maxIpV6StaticAddresses) {
        setProperty(MAX_IP_V6_STATIC_ADDR, maxIpV6StaticAddresses);
    }

    public Boolean isVlanEnable() {
        return getProperty(VLAN_ENABLE);
    }

    public void setVlanEnable(Boolean vlanEnable) {
        setProperty(VLAN_ENABLE, vlanEnable);
    }

    public Integer getVlanId() {
        return getProperty(VLAN_ID);
    }

    public void setVlanId(Integer vlanId) {
        setProperty(VLAN_ID, vlanId);
    }

    public String getIpv6DefaultGateway() {
        return getProperty(IP_V6_DEFAULT_GATEWAY);
    }

    public void setIpV6DefaultGateway(String ipV6DefaultGateway) {
        setProperty(IP_V6_DEFAULT_GATEWAY, ipV6DefaultGateway);
    }

    public NetworkInterfaceIpV4Address addIpV4Address() {
        return addDomainObject(NetworkInterfaceLink.IP4_ADDRESS, NetworkInterfaceIpV4Address.class);
    }

    public Collection<NetworkInterfaceIpV4Address> getIpV4Addresses() {
        return getLinked(NetworkInterfaceLink.IP4_ADDRESS, NetworkInterfaceIpV4Address.class);
    }

    public void setIpV4Addresses(Collection<NetworkInterfaceIpV4Address> addresses) {
        addresses.forEach(addr -> link(NetworkInterfaceLink.IP4_ADDRESS, addr));
    }

    public NetworkInterfaceIpV6AddressPolicy addIpV6AddressPolicy() {
        return addDomainObject(NetworkInterfaceLink.IP6_ADDRESS_POLICY, NetworkInterfaceIpV6AddressPolicy.class);
    }

    public Collection<NetworkInterfaceIpV6AddressPolicy> getIpV6AddressesPolicyTable() {
        return getLinked(NetworkInterfaceLink.IP6_ADDRESS_POLICY, NetworkInterfaceIpV6AddressPolicy.class);
    }

    public void setIpV6AddressesPolicyTable(Collection<NetworkInterfaceIpV6AddressPolicy> addresses) {
        addresses.forEach(addr -> link(NetworkInterfaceLink.IP6_ADDRESS_POLICY, addr));
    }

    public NetworkInterfaceIpV6Address addIpV6StaticAddress() {
        return addDomainObject(NetworkInterfaceLink.IP6_STATIC_ADDRESS, NetworkInterfaceIpV6Address.class);
    }

    public Collection<NetworkInterfaceIpV6Address> getIpV6StaticAddresses() {
        return getLinked(NetworkInterfaceLink.IP6_STATIC_ADDRESS, NetworkInterfaceIpV6Address.class);
    }

    public void setIpV6StaticAddresses(Collection<NetworkInterfaceIpV6Address> addresses) {
        addresses.forEach(addr -> link(NetworkInterfaceLink.IP6_STATIC_ADDRESS, addr));
    }

    public NetworkInterfaceIpV6Address addIpV6Address() {
        return addDomainObject(NetworkInterfaceLink.IP6_ADDRESS, NetworkInterfaceIpV6Address.class);
    }

    public Collection<NetworkInterfaceIpV6Address> getIpV6Addresses() {
        return getLinked(NetworkInterfaceLink.IP6_ADDRESS, NetworkInterfaceIpV6Address.class);
    }

    public void setIpV6Addresses(Collection<NetworkInterfaceIpV6Address> addresses) {
        addresses.forEach(addr -> link(NetworkInterfaceLink.IP6_ADDRESS, addr));
    }

    public NameServer addNameServer() {
        return addDomainObject(NetworkInterfaceLink.NAME_SERVER, NameServer.class);
    }

    public Collection<NameServer> getNameServers() {
        return getLinked(NetworkInterfaceLink.NAME_SERVER, NameServer.class);
    }

    public void setNameServers(Collection<NameServer> nameServers) {
        nameServers.forEach(nameServer -> link(NetworkInterfaceLink.NAME_SERVER, nameServer));
    }

    @Override
    public URI getSourceUri() {
        return getProperty(SOURCE_URI);
    }

    @Override
    public void setSourceUri(URI sourceUri) {
        setProperty(SOURCE_URI, sourceUri);
    }

    public Blade getBlade() {
        return singleOrNull(getLinked(CONTAINED_BY, Blade.class));
    }

    public Manager getManager() {
        return singleOrNull(getLinked(CONTAINED_BY, Manager.class));
    }

    public ComposedNode getComposedNode() {
        return singleOrNull(getLinked(ComposedNodeLink.INCLUDED, ComposedNode.class));
    }
}
