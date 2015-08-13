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

package com.intel.rsa.client;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkService;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkServiceProtocol;
import com.intel.rsa.client.api.reader.rss.RssNetworkService;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.client.psme.PsmeNetworkServiceProtocolImpl;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;

public class RsaNetworkServiceImpl extends ResourceImpl implements PsmeNetworkService, RssNetworkService {
    @JsonProperty ("Description")
    private String description;
    @JsonProperty ("FQDN")
    private String fqdn;
    @JsonProperty ("HostName")
    private String hostName;
    @JsonProperty ("Status")
    private StatusDto status;
    @JsonProperty("HTTP")
    private PsmeNetworkServiceProtocolImpl http;
    @JsonProperty("HTTPS")
    private PsmeNetworkServiceProtocolImpl https;
    @JsonProperty("IPMI")
    private PsmeNetworkServiceProtocolImpl ipmi;
    @JsonProperty("SSH")
    private PsmeNetworkServiceProtocolImpl ssh;
    @JsonProperty("SNMP")
    private PsmeNetworkServiceProtocolImpl snmp;
    @JsonProperty("VirtualMedia")
    private PsmeNetworkServiceProtocolImpl virtualMedia;
    @JsonProperty("SSDP")
    private PsmeNetworkServiceProtocolImpl ssdp;
    @JsonProperty("Telnet")
    private PsmeNetworkServiceProtocolImpl telnet;
    @JsonProperty("KVMIP")
    private PsmeNetworkServiceProtocolImpl kvmip;

    @Override
    public String getDescription() {
        return description;
    }

    @Override
    public String getFqdn() {
        return fqdn;
    }

    @Override
    public String getHostName() {
        return hostName;
    }

    @Override
    public State getState() {
        return status == null ? null : status.getState();
    }

    @Override
    public Health getHealth() {
        return status == null ? null : status.getHealth();
    }

    @Override
    public PsmeNetworkServiceProtocol getHttp() {
        return http;
    }

    @Override
    public PsmeNetworkServiceProtocol getHttps() {
        return https;
    }

    @Override
    public PsmeNetworkServiceProtocol getIpmi() {
        return ipmi;
    }

    @Override
    public PsmeNetworkServiceProtocol getSsh() {
        return ssh;
    }

    @Override
    public PsmeNetworkServiceProtocol getSnmp() {
        return snmp;
    }

    @Override
    public PsmeNetworkServiceProtocol getVirtualMedia() {
        return virtualMedia;
    }

    @Override
    public PsmeNetworkServiceProtocol getSsdp() {
        return ssdp;
    }

    @Override
    public PsmeNetworkServiceProtocol getTelnet() {
        return telnet;
    }

    @Override
    public PsmeNetworkServiceProtocol getKvmip() {
        return kvmip;
    }
}
