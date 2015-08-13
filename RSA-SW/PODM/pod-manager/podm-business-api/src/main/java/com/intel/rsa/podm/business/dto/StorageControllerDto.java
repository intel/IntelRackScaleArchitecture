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
import com.intel.rsa.common.types.StorageControllerInterface;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;

public final class StorageControllerDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final Location location;
    private final StorageControllerInterface controllerInterface;
    private final int driveCount;
    private final State state;
    private final Health health;
    private final Health healthRollup;

    private final Context context;

    private StorageControllerDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        location = builder.location;
        controllerInterface = builder.controllerInterface;
        driveCount = builder.driveCount;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
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

    public Location getLocation() {
        return location;
    }

    public StorageControllerInterface getControllerInterface() {
        return controllerInterface;
    }

    public int getDriveCount() {
        return driveCount;
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

    public Context getContext() {
        return context;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private Location location;
        private StorageControllerInterface controllerInterface;
        private int driveCount;
        private State state;
        private Health health;
        private Health healthRollup;
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

        public Builder location(Location location) {
            this.location = location;
            return this;
        }

        public Builder controllerInterface(StorageControllerInterface controllerInterface) {
            this.controllerInterface = controllerInterface;
            return this;
        }

        public Builder driveCount(int driveCount) {
            this.driveCount = driveCount;
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

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public StorageControllerDto build() {
            return new StorageControllerDto(this);
        }
    }
}
