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
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.Collection;

import static java.util.Collections.unmodifiableCollection;

public final class RemoteTargetDto {

    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final State state;
    private final Health health;
    private final Health healthRollup;

    private final String type;

    private final Context context;

    private final Collection<RemoteTargetIscsiAddressDto> iscsiAddresses;
    private final Collection<RemoteTargetIscsiInitiatorDto> initiators;
    private final Collection<Context> logicalDrives;
    private final Collection<Context> managedBy;

    private final EnumStatus enumStatus;

    private RemoteTargetDto(Builder builder) {
        this.id = builder.id;
        this.name = builder.name;
        this.modified = builder.modified;
        this.state = builder.state;
        this.health = builder.health;
        this.healthRollup = builder.healthRollup;
        this.type = builder.type;

        this.iscsiAddresses = unmodifiableCollection(builder.iscsiAddresses);
        this.initiators = unmodifiableCollection(builder.initiators);
        this.logicalDrives = unmodifiableCollection(builder.logicalDrives);
        this.managedBy = unmodifiableCollection(builder.managedby);
        this.enumStatus = builder.enumStatus;
        this.context = builder.context;
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

    public String getType() {
        return type;
    }

    public Context getContext() {
        return context;
    }

    public Collection<RemoteTargetIscsiAddressDto> getIscsiAddresses() {
        return iscsiAddresses;
    }

    public Collection<RemoteTargetIscsiInitiatorDto> getInitiators() {
        return initiators;
    }

    public Collection<Context> getLogicalDrives() {
        return logicalDrives;
    }

    public Collection<Context> getManagedBy() {
        return managedBy;
    }

    public EnumStatus getEnumStatus() {
        return enumStatus;
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private State state;
        private Health health;
        private Health healthRollup;
        private EnumStatus enumStatus;
        private String type;
        private Context context;
        private Collection<RemoteTargetIscsiAddressDto> iscsiAddresses;
        private Collection<RemoteTargetIscsiInitiatorDto> initiators;
        private Collection<Context> logicalDrives;
        private Collection<Context> managedby;

        private Builder() {
        }

        public RemoteTargetDto build() {
            return new RemoteTargetDto(this);
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

        public Builder healthRollup(Health healthRollup) {
            this.healthRollup = healthRollup;
            return this;
        }

        public Builder enumStatus(EnumStatus enumStatus) {
            this.enumStatus = enumStatus;
            return this;
        }

        public Builder type(String type) {
            this.type = type;
            return this;
        }

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public Builder iscsiAddresses(Collection<RemoteTargetIscsiAddressDto> iscsiAddresses) {
            this.iscsiAddresses = iscsiAddresses;
            return this;
        }

        public Builder initiators(Collection<RemoteTargetIscsiInitiatorDto> initiators) {
            this.initiators = initiators;
            return this;
        }

        public Builder logicalDrives(Collection<Context> logicalDrives) {
            this.logicalDrives = logicalDrives;
            return this;
        }

        public Builder managedby(Collection<Context> managedby) {
            this.managedby = managedby;
            return this;
        }
    }
}
