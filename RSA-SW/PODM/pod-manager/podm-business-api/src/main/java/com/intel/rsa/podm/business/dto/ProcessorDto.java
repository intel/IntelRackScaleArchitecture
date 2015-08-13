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
import com.intel.rsa.common.types.InstructionSet;
import com.intel.rsa.common.types.InstructionSetExtension;
import com.intel.rsa.common.types.helpers.ConvertableEnumListHolder;
import com.intel.rsa.common.types.ProcessorModel;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.List;

public final class ProcessorDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final Location location;
    private final String socket;
    private final String manufacturer;
    private final ProcessorModel model;

    private final String processorType;
    private final String processorArchitecture;
    private final InstructionSet instructionSet;
    private final ConvertableEnumListHolder<InstructionSetExtension> instructionSetExtension;
    private final ProcessorIdDto processorId;

    private final Integer maxSpeedMHz;
    private final Integer totalCores;
    private final Integer enabledCores;
    private final Integer totalThreads;
    private final Integer enabledThreads;

    private final State state;
    private final Health health;
    private final Health healthRollup;
    private List<Id> managedBy;
    private Context context;

    private ProcessorDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        location = builder.location;
        socket = builder.socket;
        manufacturer = builder.manufacturer;
        model = builder.model;
        processorType = builder.processorType;
        processorArchitecture = builder.processorArchitecture;
        instructionSet = builder.instructionSet;
        instructionSetExtension = builder.instructionSetExtension;
        processorId = builder.processorId;
        maxSpeedMHz = builder.maxSpeedMHz;
        totalCores = builder.totalCores;
        enabledCores = builder.enabledCores;
        totalThreads = builder.totalThreads;
        enabledThreads = builder.enabledThreads;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
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

    public Location getLocation() {
        return location;
    }

    public String getSocket() {
        return socket;
    }

    public String getManufacturer() {
        return manufacturer;
    }

    public ProcessorModel getModel() {
        return model;
    }

    public String getProcessorType() {
        return processorType;
    }

    public String getProcessorArchitecture() {
        return processorArchitecture;
    }

    public InstructionSet getInstructionSet() {
        return instructionSet;
    }

    public ConvertableEnumListHolder<InstructionSetExtension> getInstructionSetExtension() {
        return instructionSetExtension;
    }

    public ProcessorIdDto getProcessorId() {
        return processorId;
    }

    public Integer getMaxSpeedMHz() {
        return maxSpeedMHz;
    }

    public Integer getTotalCores() {
        return totalCores;
    }

    public Integer getEnabledCores() {
        return enabledCores;
    }

    public Integer getTotalThreads() {
        return totalThreads;
    }

    public Integer getEnabledThreads() {
        return enabledThreads;
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

    public List<Id> getManagedBy() {
        return managedBy;
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
        private String socket;
        private String manufacturer;
        private ProcessorModel model;

        private String processorType;
        private String processorArchitecture;
        private InstructionSet instructionSet;
        private ConvertableEnumListHolder<InstructionSetExtension> instructionSetExtension;
        private ProcessorIdDto processorId;

        private Integer maxSpeedMHz;
        private Integer totalCores;
        private Integer enabledCores;
        private Integer totalThreads;
        private Integer enabledThreads;

        private State state;
        private Health health;
        private Health healthRollup;
        private List<Id> managedBy;
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

        public Builder socket(String socket) {
            this.socket = socket;
            return this;
        }

        public Builder manufacturer(String manufacturer) {
            this.manufacturer = manufacturer;
            return this;
        }

        public Builder model(ProcessorModel model) {
            this.model = model;
            return this;
        }

        public Builder processorType(String processorType) {
            this.processorType = processorType;
            return this;
        }

        public Builder processorArchitecture(String processorArchitecture) {
            this.processorArchitecture = processorArchitecture;
            return this;
        }

        public Builder instructionSet(InstructionSet instructionSet) {
            this.instructionSet = instructionSet;
            return this;
        }

        public Builder instructionSetExtensions(ConvertableEnumListHolder<InstructionSetExtension> instructionSetExtension) {
            this.instructionSetExtension = instructionSetExtension;
            return this;
        }

        public Builder processorId(ProcessorIdDto processorId) {
            this.processorId = processorId;
            return this;
        }

        public Builder maxSpeedMHz(Integer maxSpeedMHz) {
            this.maxSpeedMHz = maxSpeedMHz;
            return this;
        }

        public Builder totalCores(Integer totalCores) {
            this.totalCores = totalCores;
            return this;
        }

        public Builder enabledCores(Integer enabledCores) {
            this.enabledCores = enabledCores;
            return this;
        }

        public Builder totalThreads(Integer totalThreads) {
            this.totalThreads = totalThreads;
            return this;
        }

        public Builder enabledThreads(Integer enabledThreads) {
            this.enabledThreads = enabledThreads;
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

        public Builder managedBy(List<Id> managedBy) {
            this.managedBy = managedBy;
            return this;
        }

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public ProcessorDto build() {
            return new ProcessorDto(this);
        }
    }
}
