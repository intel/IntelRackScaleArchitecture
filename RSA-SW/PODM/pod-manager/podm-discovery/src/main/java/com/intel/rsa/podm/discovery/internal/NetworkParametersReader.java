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

package com.intel.rsa.podm.discovery.internal;

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.utils.net.MacAddress;
import com.intel.rsa.podm.configuration.dto.NetworkInterfaceDto;
import com.intel.rsa.podm.configuration.dto.NetworkIpV4AddressDto;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

@Dependent
public class NetworkParametersReader {

    private static final String VLAN_SEPARATOR = ".";
    private static final String VLAN_SEPARATOR_REGEX = "\\.";

    @Inject
    private Logger logger;

    public Iterable<NetworkInterfaceDto> discoverEnabledSystemNetworkInterfaces() {
        List<NetworkInterfaceDto> networkInterfaceDtos = new ArrayList<>();
        List<NetworkInterface> networkInterfaces = null;

        try {
            networkInterfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
        } catch (SocketException e) {
            logger.e("Could not discover system network interfaces", e);
            return networkInterfaceDtos;
        }

        for (NetworkInterface networkInterface : networkInterfaces) {

            if (shouldBeOmitted(networkInterface)) {
                continue;
            }

            NetworkInterfaceDto interfaceDto = getNetworkInterfacesDto(networkInterface);
            networkInterfaceDtos.add(interfaceDto);
        }

        return networkInterfaceDtos;
    }

    private NetworkInterfaceDto getNetworkInterfacesDto(NetworkInterface networkInterface) {
        NetworkInterfaceDto interfaceDto = new NetworkInterfaceDto();
        interfaceDto.setName(networkInterface.getName());
        interfaceDto.setDescription(networkInterface.getDisplayName());
        interfaceDto.setVlanEnable(isVlanEnabled(networkInterface));
        interfaceDto.setVlanId(getVlanIdForNetworkInterface(networkInterface));
        interfaceDto.setFrameSize(getMtu(networkInterface));
        interfaceDto.setMacAddress(getMacAddressAsString(networkInterface));
        interfaceDto.setiPv4Addresses(getInetAddresses(networkInterface));
        interfaceDto.setHealth(Health.OK);
        interfaceDto.setState(State.ENABLED);
        return interfaceDto;
    }

    private boolean shouldBeOmitted(NetworkInterface networkInterface) {
        try {
            return networkInterface.isLoopback();
        } catch (SocketException e) {
            logger.e("network interface identification failed for network interface: {}", networkInterface.getName(), e);
        }

        return false;
    }

    private List<NetworkIpV4AddressDto> getInetAddresses(NetworkInterface networkInterface) {
        List<NetworkIpV4AddressDto> inetAddressesDtos = new ArrayList<>();
        List<InetAddress> inetAddresses = Collections.list(networkInterface.getInetAddresses());

        for (InetAddress inetAddress : inetAddresses) {
            // TODO handle IPv6 Addresses also
            if (inetAddress instanceof Inet4Address) {
                NetworkIpV4AddressDto addressDto = new NetworkIpV4AddressDto();
                addressDto.setAddress(inetAddress.getHostAddress());
                addressDto.setAddressOrigin(null);
                addressDto.setGateway(null);
                addressDto.setSubnetMask(null);
                inetAddressesDtos.add(addressDto);
            }
        }
        return inetAddressesDtos;
    }

    private String getMacAddressAsString(NetworkInterface networkInterface) {
        byte[] hardwareAddress = null;

        try {
            hardwareAddress = networkInterface.getHardwareAddress();
        } catch (SocketException e) {
            logger.e("mac reading failed for network interface: {}", networkInterface.getName(), e);
        }

        if (hardwareAddress == null) {
            return null;
        }

        return new MacAddress(hardwareAddress).toString();
    }

    private Integer getMtu(NetworkInterface networkInterface) {
        try {
            return networkInterface.getMTU();
        } catch (SocketException e) {
            logger.e("MTU reading failed for network interface: {}", networkInterface.getName(), e);
            return null;
        }
    }

    private boolean isVlanEnabled(NetworkInterface networkInterface) {
        return networkInterface.getName().contains(VLAN_SEPARATOR);
    }

    private Integer getVlanIdForNetworkInterface(NetworkInterface networkInterface) {
        String[] parts = networkInterface.getName().split(VLAN_SEPARATOR_REGEX);

        if (parts.length > 1) {
            try {
                return Integer.valueOf(parts[1]);
            } catch (NumberFormatException e) {
                logger.e("VlanId parsing failed for network interface: {}", networkInterface.getName(), e);
                return null;
            }
        }

        return null;
    }
}
