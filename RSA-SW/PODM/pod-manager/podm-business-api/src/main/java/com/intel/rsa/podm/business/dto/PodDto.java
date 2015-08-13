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


import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.ArrayList;
import java.util.List;

import static java.util.Collections.unmodifiableList;

public final class PodDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final Location location;
    private final EnumStatus enumStatus;
    private final State state;
    private final Health health;
    private final Context context;
    private List<Context> managedBy;

    private PodDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        location = builder.location;
        enumStatus = builder.enumStatus;
        state = builder.state;
        health = builder.health;
        context = builder.context;
        managedBy = new ArrayList<>(builder.managedBy);
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

    public Location getLocation() {
        return location;
    }

    public EnumStatus getEnumStatus() {
        return enumStatus;
    }

    public State getState() {
        return state;
    }

    public Health getHealth() {
        return health;
    }

    public Context getContext() {
        return context;
    }

    public List<Context> getManagedBy() {
        return unmodifiableList(managedBy);
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private Location location;
        private EnumStatus enumStatus;
        private State state;
        private Health health;
        private Context context;
        private List<Context> managedBy;

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

        public Builder location(Location location) {
            this.location = location;
            return this;
        }

        public Builder enumStatus(EnumStatus enumStatus) {
            this.enumStatus = enumStatus;
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

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public Builder managedBy(List<Context> managedBy) {
            this.managedBy = managedBy;
            return this;
        }

        public PodDto build() {
            return new PodDto(this);
        }
    }
}
