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

import com.intel.rsa.common.types.FormFactor;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.MemoryType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.services.context.Context;

import java.math.BigDecimal;
import java.time.OffsetDateTime;

/**
 * Class transferring Memory data.
 * Fields compliant with PSME API 20150427_fix1.
 */
public final class MemoryDto {
    private final Id id;
    private final String name;
    private final Context context;
    private final OffsetDateTime modified;
    private final String manufacturer;
    private final String socket;
    private final String bank;
    private final MemoryType type;
    private final Integer sizeGB;
    private final Integer speedMHz;
    private final BigDecimal voltageVolt;
    private final Integer dataWidthBits;
    private final Integer totalWidthBits;
    private final FormFactor formFactor;
    private final String serialNumber;
    private final String assetTag;
    private final String partNumber;
    private final String rank;
    private final Integer configuredSpeedMHz;
    private final BigDecimal minimumVoltageVolt;
    private final BigDecimal maximumVoltageVolt;
    private final State state;
    private final Health health;
    private final Location location;
    private final Context containedBy;

    private MemoryDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        context = builder.context;
        modified = builder.modified;
        manufacturer = builder.manufacturer;
        socket = builder.socket;
        bank = builder.bank;
        type = builder.type;
        sizeGB = builder.sizeGB;
        speedMHz = builder.speedMHz;
        voltageVolt = builder.voltageVolt;
        dataWidthBits = builder.dataWidthBits;
        totalWidthBits = builder.totalWidthBits;
        formFactor = builder.formFactor;
        serialNumber = builder.serialNumber;
        assetTag = builder.assetTag;
        partNumber = builder.partNumber;
        rank = builder.rank;
        configuredSpeedMHz = builder.configuredSpeedMHz;
        minimumVoltageVolt = builder.minimumVoltageVolt;
        maximumVoltageVolt = builder.maximumVoltageVolt;
        state = builder.state;
        health = builder.health;
        location = builder.location;
        containedBy = builder.containedBy;
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

    public Context getContext() {
        return context;
    }

    public OffsetDateTime getModified() {
        return modified;
    }

    public String getManufacturer() {
        return manufacturer;
    }

    public String getSocket() {
        return socket;
    }

    public String getBank() {
        return bank;
    }

    public MemoryType getType() {
        return type;
    }

    public Integer getSizeGB() {
        return sizeGB;
    }

    public Integer getSpeedMHz() {
        return speedMHz;
    }

    public BigDecimal getVoltageVolt() {
        return voltageVolt;
    }

    public Integer getDataWidthBits() {
        return dataWidthBits;
    }

    public Integer getTotalWidthBits() {
        return totalWidthBits;
    }

    public FormFactor getFormFactor() {
        return formFactor;
    }

    public String getSerialNumber() {
        return serialNumber;
    }

    public String getAssetTag() {
        return assetTag;
    }

    public String getPartNumber() {
        return partNumber;
    }

    public String getRank() {
        return rank;
    }

    public Integer getConfiguredSpeedMHz() {
        return configuredSpeedMHz;
    }

    public BigDecimal getMinimumVoltageVolt() {
        return minimumVoltageVolt;
    }

    public BigDecimal getMaximumVoltageVolt() {
        return maximumVoltageVolt;
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

    public Context getContainedBy() {
        return containedBy;
    }

    public static final class Builder {
        private Id id;
        private String name;
        private Context context;
        private OffsetDateTime modified;
        private String manufacturer;
        private String socket;
        private String bank;
        private MemoryType type;
        private Integer sizeGB;
        private Integer speedMHz;
        private BigDecimal voltageVolt;
        private Integer dataWidthBits;
        private Integer totalWidthBits;
        private FormFactor formFactor;
        private String serialNumber;
        private String assetTag;
        private String partNumber;
        private String rank;
        private Integer configuredSpeedMHz;
        private BigDecimal minimumVoltageVolt;
        private BigDecimal maximumVoltageVolt;
        private State state;
        private Health health;
        private Location location;
        private Context containedBy;

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

        public Builder context(Context context) {
            this.context = context;
            return this;
        }

        public Builder modified(OffsetDateTime modified) {
            this.modified = modified;
            return this;
        }

        public Builder manufacturer(String manufacturer) {
            this.manufacturer = manufacturer;
            return this;
        }

        public Builder socket(String socket) {
            this.socket = socket;
            return this;
        }

        public Builder bank(String bank) {
            this.bank = bank;
            return this;
        }

        public Builder type(MemoryType type) {
            this.type = type;
            return this;
        }

        public Builder sizeGB(Integer sizeGB) {
            this.sizeGB = sizeGB;
            return this;
        }

        public Builder speedMHz(Integer speedMHz) {
            this.speedMHz = speedMHz;
            return this;
        }

        public Builder voltageVolt(BigDecimal voltageVolt) {
            this.voltageVolt = voltageVolt;
            return this;
        }

        public Builder dataWidthBits(Integer dataWidthBits) {
            this.dataWidthBits = dataWidthBits;
            return this;
        }

        public Builder totalWidthBits(Integer totalWidthBits) {
            this.totalWidthBits = totalWidthBits;
            return this;
        }

        public Builder formFactor(FormFactor formFactor) {
            this.formFactor = formFactor;
            return this;
        }

        public Builder serialNumber(String serialNumber) {
            this.serialNumber = serialNumber;
            return this;
        }

        public Builder assetTag(String assetTag) {
            this.assetTag = assetTag;
            return this;
        }

        public Builder partNumber(String partNumber) {
            this.partNumber = partNumber;
            return this;
        }

        public Builder rank(String rank) {
            this.rank = rank;
            return this;
        }

        public Builder configuredSpeedMHz(Integer configuredSpeedMHz) {
            this.configuredSpeedMHz = configuredSpeedMHz;
            return this;
        }

        public Builder minimumVoltageVolt(BigDecimal minimumVoltageVolt) {
            this.minimumVoltageVolt = minimumVoltageVolt;
            return this;
        }

        public Builder maximumVoltageVolt(BigDecimal maximumVoltageVolt) {
            this.maximumVoltageVolt = maximumVoltageVolt;
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

        public Builder containedBy(Context containedBy) {
            this.containedBy = containedBy;
            return this;
        }

        public MemoryDto build() {
            return new MemoryDto(this);
        }
    }
}
