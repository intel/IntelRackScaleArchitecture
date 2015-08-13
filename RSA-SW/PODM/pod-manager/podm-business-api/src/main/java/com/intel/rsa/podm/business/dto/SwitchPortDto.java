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

import com.intel.rsa.common.types.AdministrativeState;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.OperationalState;
import com.intel.rsa.common.types.LinkType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;

public final class SwitchPortDto {
    private final Id id;
    private final String name;
    private final Location location;
    private final OffsetDateTime modified;
    private final State state;
    private final Health health;
    private final Health healthRollup;
    private final LinkType linkType;
    private final OperationalState operationalState;
    private final AdministrativeState administrativeState;
    private final Integer linkSpeedGbps;
    private final String neighbourPort;
    private final Iterable<Context> managedBy;
    private final Context containedBy;
    private final Context context;

    private SwitchPortDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        location = builder.location;
        modified = builder.modified;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
        linkType = builder.linkType;
        operationalState = builder.operationalState;
        administrativeState = builder.administrativeState;
        linkSpeedGbps = builder.linkSpeedGbps;
        neighbourPort = builder.neighbourPort;
        managedBy = builder.managedBy;
        containedBy = builder.containedBy;
        context = builder.context;
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

    public LinkType getLinkType() {
        return linkType;
    }

    public OperationalState getOperationalState() {
        return operationalState;
    }

    public AdministrativeState getAdministrativeState() {
        return administrativeState;
    }

    public Integer getLinkSpeedGbps() {
        return linkSpeedGbps;
    }

    public String getNeighbourPort() {
        return neighbourPort;
    }

    public Iterable<Context> getManagedBy() {
        return managedBy;
    }

    public Context getContainedBy() {
        return containedBy;
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
        private Location location;
        private OffsetDateTime modified;
        private State state;
        private Health health;
        private Health healthRollup;
        private LinkType linkType;
        private OperationalState operationalState;
        private AdministrativeState administrativeState;
        private Integer linkSpeedGbps;
        private String neighbourPort;
        private Iterable<Context> managedBy;
        private Context containedBy;
        private Context context;

        private Builder() {
        }

        public Builder id(final Id id) {
            this.id = id;
            return this;
        }

        public Builder name(final String name) {
            this.name = name;
            return this;
        }

        public Builder location(final Location location) {
            this.location = location;
            return this;
        }

        public Builder modified(final OffsetDateTime modified) {
            this.modified = modified;
            return this;
        }

        public Builder state(final State state) {
            this.state = state;
            return this;
        }

        public Builder health(final Health health) {
            this.health = health;
            return this;
        }

        public Builder healthRollup(final Health healthRollup) {
            this.healthRollup = healthRollup;
            return this;
        }

        public Builder linkType(final LinkType linkType) {
            this.linkType = linkType;
            return this;
        }

        public Builder operationalState(final OperationalState operationalState) {
            this.operationalState = operationalState;
            return this;
        }

        public Builder administrativeState(final AdministrativeState administrativeState) {
            this.administrativeState = administrativeState;
            return this;
        }

        public Builder linkSpeedGbps(final Integer linkSpeedGbps) {
            this.linkSpeedGbps = linkSpeedGbps;
            return this;
        }

        public Builder neighbourPort(final String neighbourPort) {
            this.neighbourPort = neighbourPort;
            return this;
        }

        public Builder managedBy(final Iterable<Context> managedBy) {
            this.managedBy = managedBy;
            return this;
        }

        public Builder containedBy(final Context containedBy) {
            this.containedBy = containedBy;
            return this;
        }

        public Builder context(final Context context) {
            this.context = context;
            return this;
        }

        public SwitchPortDto build() {
            return new SwitchPortDto(this);
        }
    }
}
