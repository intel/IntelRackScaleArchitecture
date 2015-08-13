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

package com.intel.rsa.podm.business.services;

import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.CollectionDtoType;
import com.intel.rsa.podm.business.dto.NetworkInterfaceDto;
import com.intel.rsa.podm.business.dto.NetworkIpV4AddressDto;
import com.intel.rsa.podm.business.dto.NetworkIpV6AddressDto;
import com.intel.rsa.podm.business.dto.NetworkIpV6AddressPolicyDto;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.transaction.Transactional;
import java.time.OffsetDateTime;
import java.util.Collection;
import java.util.LinkedList;
import java.util.List;

@RequestScoped
@Transactional(Transactional.TxType.REQUIRED)
public class NetworkInterfaceServiceImpl implements NetworkInterfaceService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getNetworkInterfaceCollectionForManager(Context managerContext)  throws EntityNotFoundException {
        Manager manager = (Manager) traverser.traverse(managerContext);
        Collection<NetworkInterface> networkInterfaces = manager.getNetworkInterfaces();
        return new CollectionDto(
                CollectionDtoType.NetworkInterface,
                OffsetDateTime.now(),
                Contexts.getAsIdList(networkInterfaces));
    }

    @Override
    public CollectionDto getNetworkInterfaceCollectionForBlade(Context bladeContext)  throws EntityNotFoundException {
        Blade blade = (Blade) traverser.traverse(bladeContext);
        Collection<NetworkInterface> networkInterfaces = blade.getEthernetInterfaces();
        return new CollectionDto(
                CollectionDtoType.NetworkInterface,
                OffsetDateTime.now(),
                Contexts.getAsIdList(networkInterfaces)
        );
    }

    @Override
    public NetworkInterfaceDto getNetworkInterface(Context context) throws EntityNotFoundException {
        NetworkInterface nic = (NetworkInterface) traverser.traverse(context);
        return map(context, nic);
    }

    private List<String> getNameServers(NetworkInterface nic) {
        List<String> nameServers = new LinkedList<>();
        nic.getNameServers().forEach(nameServer -> nameServers.add(
                nameServer.getNameServer()
        ));
        return nameServers;
    }

    private List<NetworkIpV4AddressDto> getIpV4Addresses(NetworkInterface nic) {
        List<NetworkIpV4AddressDto> addresses = new LinkedList<>();
        nic.getIpV4Addresses().forEach(nicAddress -> addresses.add(
                NetworkIpV4AddressDto.newBuilder()
                        .address(nicAddress.getAddress())
                        .addressOrigin(nicAddress.getAddressOrigin())
                        .gateway(nicAddress.getGateway())
                        .subnetMask(nicAddress.getSubnetMask())
                        .build()
        ));
        return addresses;
    }

    private List<NetworkIpV6AddressPolicyDto> getIpV6AddressPolicyTable(NetworkInterface nic) {
        List<NetworkIpV6AddressPolicyDto> policies = new LinkedList<>();
        nic.getIpV6AddressesPolicyTable().forEach(policy -> policies.add(
                NetworkIpV6AddressPolicyDto.newBuilder()
                        .address(policy.getPrefix())
                        .precedence(policy.getPrecedence())
                        .label(policy.getLabel())
                        .build()
        ));
        return policies;
    }

    private List<NetworkIpV6AddressDto> getIpV6StaticAddresses(NetworkInterface nic) {
        List<NetworkIpV6AddressDto> addresses = new LinkedList<>();
        nic.getIpV6StaticAddresses().forEach(staticAddress -> addresses.add(
                NetworkIpV6AddressDto.newBuilder()
                        .address(staticAddress.getAddress())
                        .prefixLength(staticAddress.getPrefixLength())
                        .addressOrigin(staticAddress.getAddressOrigin())
                        .addressState(staticAddress.getAddressState())
                        .build()
        ));
        return addresses;
    }

    private List<NetworkIpV6AddressDto> getIpV6Addresses(NetworkInterface nic) {
        List<NetworkIpV6AddressDto> addresses = new LinkedList<>();
        nic.getIpV6Addresses().forEach(staticAddress -> addresses.add(
                NetworkIpV6AddressDto.newBuilder()
                        .address(staticAddress.getAddress())
                        .prefixLength(staticAddress.getPrefixLength())
                        .addressOrigin(staticAddress.getAddressOrigin())
                        .addressState(staticAddress.getAddressState())
                        .build()
        ));
        return addresses;
    }

    private NetworkInterfaceDto map(Context context, NetworkInterface nic) {
        return NetworkInterfaceDto.newBuilder()
                .id(context.getId())
                .autosense(nic.isAutosense())
                .description(nic.getDescription())
                .factoryMacAddress(nic.getFactoryMacAddress())
                .fqdn(nic.getFqdn())
                .frameSize(nic.getFrameSize())
                .fullDuplex(nic.isFullDuplex())
                .state(nic.getState())
                .health(nic.getHealth())
                .healthRollup(nic.getHealthRollup())
                .hostName(nic.getHostName())
                .linkTechnology(nic.getLinkTechnology())
                .ipv6DefaultGateway(nic.getIpv6DefaultGateway())
                .macAddress(nic.getMacAddress())
                .maxIPv6StaticAddresses(nic.getMaxIPv6StaticAddresses())
                .modified(nic.getModified())
                .name(nic.getName())
                .speedMbps(nic.getSpeedMbps())
                .vlanEnable(nic.isVlanEnable())
                .vlanId(nic.getVlanId())
                .iPv4Addresses(getIpV4Addresses(nic))
                .ipV6AddressPolicyTable(getIpV6AddressPolicyTable(nic))
                .iPv6StaticAddresses(getIpV6StaticAddresses(nic))
                .iPv6Addresses(getIpV6Addresses(nic))
                .nameServers(getNameServers(nic))
                .build();
    }
}
