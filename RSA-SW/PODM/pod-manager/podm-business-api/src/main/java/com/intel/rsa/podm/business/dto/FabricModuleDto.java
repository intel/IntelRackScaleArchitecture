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
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.List;

/**
 * Data transfer object definition for FabricModule.
 * Fields compliant with PSME API 20150330 fixed.
 */
public final class FabricModuleDto {
    private final Id id;
    private final Context context;
    private final String name;
    private final OffsetDateTime modified;
    private final State state;
    private final Health health;
    private final Location location;
    private final List<Context> managedBy;

    private FabricModuleDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        state = builder.state;
        health = builder.health;
        location = builder.location;
        managedBy = builder.managedBy;
        context = builder.context;
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

    public State getState() {
        return state;
    }

    public Health getHealth() {
        return health;
    }

    public Location getLocation() {
        return location;
    }

    public List<Context> getManagedBy() {
        return managedBy;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public Context getContext() {
        return context;
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private State state;
        private Health health;
        private Location location;
        private List<Context> managedBy;
        private Context context;

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

        public Builder state(State state) {
            this.state = state;
            return this;
        }

        public Builder health(Health health) {
            this.health = health;
            return this;
        }

        public Builder location(Location location) {
            this.location = location;
            return this;
        }

        public Builder managedBy(List<Context> managedBy) {
            this.managedBy = managedBy;
            return this;
        }

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public FabricModuleDto build() {
            return new FabricModuleDto(this);
        }
    }
}
