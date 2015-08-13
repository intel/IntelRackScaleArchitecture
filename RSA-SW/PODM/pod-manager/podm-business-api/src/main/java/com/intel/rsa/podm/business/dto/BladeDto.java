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
import com.intel.rsa.common.types.Power;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.ArrayList;
import java.util.Collection;

import static java.util.Collections.unmodifiableCollection;

public final class BladeDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final Location location;
    private final String manufacturer;
    private final String model;
    private final String serialNumber;
    private final State state;
    private final Health health;
    private final Health healthRollup;
    private final SystemType systemType;
    private final boolean storageCapable;
    private final int storageControllersCount;
    private final Power power;
    private final String currentBiosVersion;

    private final int processorCount;
    private final Integer processorSockets;
    private final String processorFamily;
    private final State processorState;
    private final Health processorHealth;
    private final Health processorHealthRollup;

    private final int totalSystemMemoryGB;
    private final Integer memorySockets;
    private final State memoryState;
    private final Health memoryHealth;
    private final Health memoryHealthRollup;
    private final BootDto boot;

    private final Context context;
    private final Collection<Context> managedBy;
    private final Collection<Context> containedBy;

    private final EnumStatus enumStatus;

    private BladeDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        location = builder.location;
        manufacturer = builder.manufacturer;
        model = builder.model;
        serialNumber = builder.serialNumber;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
        systemType = builder.systemType;
        storageCapable = builder.storageCapable;
        storageControllersCount = builder.storageControllersCount;
        power = builder.power;
        currentBiosVersion = builder.currentBiosVersion;
        processorCount = builder.processorCount;
        processorSockets = builder.processorSockets;
        processorFamily = builder.processorFamily;
        processorState = builder.processorState;
        processorHealth = builder.processorHealth;
        processorHealthRollup = builder.processorHealthRollup;
        totalSystemMemoryGB = builder.totalSystemMemoryGB;
        memoryState = builder.memoryState;
        memoryHealth = builder.memoryHealth;
        memoryHealthRollup = builder.memoryHealthRollup;
        boot = builder.boot;
        memorySockets = builder.memorySockets;
        context = builder.context;
        managedBy = unmodifiableCollection(builder.managedBy);
        containedBy = unmodifiableCollection(builder.containedBy);
        enumStatus = builder.enumStatus;
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

    public SystemType getSystemType() {
        return systemType;
    }

    public boolean isStorageCapable() {
        return storageCapable;
    }

    public int getStorageControllersCount() {
        return storageControllersCount;
    }

    public Power getPower() {
        return power;
    }

    public String getCurrentBiosVersion() {
        return currentBiosVersion;
    }

    public int getProcessorCount() {
        return processorCount;
    }

    public Integer getProcessorSockets() {
        return processorSockets;
    }

    public String getProcessorFamily() {
        return processorFamily;
    }

    public State getProcessorState() {
        return processorState;
    }

    public Health getProcessorHealth() {
        return processorHealth;
    }

    public Health getProcessorHealthRollup() {
        return processorHealthRollup;
    }

    public int getTotalSystemMemoryGB() {
        return totalSystemMemoryGB;
    }

    public Integer getMemorySockets() {
        return memorySockets;
    }

    public State getMemoryState() {
        return memoryState;
    }

    public Health getMemoryHealth() {
        return memoryHealth;
    }

    public Health getMemoryHealthRollup() {
        return memoryHealthRollup;
    }

    public BootDto getBoot() {
        return boot;
    }

    public Context getContext() {
        return context;
    }

    public Collection<Context> getManagedBy() {
        return managedBy;
    }

    public Collection<Context> getContainedBy() {
        return containedBy;
    }

    public EnumStatus getEnumStatus() {
        return enumStatus;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private Location location;
        private String manufacturer;
        private String model;
        private String serialNumber;
        private State state;
        private Health health;
        private Health healthRollup;
        private SystemType systemType;
        private boolean storageCapable;
        private int storageControllersCount;
        private Power power;
        private String currentBiosVersion;
        private int processorCount;
        private Integer processorSockets;
        private String processorFamily;
        private State processorState;
        private Health processorHealth;
        private Health processorHealthRollup;
        private int totalSystemMemoryGB;
        private Integer memorySockets;
        private State memoryState;
        private Health memoryHealth;
        private Health memoryHealthRollup;
        private BootDto boot;
        private Context context;
        private Collection<Context> managedBy = new ArrayList<>();
        private Collection<Context> containedBy = new ArrayList<>();
        private EnumStatus enumStatus;

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

        public Builder manufacturer(String manufacturer) {
            this.manufacturer = manufacturer;
            return this;
        }

        public Builder model(String model) {
            this.model = model;
            return this;
        }

        public Builder memorySockets(Integer memorySockets) {
            this.memorySockets = memorySockets;
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

        public Builder systemType(SystemType systemType) {
            this.systemType = systemType;
            return this;
        }

        public Builder storageCapable(boolean storageCapable) {
            this.storageCapable = storageCapable;
            return this;
        }

        public Builder storageControllersCount(int storageControllersCount) {
            this.storageControllersCount = storageControllersCount;
            return this;
        }

        public Builder power(Power power) {
            this.power = power;
            return this;
        }

        public Builder currentBiosVersion(String currentBiosVersion) {
            this.currentBiosVersion = currentBiosVersion;
            return this;
        }

        public Builder processorCount(int processorCount) {
            this.processorCount = processorCount;
            return this;
        }

        public Builder processorSockets(Integer processorSockets) {
            this.processorSockets = processorSockets;
            return this;
        }

        public Builder processorFamily(String processorFamily) {
            this.processorFamily = processorFamily;
            return this;
        }

        public Builder processorState(State processorState) {
            this.processorState = processorState;
            return this;
        }

        public Builder processorHealth(Health processorHealth) {
            this.processorHealth = processorHealth;
            return this;
        }

        public Builder processorHealthRollup(Health processorHealthRollup) {
            this.processorHealthRollup = processorHealthRollup;
            return this;
        }

        public Builder totalSystemMemoryGB(int totalSystemMemoryGB) {
            this.totalSystemMemoryGB = totalSystemMemoryGB;
            return this;
        }

        public Builder memoryState(State memoryState) {
            this.memoryState = memoryState;
            return this;
        }

        public Builder memoryHealth(Health memoryHealth) {
            this.memoryHealth = memoryHealth;
            return this;
        }

        public Builder memoryHealthRollup(Health memoryHealthRollup) {
            this.memoryHealthRollup = memoryHealthRollup;
            return this;
        }

        public Builder boot(BootDto boot) {
            this.boot = boot;
            return this;
        }

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public Builder managedBy(Collection<Context> managersId) {
            this.managedBy.addAll(managersId);
            return this;
        }

        public Builder containedBy(Context containedById) {
            this.containedBy.add(containedById);
            return this;
        }

        public Builder enumStatus(EnumStatus enumStatus) {
            this.enumStatus = enumStatus;
            return this;
        }

        public BladeDto build() {
            return new BladeDto(this);
        }
    }
}
