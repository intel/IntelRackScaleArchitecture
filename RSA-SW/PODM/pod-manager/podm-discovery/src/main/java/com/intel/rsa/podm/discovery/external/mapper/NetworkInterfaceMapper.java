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

package com.intel.rsa.podm.discovery.external.mapper;

import com.intel.rsa.client.api.reader.RsaNetworkInterface;
import com.intel.rsa.client.api.reader.RsaNetworkIpV4Address;
import com.intel.rsa.client.api.reader.RsaNetworkIpV6Address;
import com.intel.rsa.client.api.reader.RsaNetworkIpV6AddressPolicy;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.properties.NameServer;
import com.intel.rsa.podm.business.entities.assets.properties.NetworkInterfaceIpV4Address;
import com.intel.rsa.podm.business.entities.assets.properties.NetworkInterfaceIpV6Address;
import com.intel.rsa.podm.business.entities.assets.properties.NetworkInterfaceIpV6AddressPolicy;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.Collection;
import java.util.HashSet;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

@Dependent
public class NetworkInterfaceMapper extends DomainObjectMapper<RsaNetworkInterface, NetworkInterface> {
    @Inject
    private DomainObjectRepository repository;

    protected NetworkInterfaceMapper() {
        super(RsaNetworkInterface.class, NetworkInterface.class);
        registerProvider(NetworkInterfaceIpV4Address.class, this::provideIpV4Address);
        registerProvider(NetworkInterfaceIpV6AddressPolicy.class, this::provideIpV6AddressPolicy);
        registerProvider(NetworkInterfaceIpV6Address.class, this::provideIpV6Address);
        registerProvider(NameServer.class, this::provideNameServer);
    }

    @Override
    protected void performNotAutomatedMapping(RsaNetworkInterface source, NetworkInterface target) {
        super.performNotAutomatedMapping(source, target);

        if (target.getMacAddress() != null) {
            target.setMacAddress(target.getMacAddress().toUpperCase());
        }

        if (target.getFactoryMacAddress() != null) {
            target.setFactoryMacAddress(target.getFactoryMacAddress().toUpperCase());
        }

        target.updateModified();

        cleanIpV4Addresses(source.getIpV4Addresses(), target.getIpV4Addresses());
        cleanIpV6Addresses(source.getIpV6Addresses(), target.getIpV6Addresses());
        cleanIpV6Addresses(source.getIpV6StaticAddresses(), target.getIpV6StaticAddresses());
        cleanIpV6AddressPolicies(source, target);
        cleanNameServers(source, target);
    }

    private NetworkInterfaceIpV4Address provideIpV4Address(RsaNetworkIpV4Address address) {
        for (NetworkInterfaceIpV4Address v4Address : target.getIpV4Addresses()) {
            if (Objects.equals(v4Address.getAddress(), address.getAddress())) {
                return v4Address;
            }
        }

        return repository.create(NetworkInterfaceIpV4Address.class);
    }

    private NetworkInterfaceIpV6AddressPolicy provideIpV6AddressPolicy(RsaNetworkIpV6AddressPolicy addressPolicy) {
        for (NetworkInterfaceIpV6AddressPolicy policy : target.getIpV6AddressesPolicyTable()) {
            if (Objects.equals(policy.getLabel(), addressPolicy.getLabel())
                    && Objects.equals(policy.getPrecedence(), addressPolicy.getPrecedence())
                    && Objects.equals(policy.getPrefix(), addressPolicy.getPrefix())) {
                return policy;
            }
        }

        return repository.create(NetworkInterfaceIpV6AddressPolicy.class);
    }

    private NetworkInterfaceIpV6Address provideIpV6Address(RsaNetworkIpV6Address address) {
        for (NetworkInterfaceIpV6Address v6Address : getIpV6AddressTargetCollection(address)) {
            if (Objects.equals(v6Address.getAddress(), address.getAddress())) {
                return v6Address;
            }
        }

        return repository.create(NetworkInterfaceIpV6Address.class);
    }

    private Collection<NetworkInterfaceIpV6Address> getIpV6AddressTargetCollection(RsaNetworkIpV6Address address) {
        return source.getIpV6StaticAddresses().contains(address) ? target.getIpV6StaticAddresses() : target.getIpV6Addresses();
    }

    private NameServer provideNameServer(String sourceNameServer) {
        for (NameServer nameServer : target.getNameServers()) {
            if (nameServer.getNameServer().equals(sourceNameServer)) {
                return nameServer;
            }
        }

        NameServer nameServer = repository.create(NameServer.class);
        nameServer.setNameServer(sourceNameServer);
        return nameServer;
    }

    private void cleanIpV4Addresses(Collection<? extends RsaNetworkIpV4Address> srcAddrs,
                                    Collection<NetworkInterfaceIpV4Address> destAddrs) {
        Set<String> existingAddrs = srcAddrs.stream().map(RsaNetworkIpV4Address::getAddress).collect(Collectors.toSet());
        destAddrs.forEach(addr -> {
            if (!existingAddrs.contains(addr.getAddress())) {
                repository.delete(NetworkInterfaceIpV4Address.class, addr.getId());
            }
        });
    }

    private void cleanIpV6Addresses(Collection<? extends RsaNetworkIpV6Address> srcAddrs,
                                    Collection<NetworkInterfaceIpV6Address> destAddrs) {
        Set<String> existingAddrs = srcAddrs.stream().map(RsaNetworkIpV6Address::getAddress).collect(Collectors.toSet());
        destAddrs.forEach(addr -> {
            if (!existingAddrs.contains(addr.getAddress())) {
                repository.delete(NetworkInterfaceIpV6Address.class, addr.getId());
            }
        });
    }

    private void cleanNameServers(RsaNetworkInterface source, NetworkInterface target) {
        Set<String> existingNameServers = new HashSet<>(source.getNameServers());
        target.getNameServers().forEach(nameServer -> {
            if (!existingNameServers.contains(nameServer.getNameServer())) {
                repository.delete(NameServer.class, nameServer.getId());
            }
        });
    }

    private void cleanIpV6AddressPolicies(RsaNetworkInterface source, NetworkInterface target) {
        Set<String> existingPolicies = source.getIpV6AddressesPolicyTable().stream()
                .map(policy -> getAddressPolicyKey(policy.getPrefix(), policy.getLabel(), policy.getPrecedence()))
                .collect(Collectors.toSet());
        target.getIpV6AddressesPolicyTable().forEach(policy -> {
            if (!existingPolicies.contains(getAddressPolicyKey(policy.getPrefix(), policy.getLabel(), policy.getPrecedence()))) {
                repository.delete(NetworkInterfaceIpV6AddressPolicy.class, policy.getId());
            }
        });
    }

    private String getAddressPolicyKey(String prefix, Integer label, Integer precendence) {
        return label + prefix + precendence;
    }
}
