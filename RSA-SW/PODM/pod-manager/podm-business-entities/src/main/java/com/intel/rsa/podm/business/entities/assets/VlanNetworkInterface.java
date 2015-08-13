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
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.entities.assets.links.IpAddressLink;
import com.intel.rsa.podm.business.entities.assets.properties.VlanNetworkInterfaceIpV4Address;
import com.intel.rsa.podm.business.entities.assets.properties.VlanNetworkInterfaceIpV6Address;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.RequestScoped;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.booleanProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

/**
 *  Vlan network interface
 *
 */
@RequestScoped
@Transactional(Transactional.TxType.REQUIRED)
public class VlanNetworkInterface extends Asset {
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<String> DESCRIPTION = stringProperty("description");
    public static final DomainObjectProperty<Boolean> VLAN_ENABLED = booleanProperty("vlanEnabled");
    public static final DomainObjectProperty<Integer> VLAN_ID = integerProperty("vlanId");

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

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public Boolean isVlanEnable() {
        return getProperty(VLAN_ENABLED);
    }

    public void setVlanEnable(Boolean vlanEnable) {
        setProperty(VLAN_ENABLED, vlanEnable);
    }

    public Integer getVlanId() {
        return getProperty(VLAN_ID);
    }

    public void setVlanId(Integer vlanId) {
        setProperty(VLAN_ID, vlanId);
    }

    public VlanNetworkInterfaceIpV4Address addIpV4Address() {
        return addDomainObject(IpAddressLink.IP_V4_ADDRESS, VlanNetworkInterfaceIpV4Address.class);
    }

    public Collection<VlanNetworkInterfaceIpV4Address> getIpV4Addresses() {
        return getLinked(IpAddressLink.IP_V4_ADDRESS, VlanNetworkInterfaceIpV4Address.class);
    }

    public void setIpV4Addresses(Collection<VlanNetworkInterfaceIpV4Address> ipV4Addresses) {
        ipV4Addresses.forEach(address -> link(IpAddressLink.IP_V4_ADDRESS, address));
    }

    public VlanNetworkInterfaceIpV6Address addIpV6Address() {
        return addDomainObject(IpAddressLink.IP_V6_ADDRESS, VlanNetworkInterfaceIpV6Address.class);
    }

    public Collection<VlanNetworkInterfaceIpV6Address> getIpV6Addresses() {
        return getLinked(IpAddressLink.IP_V6_ADDRESS, VlanNetworkInterfaceIpV6Address.class);
    }

    public void setIpV6Addresses(Collection<VlanNetworkInterfaceIpV6Address> ipV6Addresses) {
        ipV6Addresses.forEach(address -> link(IpAddressLink.IP_V6_ADDRESS, address));
    }
}
