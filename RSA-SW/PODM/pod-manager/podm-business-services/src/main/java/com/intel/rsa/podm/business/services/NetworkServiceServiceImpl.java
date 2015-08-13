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

import com.intel.rsa.podm.business.dto.NetworkProtocolDto;
import com.intel.rsa.podm.business.dto.NetworkServiceDto;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.NetworkService;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.transaction.Transactional;

@RequestScoped
@Transactional(Transactional.TxType.REQUIRED)
public class NetworkServiceServiceImpl implements NetworkServiceService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public NetworkServiceDto getNetworkService(Context managerContext) throws EntityNotFoundException {
        Manager manager = (Manager) traverser.traverse(managerContext);
        return map(manager.getNetworkService());
    }

    private NetworkServiceDto map(NetworkService networkService) {
        return NetworkServiceDto.newBuilder()
                .id("NetworkService")
                .name(networkService.getName())
                .description(networkService.getDescription())
                .fqdn(networkService.getFqdn())
                .health(networkService.getHealth())
                .state(networkService.getState())
                .hostName(networkService.getHostName())
                .modified(networkService.getModified())
                .http(new NetworkProtocolDto(networkService.getHttp().isEnabled(), networkService.getHttp().getPort()))
                .https(new NetworkProtocolDto(networkService.getHttps().isEnabled(), networkService.getHttps().getPort()))
                .ipmi(new NetworkProtocolDto(networkService.getIpmi().isEnabled(), networkService.getIpmi().getPort()))
                .ssh(new NetworkProtocolDto(networkService.getSsh().isEnabled(), networkService.getSsh().getPort()))
                .snmp(new NetworkProtocolDto(networkService.getSnmp().isEnabled(), networkService.getSnmp().getPort()))
                .virtualMedia(new NetworkProtocolDto(networkService.getVirtualMedia().isEnabled(), networkService.getVirtualMedia().getPort()))
                .ssdp(new NetworkProtocolDto(networkService.getSsdp().isEnabled(), networkService.getSsdp().getPort()))
                .telnet(new NetworkProtocolDto(networkService.getTelnet().isEnabled(), networkService.getTelnet().getPort()))
                .kvmip(new NetworkProtocolDto(networkService.getKvmip().isEnabled(), networkService.getKvmip().getPort()))
                .build();
    }
}
