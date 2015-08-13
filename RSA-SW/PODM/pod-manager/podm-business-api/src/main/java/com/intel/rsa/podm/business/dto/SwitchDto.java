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

public final class SwitchDto {
    private final Id id;
    private final Context context;
    private final String name;
    private final Location location;
    private final OffsetDateTime modified;
    private final State state;
    private final Health health;
    private final Health healthRollup;
    private final Context containedBy;
    private final Iterable<Context> managedBy;

    private SwitchDto(Builder builder) {
        id = builder.id;
        context = builder.context;
        name = builder.name;
        location = builder.location;
        modified = builder.modified;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
        containedBy = builder.containedBy;
        managedBy = builder.managedBy;
    }

    public static Builder newBuilder() {
        return new Builder();
    }


    public Id getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public Location getLocation() {
        return location;
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

    public Health getHealthRollup() {
        return healthRollup;
    }

    public Iterable<Context> getManagedBy() {
        return managedBy;
    }

    public Context getContext() {
        return context;
    }

    public Context getContainedBy() {
        return containedBy;
    }

    public static final class Builder {
        private Id id;
        private Context context;
        private String name;
        private Location location;
        private OffsetDateTime modified;
        private State state;
        private Health health;
        private Health healthRollup;
        private Context containedBy;
        private Iterable<Context> managedBy;

        private Builder() {
        }

        public Builder id(Id id) {
            this.id = id;
            return this;
        }

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public Builder name(String name) {
            this.name = name;
            return this;
        }

        public Builder location(Location location) {
            this.location = location;
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

        public Builder healthRollup(Health healthRollup) {
            this.healthRollup = healthRollup;
            return this;
        }

        public Builder containedBy(Context containedBy) {
            this.containedBy = containedBy;
            return this;
        }

        public Builder managedBy(Iterable<Context> managedBy) {
            this.managedBy = managedBy;
            return this;
        }

        public SwitchDto build() {
            return new SwitchDto(this);
        }
    }
}
