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

public final class VlanNetworkInterfaceIpV4AddressDto {
    private String address;
    private String subnetMask;
    private String addressOrigin;
    private String gateway;
    private String hostname;
    private String fqdn;

    private VlanNetworkInterfaceIpV4AddressDto(Builder builder) {
        address = builder.address;
        subnetMask = builder.subnetMask;
        addressOrigin = builder.addressOrigin;
        gateway = builder.gateway;
        hostname = builder.hostname;
        fqdn = builder.fqdn;
    }

    public String getAddress() {
        return address;
    }

    public String getSubnetMask() {
        return subnetMask;
    }

    public String getAddressOrigin() {
        return addressOrigin;
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
        private String subnetMask;
        private String addressOrigin;
        private String gateway;
        private String hostname;
        private String fqdn;

        private Builder() {
        }

        public Builder address(String address) {
            this.address = address;
            return this;
        }

        public Builder subnetMask(String subnetMask) {
            this.subnetMask = subnetMask;
            return this;
        }

        public Builder addressOrigin(String addressOrigin) {
            this.addressOrigin = addressOrigin;
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

        public VlanNetworkInterfaceIpV4AddressDto build() {
            return new VlanNetworkInterfaceIpV4AddressDto(this);
        }
    }
}
