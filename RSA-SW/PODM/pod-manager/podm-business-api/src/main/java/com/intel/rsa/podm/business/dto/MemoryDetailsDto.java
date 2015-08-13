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
import com.intel.rsa.common.types.State;

import java.math.BigDecimal;

public final class MemoryDetailsDto {
    private final String socket;
    private final String type;
    private final int sizeGB;
    private final int speedMHz;
    private final BigDecimal voltageVolt;
    private final State state;
    private final Health health;
    private final Health healthRollup;

    private MemoryDetailsDto(Builder builder) {
        socket = builder.socket;
        type = builder.type;
        sizeGB = builder.sizeGB;
        speedMHz = builder.speedMHz;
        voltageVolt = builder.voltageVolt;
        state = builder.state;
        health = builder.health;
        healthRollup = builder.healthRollup;
    }

    public String getSocket() {
        return socket;
    }

    public String getType() {
        return type;
    }

    public int getSizeGB() {
        return sizeGB;
    }

    public int getSpeedMHz() {
        return speedMHz;
    }

    public BigDecimal getVoltageVolt() {
        return voltageVolt;
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

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private String socket;
        private String type;
        private int sizeGB;
        private int speedMHz;
        private BigDecimal voltageVolt;
        private State state;
        private Health health;
        private Health healthRollup;

        private Builder() {
        }

        public Builder socket(String socket) {
            this.socket = socket;
            return this;
        }

        public Builder type(String type) {
            this.type = type;
            return this;
        }

        public Builder sizeGB(int sizeGB) {
            this.sizeGB = sizeGB;
            return this;
        }

        public Builder speedMHz(int speedMHz) {
            this.speedMHz = speedMHz;
            return this;
        }

        public Builder voltageVolt(BigDecimal voltageVolt) {
            this.voltageVolt = voltageVolt;
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

        public MemoryDetailsDto build() {
            return new MemoryDetailsDto(this);
        }
    }
}
