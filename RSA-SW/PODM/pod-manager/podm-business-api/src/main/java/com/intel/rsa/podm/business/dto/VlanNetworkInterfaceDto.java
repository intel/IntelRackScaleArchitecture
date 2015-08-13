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
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.State;

import java.time.OffsetDateTime;
import java.util.Collection;

/**
 * Class transferring VlanNetworkInterface data.
 * Fields compliant with PSME API 20150427.
 */
public final class VlanNetworkInterfaceDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final String description;
    private final State state;
    private final Health health;
    private final Boolean vlanEnabled;
    private final Integer vlanId;
    private Collection<VlanNetworkInterfaceIpV4AddressDto> ipV4Addresses;
    private Collection<VlanNetworkInterfaceIpV6AddressDto> ipV6Addresses;

    private VlanNetworkInterfaceDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        description = builder.description;
        state = builder.state;
        health = builder.health;
        vlanEnabled = builder.vlanEnabled;
        vlanId = builder.vlanId;
        ipV4Addresses = builder.ipV4Addresses;
        ipV6Addresses = builder.ipV6Addresses;
    }

    public Id getId() {
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

    public Boolean getVlanEnabled() {
        return vlanEnabled;
    }

    public Integer getVlanId() {
        return vlanId;
    }

    public Collection<VlanNetworkInterfaceIpV4AddressDto> getIpV4Addresses() {
        return ipV4Addresses;
    }

    public Collection<VlanNetworkInterfaceIpV6AddressDto> getIpV6Addresses() {
        return ipV6Addresses;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private String description;
        private State state;
        private Health health;
        private Boolean vlanEnabled;
        private Integer vlanId;
        private Collection<VlanNetworkInterfaceIpV4AddressDto> ipV4Addresses;
        private Collection<VlanNetworkInterfaceIpV6AddressDto> ipV6Addresses;

        private Builder() {
        }

        public Builder id(Id id) {
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

        public Builder vlanEnabled(Boolean vlanEnabled) {
            this.vlanEnabled = vlanEnabled;
            return this;
        }

        public Builder vlanId(Integer vlanId) {
            this.vlanId = vlanId;
            return this;
        }

        public Builder ipV4Addresses(Collection<VlanNetworkInterfaceIpV4AddressDto> ipV4Addresses) {
            this.ipV4Addresses = ipV4Addresses;
            return this;
        }

        public Builder ipV6Addresses(Collection<VlanNetworkInterfaceIpV6AddressDto> ipV6Addresses) {
            this.ipV6Addresses = ipV6Addresses;
            return this;
        }

        public VlanNetworkInterfaceDto build() {
            return new VlanNetworkInterfaceDto(this);
        }
    }
}
