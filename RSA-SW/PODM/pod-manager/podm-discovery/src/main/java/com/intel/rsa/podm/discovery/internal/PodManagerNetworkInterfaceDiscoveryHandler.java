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
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.properties.NetworkInterfaceIpV4Address;
import com.intel.rsa.podm.configuration.ConfigurationIOException;
import com.intel.rsa.podm.configuration.ConfigurationReader;
import com.intel.rsa.podm.configuration.dto.NetworkInterfaceDto;
import com.intel.rsa.podm.configuration.dto.NetworkIpV4AddressDto;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;

@Dependent
public class PodManagerNetworkInterfaceDiscoveryHandler {

    @Inject
    Logger logger;

    @Inject
    private ConfigurationReader configurationReader;

    @Inject
    private NetworkParametersReader networkParametersReader;

    public void addNetworkInterfaces(Manager manager) {
        for (NetworkInterfaceDto interfaceDto : networkParametersReader.discoverEnabledSystemNetworkInterfaces()) {
            addNetworkInterface(manager, interfaceDto);
        }
    }

    private void addNetworkInterface(Manager manager, NetworkInterfaceDto fromSystem) {
        NetworkInterface networkInterface = manager.addNetworkInterface();
        networkInterface.updateModified();
        networkInterface.setName(fromSystem.getName());
        networkInterface.setDescription(fromSystem.getDescription());
        networkInterface.setVlanEnable(fromSystem.getVlanEnable());
        networkInterface.setVlanId(fromSystem.getVlanId());
        networkInterface.setFrameSize(fromSystem.getFrameSize());
        networkInterface.setMacAddress(fromSystem.getMacAddress());
        networkInterface.setState(fromSystem.getState());
        networkInterface.setHealth(fromSystem.getHealth());
        networkInterface.setIpV6DefaultGateway(fromSystem.getiPv6DefaultGateway() == null ? "" : fromSystem.getiPv6DefaultGateway());

        for (NetworkIpV4AddressDto dto : fromSystem.getiPv4Addresses()) {
            NetworkInterfaceIpV4Address ipAddress = networkInterface.addIpV4Address();
            ipAddress.setAddress(dto.getAddress());
        }

        enhanceFromConfig(networkInterface);
    }

    private void enhanceFromConfig(NetworkInterface networkInterface) {
        try {
            NetworkInterfaceDto fromConfig = configurationReader.readConfiguration("interface-" + networkInterface.getName(),
                    NetworkInterfaceDto.class);

            networkInterface.setState(fromConfig.getState());
            networkInterface.setHealth(fromConfig.getHealth());
            networkInterface.setAutosense(fromConfig.getAutosense());
            networkInterface.setFactoryMacAddress(fromConfig.getFactoryMacAddress());
            networkInterface.setFqdn(fromConfig.getFqdn());
            networkInterface.setFullDuplex(fromConfig.getFullDuplex());
            networkInterface.setHostName(fromConfig.getHostName());
            networkInterface.setLinkTechnology(fromConfig.getLinkTechnology());
            networkInterface.setSpeedMbps(fromConfig.getSpeedMbps());
            networkInterface.setFqdn(fromConfig.getFqdn());
            networkInterface.setMaxIpV6StaticAddresses(fromConfig.getMaxIPv6StaticAddresses());
            networkInterface.setIpV6DefaultGateway(fromConfig.getiPv6DefaultGateway() == null ? "" : fromConfig.getiPv6DefaultGateway());
            enhanceIpV4AddressesFromConfig(networkInterface, fromConfig);
        } catch (ConfigurationIOException e) {
            logger.e("Pod manager network interface {} configuration is unavailable", networkInterface.getName());
        }
    }

    private void enhanceIpV4AddressesFromConfig(NetworkInterface networkInterface, NetworkInterfaceDto fromConfig) {
        for (NetworkInterfaceIpV4Address ipAddress : networkInterface.getIpV4Addresses()) {
            ipAddress.setAddressOrigin(fromConfig.getiPv4Address().getAddressOrigin());
            ipAddress.setGateway(fromConfig.getiPv4Address().getGateway());
            ipAddress.setSubnetMask(fromConfig.getiPv4Address().getSubnetMask());
        }
    }
}
