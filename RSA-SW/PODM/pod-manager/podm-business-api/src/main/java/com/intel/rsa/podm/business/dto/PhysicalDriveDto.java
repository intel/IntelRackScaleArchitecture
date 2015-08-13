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

import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.StorageControllerInterface;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.Collection;

public final class PhysicalDriveDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final StorageControllerInterface controllerInterface;
    private final int capacityGB;
    private final DriveType type;
    private final Integer rpm;
    private final String manufacturer;
    private final String model;
    private final String serialNumber;
    private final State state;
    private final Health health;
    private final Health healthRollup;

    private final Collection<Context> usedBy;
    private final Collection<Context> managedBy;

    private final Context context;

    private PhysicalDriveDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        controllerInterface = builder.controllerInterface;
        capacityGB = builder.capacityGB;
        type = builder.type;
        rpm = builder.rpm;
        manufacturer = builder.manufacturer;
        model = builder.model;
        serialNumber = builder.serialNumber;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
        usedBy = builder.usedBy;
        managedBy = builder.managedBy;
        context = builder.context;
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

    public StorageControllerInterface getControllerInterface() {
        return controllerInterface;
    }

    public int getCapacityGB() {
        return capacityGB;
    }

    public DriveType getType() {
        return type;
    }

    public Integer getRpm() {
        return rpm;
    }

    public String getManufacturer() {
        return manufacturer;
    }

    public String getModel() {
        return model;
    }

    public String getSerialNumber() {
        return serialNumber;
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

    public Collection<Context> getUsedBy() {
        return usedBy;
    }

    public Collection<Context> getManagedBy() {
        return managedBy;
    }

    public Context getContext() {
        return context;
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private StorageControllerInterface controllerInterface;
        private int capacityGB;
        private DriveType type;
        private Integer rpm;
        private String manufacturer;
        private String model;
        private String serialNumber;
        private State state;
        private Health health;
        private Health healthRollup;
        private Collection<Context> usedBy;
        private Collection<Context> managedBy;
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

        public Builder controllerInterface(StorageControllerInterface controllerInterface) {
            this.controllerInterface = controllerInterface;
            return this;
        }

        public Builder capacityGB(int capacityGB) {
            this.capacityGB = capacityGB;
            return this;
        }

        public Builder type(DriveType type) {
            this.type = type;
            return this;
        }

        public Builder rpm(Integer rpm) {
            this.rpm = rpm;
            return this;
        }

        public Builder manufacturer(String manufacturer) {
            this.manufacturer = manufacturer;
            return this;
        }

        public Builder model(String model) {
            this.model = model;
            return this;
        }

        public Builder serialNumber(String serialNumber) {
            this.serialNumber = serialNumber;
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

        public Builder usedBy(Collection<Context> usedBy) {
            this.usedBy = usedBy;
            return this;
        }

        public Builder managedBy(Collection<Context> managedBy) {
            this.managedBy = managedBy;
            return this;
        }

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public PhysicalDriveDto build() {
            return new PhysicalDriveDto(this);
        }
    }
}
