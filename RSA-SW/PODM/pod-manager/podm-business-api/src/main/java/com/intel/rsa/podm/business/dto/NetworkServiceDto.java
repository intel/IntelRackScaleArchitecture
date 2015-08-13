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

package com.intel.rsa.podm.business.dto;

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;

import java.time.OffsetDateTime;

/**
 * Class transferring NetworkService data. Fields compliant with PSME API 20150330 fixed.
 */
public final class NetworkServiceDto {
    private final String id;
    private final String name;
    private final OffsetDateTime modified;
    private final String description;
    private final State state;
    private final Health health;
    private final String hostName;
    private final String fqdn;
    private final NetworkProtocolDto http;
    private final NetworkProtocolDto https;
    private final NetworkProtocolDto ipmi;
    private final NetworkProtocolDto ssh;
    private final NetworkProtocolDto snmp;
    private final NetworkProtocolDto virtualMedia;
    private final NetworkProtocolDto ssdp;
    private final NetworkProtocolDto telnet;
    private final NetworkProtocolDto kvmip;

    private NetworkServiceDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        description = builder.description;
        state = builder.state;
        health = builder.health;
        hostName = builder.hostName;
        fqdn = builder.fqdn;
        http = builder.http;
        https = builder.https;
        ipmi = builder.ipmi;
        ssh = builder.ssh;
        snmp = builder.snmp;
        virtualMedia = builder.virtualMedia;
        ssdp = builder.ssdp;
        telnet = builder.telnet;
        kvmip = builder.kvmip;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public OffsetDateTime getModified() {
        return modified;
    }

    public String getDescription() {
        return description;
    }

    public State getState() {
        return state;
    }

    public Health getHealth() {
        return health;
    }

    public String getHostName() {
        return hostName;
    }

    public String getFqdn() {
        return fqdn;
    }

    public NetworkProtocolDto getHttp() {
        return http;
    }

    public NetworkProtocolDto getHttps() {
        return https;
    }

    public NetworkProtocolDto getIpmi() {
        return ipmi;
    }

    public NetworkProtocolDto getSsh() {
        return ssh;
    }

    public NetworkProtocolDto getSnmp() {
        return snmp;
    }

    public NetworkProtocolDto getVirtualMedia() {
        return virtualMedia;
    }

    public NetworkProtocolDto getSsdp() {
        return ssdp;
    }

    public NetworkProtocolDto getTelnet() {
        return telnet;
    }

    public NetworkProtocolDto getKvmip() {
        return kvmip;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private String id;
        private String name;
        private OffsetDateTime modified;
        private String description;
        private State state;
        private Health health;
        private String hostName;
        private String fqdn;
        private NetworkProtocolDto http;
        private NetworkProtocolDto https;
        private NetworkProtocolDto ipmi;
        private NetworkProtocolDto ssh;
        private NetworkProtocolDto snmp;
        private NetworkProtocolDto virtualMedia;
        private NetworkProtocolDto ssdp;
        private NetworkProtocolDto telnet;
        private NetworkProtocolDto kvmip;

        private Builder() {
        }

        public Builder id(String id) {
            this.id = id;
            return this;
        }

        public Builder name(String name) {
            this.name = name;
            return this;
        }

        public Builder modified(OffsetDateTime modified) {
            this.modified = modified;
            return this;
        }

        public Builder description(String description) {
            this.description = description;
            return this;
        }

        public Builder state(State state) {
            this.state = state;
            return this;
        }

        public Builder health(Health health) {
            this.health = health;
            return this;
        }

        public Builder hostName(String hostName) {
            this.hostName = hostName;
            return this;
        }

        public Builder fqdn(String fqdn) {
            this.fqdn = fqdn;
            return this;
        }

        public Builder http(NetworkProtocolDto http) {
            this.http = http;
            return this;
        }

        public Builder https(NetworkProtocolDto https) {
            this.https = https;
            return this;
        }

        public Builder ipmi(NetworkProtocolDto ipmi) {
            this.ipmi = ipmi;
            return this;
        }

        public Builder ssh(NetworkProtocolDto ssh) {
            this.ssh = ssh;
            return this;
        }

        public Builder snmp(NetworkProtocolDto snmp) {
            this.snmp = snmp;
            return this;
        }

        public Builder virtualMedia(NetworkProtocolDto virtualMedia) {
            this.virtualMedia = virtualMedia;
            return this;
        }

        public Builder ssdp(NetworkProtocolDto ssdp) {
            this.ssdp = ssdp;
            return this;
        }

        public Builder telnet(NetworkProtocolDto telnet) {
            this.telnet = telnet;
            return this;
        }

        public Builder kvmip(NetworkProtocolDto kvmip) {
            this.kvmip = kvmip;
            return this;
        }

        public NetworkServiceDto build() {
            return new NetworkServiceDto(this);
        }
    }
}
