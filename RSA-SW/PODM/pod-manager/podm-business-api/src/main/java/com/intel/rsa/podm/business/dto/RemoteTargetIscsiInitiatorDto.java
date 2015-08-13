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

public final class RemoteTargetIscsiInitiatorDto {

    private final String initiatorIQN;

    private RemoteTargetIscsiInitiatorDto(Builder builder) {
        this.initiatorIQN = builder.initiatorIQN;
    }

    public String getInitiatorIQN() {
        return initiatorIQN;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {

        private String initiatorIQN;

        private Builder() {

        }

        public RemoteTargetIscsiInitiatorDto build() {
            return new RemoteTargetIscsiInitiatorDto(this);
        }

        public Builder initiatorIQN(String initiatorIQN) {
            this.initiatorIQN = initiatorIQN;
            return this;
        }
    }
}
