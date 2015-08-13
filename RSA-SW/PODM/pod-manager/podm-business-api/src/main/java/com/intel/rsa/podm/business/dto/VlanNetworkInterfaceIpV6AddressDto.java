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

public final class VlanNetworkInterfaceIpV6AddressDto {
    private String address;
    private Integer prefixLength;
    private String addressOrigin;
    private String scope;
    private String gateway;
    private String hostname;
    private String fqdn;

    private VlanNetworkInterfaceIpV6AddressDto(Builder builder) {
        address = builder.address;
        prefixLength = builder.prefixLength;
        addressOrigin = builder.addressOrigin;
        scope = builder.scope;
        gateway = builder.gateway;
        hostname = builder.hostname;
        fqdn = builder.fqdn;
    }

    public String getAddress() {
        return address;
    }

    public Integer getPrefixLength() {
        return prefixLength;
    }

    public String getAddressOrigin() {
        return addressOrigin;
    }

    public String getScope() {
        return scope;
    }

    public String getGateway() {
        return gateway;
    }

    public String getHostname() {
        return hostname;
    }

    public String getFqdn() {
        return fqdn;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private String address;
        private Integer prefixLength;
        private String addressOrigin;
        private String scope;
        private String gateway;
        private String hostname;
        private String fqdn;

        private Builder() {
        }

        public Builder address(String address) {
            this.address = address;
            return this;
        }

        public Builder prefixLength(Integer prefixLength) {
            this.prefixLength = prefixLength;
            return this;
        }

        public Builder addressOrigin(String addressOrigin) {
            this.addressOrigin = addressOrigin;
            return this;
        }

        public Builder scope(String scope) {
            this.scope = scope;
            return this;
        }

        public Builder gateway(String gateway) {
            this.gateway = gateway;
            return this;
        }

        public Builder hostname(String hostname) {
            this.hostname = hostname;
            return this;
        }

        public Builder fqdn(String fqdn) {
            this.fqdn = fqdn;
            return this;
        }

        public VlanNetworkInterfaceIpV6AddressDto build() {
            return new VlanNetworkInterfaceIpV6AddressDto(this);
        }
    }
}
