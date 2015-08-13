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

import com.intel.rsa.podm.business.dto.NetworkInterfaceDto;
import com.intel.rsa.podm.business.dto.NetworkIpV4AddressDto;
import com.intel.rsa.podm.business.dto.NetworkIpV6AddressDto;
import com.intel.rsa.podm.business.dto.NetworkIpV6AddressPolicyDto;
import com.intel.rsa.podm.rest.representation.json.templates.NetworkInterfaceJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.NetworkIPv4AddressJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.NetworkIPv6AddressJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.NetworkIpV6AddressPolicyJson;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static java.net.URI.create;

public final class NetworkInterfaceDtoJsonSerializer extends DtoJsonSerializer<NetworkInterfaceDto> {
    public NetworkInterfaceDtoJsonSerializer() {
        super(NetworkInterfaceDto.class);
    }

    @Override
    protected NetworkInterfaceJson translate(NetworkInterfaceDto dto) {
        NetworkInterfaceJson result = new NetworkInterfaceJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAManagers/Links/Members/1/Links/EthernetInterfaces/Links/Members/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.modified = dto.getModified();
        result.description = dto.getDescription();
        mapStatus(dto, result);
        result.factoryMacAddress = dto.getFactoryMacAddress();
        result.macAddress = dto.getMacAddress();
        result.linkTechnology = dto.getLinkTechnology();
        result.speedMbps = dto.getSpeedMbps();
        result.autosense = dto.isAutosense();
        result.fullDuplex = dto.isFullDuplex();
        result.frameSize = dto.getFrameSize();
        result.hostName = dto.getHostName();
        result.fqdn = dto.getFqdn();
        result.maxIPv6StaticAddresses = dto.getMaxIPv6StaticAddresses();
        result.vlanEnable = dto.isVlanEnable();
        result.vlanId = dto.getVlanId();
        result.iPv6DefaultGateway = dto.getIpv6DefaultGateway();

        dto.getiPv4Addresses().forEach(address -> addIpV4Address(result, address));
        dto.getIpV6AddressPolicyTable().forEach(policy -> addIpV6AddressPolicy(result, policy));
        dto.getiPv6StaticAddresses().forEach(address -> addIpV6StaticAddress(result, address));
        dto.getiPv6Addresses().forEach(address -> addIpV6Address(result, address));

        result.nameServers.addAll(dto.getNameServers());

        return result;
    }

    private void mapStatus(NetworkInterfaceDto dto, NetworkInterfaceJson result) {
        result.status.health = dto.getHealth();
        result.status.state = dto.getState();
        result.status.healthRollup = dto.getHealthRollup();
    }

    private void addIpV4Address(NetworkInterfaceJson result, NetworkIpV4AddressDto address) {
        NetworkIPv4AddressJson addressJson = new NetworkIPv4AddressJson();
        addressJson.address = address.getAddress();
        addressJson.addressOrigin = address.getAddressOrigin();
        addressJson.gateway = address.getGateway();
        addressJson.subnetMask = address.getSubnetMask();
        result.iPv4Addresses.add(addressJson);
    }

    private void addIpV6AddressPolicy(NetworkInterfaceJson result, NetworkIpV6AddressPolicyDto policy) {
        NetworkIpV6AddressPolicyJson addressJson = new NetworkIpV6AddressPolicyJson();
        addressJson.prefix = policy.getPrefix();
        addressJson.precedence = policy.getPrecedence();
        addressJson.label = policy.getLabel();
        result.iPv6AddressPolicyTable.add(addressJson);
    }

    private void addIpV6Address(NetworkInterfaceJson result, NetworkIpV6AddressDto address) {
        NetworkIPv6AddressJson addressJson = new NetworkIPv6AddressJson();
        addressJson.address = address.getAddress();
        addressJson.prefixLength = address.getPrefixLength();
        addressJson.addressOrigin = address.getAddressOrigin();
        addressJson.addressState = address.getAddressState();
        result.iPv6Addresses.add(addressJson);
    }

    private void addIpV6StaticAddress(NetworkInterfaceJson result, NetworkIpV6AddressDto address) {
        NetworkIPv6AddressJson addressJson = new NetworkIPv6AddressJson();
        addressJson.address = address.getAddress();
        addressJson.prefixLength = address.getPrefixLength();
        addressJson.addressOrigin = address.getAddressOrigin();
        addressJson.addressState = address.getAddressState();
        result.iPv6StaticAddresses.add(addressJson);
    }

}
