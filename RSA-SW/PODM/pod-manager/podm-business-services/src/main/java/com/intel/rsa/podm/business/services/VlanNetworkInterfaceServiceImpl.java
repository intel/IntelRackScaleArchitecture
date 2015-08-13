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
import com.intel.rsa.podm.business.dto.VlanNetworkInterfaceDto;
import com.intel.rsa.podm.business.dto.VlanNetworkInterfaceIpV4AddressDto;
import com.intel.rsa.podm.business.dto.VlanNetworkInterfaceIpV6AddressDto;
import com.intel.rsa.podm.business.entities.assets.SwitchPort;
import com.intel.rsa.podm.business.entities.assets.VlanNetworkInterface;
import com.intel.rsa.podm.business.entities.assets.properties.VlanNetworkInterfaceIpV4Address;
import com.intel.rsa.podm.business.entities.assets.properties.VlanNetworkInterfaceIpV6Address;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.ArrayList;
import java.util.Collection;

import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class VlanNetworkInterfaceServiceImpl implements VlanNetworkInterfaceService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getVlanNetworkInterfaceCollection(Context context)
            throws EntityNotFoundException {

        SwitchPort switchPort = (SwitchPort) traverser.traverse(context);

        return new CollectionDto(CollectionDtoType.VlanNetworkInterface, switchPort.getModified(), getAsIdList(switchPort.getVlans()));
    }

    @Override
    public VlanNetworkInterfaceDto getVlanNetworkInterface(Context context)
            throws EntityNotFoundException {

        VlanNetworkInterface vlanNetworkInterface = (VlanNetworkInterface) traverser.traverse(context);

        Collection<VlanNetworkInterfaceIpV4AddressDto> ipV4Addresses = getIpV4Addresses(vlanNetworkInterface);
        Collection<VlanNetworkInterfaceIpV6AddressDto> ipV6Addresses = getIpV6Addresses(vlanNetworkInterface);

        return VlanNetworkInterfaceDto
                .newBuilder()
                .id(vlanNetworkInterface.getId())
                .description(vlanNetworkInterface.getDescription())
                .name(vlanNetworkInterface.getName())
                .description(vlanNetworkInterface.getDescription())
                .state(vlanNetworkInterface.getState())
                .health(vlanNetworkInterface.getHealth())
                .modified(vlanNetworkInterface.getModified())
                .vlanEnabled(vlanNetworkInterface.isVlanEnable())
                .vlanId(vlanNetworkInterface.getVlanId())
                .ipV4Addresses(ipV4Addresses)
                .ipV6Addresses(ipV6Addresses)
                .build();
    }

    private Collection<VlanNetworkInterfaceIpV4AddressDto> getIpV4Addresses(VlanNetworkInterface vlanNetworkInterface) {
        Collection<VlanNetworkInterfaceIpV4AddressDto> addresses = new ArrayList<>();

        for (VlanNetworkInterfaceIpV4Address v4Address : vlanNetworkInterface.getIpV4Addresses()) {
            addresses.add(VlanNetworkInterfaceIpV4AddressDto
                            .newBuilder()
                            .address(v4Address.getAddress())
                            .subnetMask(v4Address.getSubnetMask())
                            .addressOrigin(v4Address.getAddressOrigin())
                            .gateway(v4Address.getGateway())
                            .hostname(v4Address.getHostName())
                            .fqdn(v4Address.getFqdn())
                            .build()
            );
        }

        return addresses;
    }

    private Collection<VlanNetworkInterfaceIpV6AddressDto> getIpV6Addresses(VlanNetworkInterface vlanNetworkInterface) {
        Collection<VlanNetworkInterfaceIpV6AddressDto> addresses = new ArrayList<>();

        for (VlanNetworkInterfaceIpV6Address v6Address : vlanNetworkInterface.getIpV6Addresses()) {
            addresses.add(VlanNetworkInterfaceIpV6AddressDto
                            .newBuilder()
                            .address(v6Address.getAddress())
                            .prefixLength(v6Address.getPrefixLength())
                            .addressOrigin(v6Address.getAddressOrigin())
                            .scope(v6Address.getScope())
                            .gateway(v6Address.getGateway())
                            .hostname(v6Address.getHostName())
                            .fqdn(v6Address.getFqdn())
                            .build()
            );
        }

        return addresses;
    }

}
