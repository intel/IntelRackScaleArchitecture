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

package com.intel.rsa.podm.discovery.external.mapper.rss;

import com.intel.rsa.client.api.reader.RsaNetworkService;
import com.intel.rsa.client.api.reader.RsaNetworkServiceProtocol;
import com.intel.rsa.podm.business.entities.assets.NetworkService;
import com.intel.rsa.podm.business.entities.assets.NetworkServiceProtocol;
import com.intel.rsa.podm.discovery.external.mapper.DomainObjectMapper;

import javax.enterprise.context.Dependent;

@Dependent
public class NetworkServiceMapper extends DomainObjectMapper<RsaNetworkService, NetworkService> {
    protected NetworkServiceMapper() {
        super(RsaNetworkService.class, NetworkService.class);
    }

    @Override
    protected void performNotAutomatedMapping(RsaNetworkService source, NetworkService target) {
        super.performNotAutomatedMapping(source, target);
        target.updateModified();
        mapProtocol(source.getHttp(), target.getHttp());
        mapProtocol(source.getHttps(), target.getHttps());
        mapProtocol(source.getKvmip(), target.getKvmip());
        mapProtocol(source.getIpmi(), target.getIpmi());
        mapProtocol(source.getSnmp(), target.getSnmp());
        mapProtocol(source.getSsdp(), target.getSsdp());
        mapProtocol(source.getSsh(), target.getSsh());
        mapProtocol(source.getTelnet(), target.getTelnet());
        mapProtocol(source.getVirtualMedia(), target.getVirtualMedia());
    }

    private void mapProtocol(RsaNetworkServiceProtocol source, NetworkServiceProtocol target) {
        if (source != null) {
            target.setEnabled(source.isEnabled());
            target.setPort(source.getPort());
        }
    }
}
