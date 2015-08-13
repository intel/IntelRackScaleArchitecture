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
import com.intel.rsa.common.types.VolumeMode;
import com.intel.rsa.common.types.VolumeType;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.Collection;

public final class LogicalDriveDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final VolumeType type;
    private final VolumeMode mode;
    private final boolean writeProtected;
    private final String assetTag;
    private final int capacityGB;
    private final String image;
    private final boolean bootable;
    private final Boolean snapshot;
    private final State state;
    private final Health health;
    private final Health healthRollup;

    private final Collection<Context> logicalDrives;
    private final Collection<Context> physicalDrives;
    private final Context masterDrive;
    private final Collection<Context> usedBy;
    private final Collection<Context> targets;
    private final Collection<Context> managedBy;

    private final Context context;

    private LogicalDriveDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        type = builder.type;
        mode = builder.mode;
        writeProtected = builder.writeProtected;
        assetTag = builder.assetTag;
        capacityGB = builder.capacityGB;
        image = builder.image;
        bootable = builder.bootable;
        snapshot = builder.snapshot;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
        logicalDrives = builder.logicalDrives;
        physicalDrives = builder.physicalDrives;
        masterDrive = builder.masterDrive;
        usedBy = builder.usedBy;
        targets = builder.targets;
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

    public VolumeType getType() {
        return type;
    }

    public VolumeMode getMode() {
        return mode;
    }

    public boolean isWriteProtected() {
        return writeProtected;
    }

    public String getAssetTag() {
        return assetTag;
    }

    public int getCapacityGB() {
        return capacityGB;
    }

    public String getImage() {
        return image;
    }

    public boolean isBootable() {
        return bootable;
    }

    public Boolean getSnapshot() {
        return snapshot;
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

    public Collection<Context> getLogicalDrives() {
        return logicalDrives;
    }

    public Collection<Context> getPhysicalDrives() {
        return physicalDrives;
    }

    public Context getMasterDrive() {
        return masterDrive;
    }

    public Collection<Context> getUsedBy() {
        return usedBy;
    }

    public Collection<Context> getTargets() {
        return targets;
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
        private VolumeType type;
        private VolumeMode mode;
        private boolean writeProtected;
        private String assetTag;
        private int capacityGB;
        private String image;
        private boolean bootable;
        private Boolean snapshot;
        private State state;
        private Health health;
        private Health healthRollup;
        private Collection<Context> logicalDrives;
        private Collection<Context> physicalDrives;
        private Context masterDrive;
        private Collection<Context> usedBy;
        private Collection<Context> targets;
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

        public Builder type(VolumeType type) {
            this.type = type;
            return this;
        }

        public Builder mode(VolumeMode mode) {
            this.mode = mode;
            return this;
        }

        public Builder writeProtected(boolean writeProtected) {
            this.writeProtected = writeProtected;
            return this;
        }

        public Builder assetTag(String assetTag) {
            this.assetTag = assetTag;
            return this;
        }

        public Builder capacityGB(int capacityGB) {
            this.capacityGB = capacityGB;
            return this;
        }

        public Builder image(String image) {
            this.image = image;
            return this;
        }

        public Builder bootable(boolean bootable) {
            this.bootable = bootable;
            return this;
        }

        public Builder snapshot(Boolean snapshot) {
            this.snapshot = snapshot;
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

        public Builder logicalDrives(Collection<Context> logicalDrives) {
            this.logicalDrives = logicalDrives;
            return this;
        }

        public Builder physicalDrives(Collection<Context> physicalDrives) {
            this.physicalDrives = physicalDrives;
            return this;
        }

        public Builder masterDrive(Context masterDrive) {
            this.masterDrive = masterDrive;
            return this;
        }

        public Builder usedBy(Collection<Context> usedBy) {
            this.usedBy = usedBy;
            return this;
        }

        public Builder targets(Collection<Context> targets) {
            this.targets = targets;
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

        public LogicalDriveDto build() {
            return new LogicalDriveDto(this);
        }
    }
}
