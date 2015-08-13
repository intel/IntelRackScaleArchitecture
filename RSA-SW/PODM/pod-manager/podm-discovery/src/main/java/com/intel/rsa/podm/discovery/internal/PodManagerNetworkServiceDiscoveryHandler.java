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
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.NetworkService;
import com.intel.rsa.podm.configuration.ConfigurationIOException;
import com.intel.rsa.podm.configuration.ConfigurationReader;
import com.intel.rsa.podm.configuration.dto.NetworkServiceDto;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;

@Dependent
public class PodManagerNetworkServiceDiscoveryHandler {

    private static final String DEFAULT_NAME = "Pod Manager Network Service";

    @Inject
    private Logger logger;

    @Inject
    private ConfigurationReader configurationReader;

    public void addNetworkService(Manager manager) {
        try {
            NetworkServiceDto networkServiceDto = configurationReader.readConfiguration("network-service", NetworkServiceDto.class);

            NetworkService networkService = manager.addNetworkService();
            networkService.updateModified();
            networkService.setName(networkServiceDto.getName());
            networkService.setDescription(networkServiceDto.getDescription());
            networkService.setState(networkServiceDto.getState());
            networkService.setHealth(networkServiceDto.getHealth());
            networkService.setHostName(networkServiceDto.getHostName());
            networkService.setFqdn(networkServiceDto.getFqdn());
            mapProtocols(networkServiceDto, networkService);
        } catch (ConfigurationIOException e) {
            logger.e("Pod manager network service configuration is unavailable, using defaults");
            addDefaultDisabledNetworkService(manager);
        }
    }

    private void mapProtocols(NetworkServiceDto networkServiceDto, NetworkService networkService) {
        networkService.setHttp(networkServiceDto.getHttp().getEnabled(), networkServiceDto.getHttp().getPort());
        networkService.setHttps(networkServiceDto.getHttps().getEnabled(), networkServiceDto.getHttps().getPort());
        networkService.setIpmi(networkServiceDto.getIpmi().getEnabled(), networkServiceDto.getIpmi().getPort());
        networkService.setSsh(networkServiceDto.getSsh().getEnabled(), networkServiceDto.getSsh().getPort());
        networkService.setSnmp(networkServiceDto.getSnmp().getEnabled(), networkServiceDto.getSnmp().getPort());
        networkService.setVirtualMedia(networkServiceDto.getVirtualMedia().getEnabled(), networkServiceDto.getVirtualMedia().getPort());
        networkService.setSsdp(networkServiceDto.getSsdp().getEnabled(), networkServiceDto.getSsdp().getPort());
        networkService.setTelnet(networkServiceDto.getTelnet().getEnabled(), networkServiceDto.getTelnet().getPort());
        networkService.setKvmip(networkServiceDto.getKvmip().getEnabled(), networkServiceDto.getKvmip().getPort());
    }

    private void addDefaultDisabledNetworkService(Manager manager) {
        NetworkService networkService = manager.addNetworkService();
        networkService.updateModified();
        networkService.setName(DEFAULT_NAME);
        networkService.setDescription(DEFAULT_NAME);
        networkService.setState(State.DISABLED);
        networkService.setHttp(false, null);
        networkService.setHttps(false, null);
        networkService.setIpmi(false, null);
        networkService.setSsh(false, null);
        networkService.setSnmp(false, null);
        networkService.setVirtualMedia(false, null);
        networkService.setSsdp(false, null);
        networkService.setTelnet(false, null);
        networkService.setKvmip(false, null);
    }
}
