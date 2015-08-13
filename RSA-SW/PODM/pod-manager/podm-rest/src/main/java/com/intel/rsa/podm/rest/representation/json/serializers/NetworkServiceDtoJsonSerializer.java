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

import com.intel.rsa.podm.business.dto.NetworkServiceDto;
import com.intel.rsa.podm.rest.representation.json.templates.NetworkServiceJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.NetworkProtocolJson;

import static com.intel.rsa.podm.rest.odataid.ODataId.oDataId;
import static java.net.URI.create;

public final class NetworkServiceDtoJsonSerializer extends DtoJsonSerializer<NetworkServiceDto> {
    public NetworkServiceDtoJsonSerializer() {
        super(NetworkServiceDto.class);
    }

    @Override
    protected NetworkServiceJson translate(NetworkServiceDto dto) {
        NetworkServiceJson result = new NetworkServiceJson();

        result.oDataContext = create("/rest/v1/$metadata#RSAManagers/Links/Members/1/Links/NetworkService/$entity");
        result.oDataId = oDataId(context.getRequestPath());

        result.id = dto.getId();
        result.name = dto.getName();
        result.modified = dto.getModified();
        result.description = dto.getDescription();
        result.status.health = dto.getHealth();
        result.status.state = dto.getState();
        result.hostName = dto.getHostName();
        result.fqdn = dto.getFqdn();
        mapProtocols(dto, result);

        return result;
    }

    private void mapProtocols(NetworkServiceDto dto, NetworkServiceJson result) {
        result.http = new NetworkProtocolJson(dto.getHttp().isEnabled(), dto.getHttp().getPort());
        result.https = new NetworkProtocolJson(dto.getHttps().isEnabled(), dto.getHttps().getPort());
        result.ipmi = new NetworkProtocolJson(dto.getIpmi().isEnabled(), dto.getIpmi().getPort());
        result.ssh = new NetworkProtocolJson(dto.getSsh().isEnabled(), dto.getSsh().getPort());
        result.snmp = new NetworkProtocolJson(dto.getSnmp().isEnabled(), dto.getSnmp().getPort());
        result.virtualMedia = new NetworkProtocolJson(dto.getVirtualMedia().isEnabled(), dto.getVirtualMedia().getPort());
        result.ssdp = new NetworkProtocolJson(dto.getSsdp().isEnabled(), dto.getSsdp().getPort());
        result.telnet = new NetworkProtocolJson(dto.getTelnet().isEnabled(), dto.getTelnet().getPort());
        result.kvmip = new NetworkProtocolJson(dto.getKvmip().isEnabled(), dto.getKvmip().getPort());
    }
}
