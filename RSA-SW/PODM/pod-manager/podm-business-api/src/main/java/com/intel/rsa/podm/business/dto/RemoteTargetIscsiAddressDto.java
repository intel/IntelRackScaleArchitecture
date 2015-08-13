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

import java.util.List;

public final class RemoteTargetIscsiAddressDto {

    private final List<Integer> targetLUN;
    private final String targetIQN;
    private final String targetPortalIP;
    private final int targetPortalPort;

    private RemoteTargetIscsiAddressDto(Builder builder) {
        this.targetLUN = builder.targetLUN;
        this.targetIQN = builder.targetIQN;
        this.targetPortalIP = builder.targetPortalIP;
        this.targetPortalPort = builder.targetPortalPort;
    }

    public List<Integer> getTargetLUN() {
        return targetLUN;
    }

    public String getTargetIQN() {
        return targetIQN;
    }

    public String getTargetPortalIP() {
        return targetPortalIP;
    }

    public int getTargetPortalPort() {
        return targetPortalPort;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {

        private List<Integer> targetLUN;
        private String targetIQN;
        private String targetPortalIP;
        private int targetPortalPort;

        private Builder() {

        }

        public RemoteTargetIscsiAddressDto build() {
            return new RemoteTargetIscsiAddressDto(this);
        }

        public Builder targetPortalPort(int targetPortalPort) {
            this.targetPortalPort = targetPortalPort;
            return this;
        }

        public Builder targetPortalIP(String targetPortalIP) {
            this.targetPortalIP = targetPortalIP;
            return this;
        }

        public Builder targetIQN(String targetIQN) {
            this.targetIQN = targetIQN;
            return this;
        }

        public Builder targetLUN(List<Integer> targetLUN) {
            this.targetLUN = targetLUN;
            return this;
        }
    }
}
