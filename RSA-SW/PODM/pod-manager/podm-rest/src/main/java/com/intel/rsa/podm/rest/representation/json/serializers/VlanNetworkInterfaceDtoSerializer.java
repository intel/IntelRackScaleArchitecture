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

package com.intel.rsa.podm.rest.representation.json.serializers;

import com.intel.rsa.podm.business.dto.VlanNetworkInterfaceDto;
import com.intel.rsa.podm.business.dto.VlanNetworkInterfaceIpV4AddressDto;
import com.intel.rsa.podm.business.dto.VlanNetworkInterfaceIpV6AddressDto;
import com.intel.rsa.podm.rest.representation.json.templates.VlanNetworkInterfaceJson;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static java.net.URI.create;

public final class VlanNetworkInterfaceDtoSerializer extends DtoJsonSerializer<VlanNetworkInterfaceDto> {
    public VlanNetworkInterfaceDtoSerializer() {
        super(VlanNetworkInterfaceDto.class);
    }

    @Override
    protected VlanNetworkInterfaceJson translate(VlanNetworkInterfaceDto dto) {
        VlanNetworkInterfaceJson result = new VlanNetworkInterfaceJson();

        result.oDataContext = create("/rest/v1/$metadata#VLANNetworkInterface/$entity");
        result.oDataId = oDataId(context.getRequestPath());
        result.id = dto.getId();
        result.name = dto.getName();
        result.description = dto.getDescription();
        result.modified = dto.getModified();
        mapStatus(dto, result);
        result.vlanEnabled = dto.getVlanEnabled();
        result.vlanId = dto.getVlanId();

        for (VlanNetworkInterfaceIpV4AddressDto addressDto : dto.getIpV4Addresses()) {
            appendIpV4Address(result, addressDto);
        }

        for (VlanNetworkInterfaceIpV6AddressDto addressDto : dto.getIpV6Addresses()) {
            appendIpV6Address(result, addressDto);
        }
        return result;
    }

    private void mapStatus(VlanNetworkInterfaceDto dto, VlanNetworkInterfaceJson result) {
        result.status.state = dto.getState();
        result.status.health = dto.getHealth();
    }

    private void appendIpV4Address(VlanNetworkInterfaceJson interfaceJson, VlanNetworkInterfaceIpV4AddressDto addressDto) {
        VlanNetworkInterfaceJson.IpV4Address address = new VlanNetworkInterfaceJson.IpV4Address();
        address.address = addressDto.getAddress();
        address.subnetMask = addressDto.getSubnetMask();
        address.addressOrigin = addressDto.getAddressOrigin();
        address.gateway = addressDto.getGateway();
        address.hostname = addressDto.getHostname();
        address.fqdn = addressDto.getFqdn();

        interfaceJson.iPv4Addresses.add(address);
    }

    private void appendIpV6Address(VlanNetworkInterfaceJson interfaceJson, VlanNetworkInterfaceIpV6AddressDto addressDto) {
        VlanNetworkInterfaceJson.IpV6Address address = new VlanNetworkInterfaceJson.IpV6Address();

        address.address = addressDto.getAddress();
        address.prefixLength = addressDto.getPrefixLength();
        address.addressOrigin = addressDto.getAddressOrigin();
        address.scope = addressDto.getScope();
        address.gateway = addressDto.getGateway();
        address.hostname = addressDto.getHostname();
        address.fqdn = addressDto.getFqdn();

        interfaceJson.iPv6Addresses.add(address);
    }
}
