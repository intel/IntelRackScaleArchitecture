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

package com.intel.rsa.podm.discovery.external.mapper.psme;

import com.intel.rsa.client.api.reader.psme.PsmeVlanNetworkInterface;
import com.intel.rsa.client.api.reader.psme.PsmeVlanNetworkInterfaceIpV4Address;
import com.intel.rsa.client.api.reader.psme.PsmeVlanNetworkInterfaceIpV6Address;
import com.intel.rsa.podm.business.entities.assets.VlanNetworkInterface;
import com.intel.rsa.podm.business.entities.assets.properties.VlanNetworkInterfaceIpV4Address;
import com.intel.rsa.podm.business.entities.assets.properties.VlanNetworkInterfaceIpV6Address;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.discovery.external.mapper.DomainObjectMapper;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.Objects;

@Dependent
public class VlanNetworkInterfaceMapper extends DomainObjectMapper<PsmeVlanNetworkInterface, VlanNetworkInterface> {
    @Inject
    private DomainObjectRepository repository;

    public VlanNetworkInterfaceMapper() {
        super(PsmeVlanNetworkInterface.class, VlanNetworkInterface.class);
        registerProvider(VlanNetworkInterfaceIpV6Address.class, this::provideIpV6Address);
        registerProvider(VlanNetworkInterfaceIpV4Address.class, this::provideIpV4Address);
    }

    private VlanNetworkInterfaceIpV6Address provideIpV6Address(PsmeVlanNetworkInterfaceIpV6Address address) {
        for (VlanNetworkInterfaceIpV6Address v4Address : target.getIpV6Addresses()) {
            if (Objects.equals(v4Address.getAddress(), address.getAddress())) {
                return v4Address;
            }
        }
        return repository.create(VlanNetworkInterfaceIpV6Address.class);
    }

    private VlanNetworkInterfaceIpV4Address provideIpV4Address(PsmeVlanNetworkInterfaceIpV4Address address) {
        for (VlanNetworkInterfaceIpV4Address v4Address : target.getIpV4Addresses()) {
            if (Objects.equals(v4Address.getAddress(), address.getAddress())) {
                return v4Address;
            }
        }
        return repository.create(VlanNetworkInterfaceIpV4Address.class);
    }

    @Override
    protected void performNotAutomatedMapping(PsmeVlanNetworkInterface source, VlanNetworkInterface target) {
        super.performNotAutomatedMapping(source, target);
        target.updateModified();
    }
}
