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
import com.intel.rsa.common.types.ManagerType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.Collections;
import java.util.List;

import static com.intel.rsa.podm.business.services.context.ContextType.MANAGER;

/**
 * Class transferring Manager data. Fields compliant with PSME API 20150330 fixed.
 */
public final class ManagerDto {
    private final Id id;
    private final String name;
    private final OffsetDateTime modified;
    private final ManagerType managerType;
    private final String model;
    private final State state;
    private final Health health;
    private final Health healthRollup;
    private final ConsoleDto graphicalConsole;
    private final ConsoleDto serialConsole;
    private final ConsoleDto commandShell;
    private final String currentFirmwareVersion;

    private List<Context> managerForDrawers;
    private List<Context> managerForComputeModules;
    private List<Context> managerForFabricModules;
    private List<Context> managerForBlades;
    private List<Context> managerForSwitches;

    private ManagerDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        modified = builder.modified;
        managerType = builder.managerType;
        model = builder.model;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
        graphicalConsole = builder.graphicalConsole;
        serialConsole = builder.serialConsole;
        commandShell = builder.commandShell;
        currentFirmwareVersion = builder.currentFirmwareVersion;
        managerForDrawers = Collections.unmodifiableList(builder.managerForDrawers);
        managerForComputeModules = Collections.unmodifiableList(builder.managerForComputeModules);
        managerForFabricModules = Collections.unmodifiableList(builder.managerForFabricModules);
        managerForBlades = Collections.unmodifiableList(builder.managerForBlades);
        managerForSwitches = Collections.unmodifiableList(builder.managerForSwitches);
    }

    public Context getContext() {
        return Context.contextOf(id, MANAGER);
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

    public ManagerType getManagerType() {
        return managerType;
    }

    public String getModel() {
        return model;
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

    public ConsoleDto getGraphicalConsole() {
        return graphicalConsole;
    }

    public ConsoleDto getSerialConsole() {
        return serialConsole;
    }

    public ConsoleDto getCommandShell() {
        return commandShell;
    }

    public String getCurrentFirmwareVersion() {
        return currentFirmwareVersion;
    }

    public List<Context> getManagerForDrawers() {
        return managerForDrawers;
    }

    public List<Context> getManagerForComputeModules() {
        return managerForComputeModules;
    }

    public List<Context> getManagerForFabricModules() {
        return managerForFabricModules;
    }

    public List<Context> getManagerForBlades() {
        return managerForBlades;
    }

    public List<Context> getManagerForSwitches() {
        return managerForSwitches;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private Id id;
        private String name;
        private OffsetDateTime modified;
        private ManagerType managerType;
        private String model;
        private State state;
        private Health health;
        private Health healthRollup;
        private ConsoleDto graphicalConsole;
        private ConsoleDto serialConsole;
        private ConsoleDto commandShell;
        private String currentFirmwareVersion;
        private List<Context> managerForDrawers;
        private List<Context> managerForComputeModules;
        private List<Context> managerForFabricModules;
        private List<Context> managerForBlades;
        private List<Context> managerForSwitches;

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

        public Builder managerType(ManagerType managerType) {
            this.managerType = managerType;
            return this;
        }

        public Builder model(String model) {
            this.model = model;
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

        public Builder graphicalConsole(ConsoleDto graphicalConsole) {
            this.graphicalConsole = graphicalConsole;
            return this;
        }

        public Builder serialConsole(ConsoleDto serialConsole) {
            this.serialConsole = serialConsole;
            return this;
        }

        public Builder commandShell(ConsoleDto commandShell) {
            this.commandShell = commandShell;
            return this;
        }

        public Builder currentFirmwareVersion(String currentFirmwareVersion) {
            this.currentFirmwareVersion = currentFirmwareVersion;
            return this;
        }

        public Builder managerForDrawers(List<Context> managerForDrawers) {
            this.managerForDrawers = managerForDrawers;
            return this;
        }

        public Builder managerForComputeModules(List<Context> managerForComputeModules) {
            this.managerForComputeModules = managerForComputeModules;
            return this;
        }

        public Builder managerForFabricModules(List<Context> managerForFabricModules) {
            this.managerForFabricModules = managerForFabricModules;
            return this;
        }

        public Builder managerForBlades(List<Context> managerForBlades) {
            this.managerForBlades = managerForBlades;
            return this;
        }

        public Builder managerForSwitches(List<Context> managerForSwitches) {
            this.managerForSwitches = managerForSwitches;
            return this;
        }

        public ManagerDto build() {
            return new ManagerDto(this);
        }
    }
}
