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

import com.intel.rsa.common.types.ConnectionType;

public final class ConsoleDto {
    private final boolean enabled;
    private final Integer maxConcurrentSessions;
    private final ConnectionType connectTypesSupported;

    private ConsoleDto(Builder builder) {
        enabled = builder.enabled;
        maxConcurrentSessions = builder.maxConcurrentSessions;
        connectTypesSupported = builder.connectTypesSupported;
    }

    public boolean isEnabled() {
        return enabled;
    }

    public Integer getMaxConcurrentSessions() {
        return maxConcurrentSessions;
    }

    public ConnectionType getConnectTypesSupported() {
        return connectTypesSupported;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private boolean enabled;
        private Integer maxConcurrentSessions;
        private ConnectionType connectTypesSupported;

        private Builder() {
        }

        public Builder enabled(boolean enabled) {
            this.enabled = enabled;
            return this;
        }

        public Builder maxConcurrentSessions(Integer maxConcurrentSessions) {
            this.maxConcurrentSessions = maxConcurrentSessions;
            return this;
        }

        public Builder connectTypesSupported(ConnectionType connectTypesSupported) {
            this.connectTypesSupported = connectTypesSupported;
            return this;
        }

        public ConsoleDto build() {
            return new ConsoleDto(this);
        }
    }
 }
