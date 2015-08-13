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

import java.time.OffsetDateTime;
import java.util.UUID;

public final class ServiceRootDto {
    private final OffsetDateTime modified;
    private final OffsetDateTime time;
    private final UUID uuid;
    private final String name;
    private final String serviceVersion;
    private final String id;

    private ServiceRootDto(Builder builder) {
        id = builder.id;
        modified = builder.modified;
        time = builder.time;
        uuid = builder.uuid;
        serviceVersion = builder.serviceVersion;
        name = builder.name;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public OffsetDateTime getModified() {
        return modified;
    }

    public OffsetDateTime getTime() {
        return time;
    }

    public UUID getUuid() {
        return uuid;
    }

    public String getServiceVersion() {
        return serviceVersion;
    }

    public String getName() {
        return name;
    }

    public String getId() {
        return id;
    }

    public static final class Builder {
        private OffsetDateTime modified;
        private OffsetDateTime time;
        private UUID uuid;
        private String name;
        private String serviceVersion;
        private String id;

        private Builder() {
        }

        public Builder modified(OffsetDateTime modified) {
            this.modified = modified;
            return this;
        }

        public Builder time(OffsetDateTime time) {
            this.time = time;
            return this;
        }

        public Builder uuid(UUID uuid) {
            this.uuid = uuid;
            return this;
        }

        public Builder name(String name) {
            this.name = name;
            return this;
        }

        public Builder serviceVersion(String serviceVersion) {
            this.serviceVersion = serviceVersion;
            return this;
        }

        public ServiceRootDto build() {
            return new ServiceRootDto(this);
        }

        public Builder id(String id) {
            this.id = id;
            return this;
        }
    }
}
