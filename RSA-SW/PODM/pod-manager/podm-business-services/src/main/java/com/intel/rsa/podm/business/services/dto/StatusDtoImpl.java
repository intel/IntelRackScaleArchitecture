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

package com.intel.rsa.podm.business.services.dto;

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.dto.ComposedNodeDto;

public class StatusDtoImpl implements ComposedNodeDto.StatusDto {

    private State state;
    private Health health;
    private Health healthRollup;

    public StatusDtoImpl(State state, Health health, Health healthRollup) {
        this.state = state;
        this.health = health;
        this.healthRollup = healthRollup;
    }

    @Override
    public State getState() {
        return state;
    }

    public void setState(State state) {
        this.state = state;
    }

    @Override
    public Health getHealth() {
        return health;
    }

    public void setHealth(Health health) {
        this.health = health;
    }

    @Override
    public Health getHealthRollup() {
        return healthRollup;
    }

    public void setHealthRollup(Health healthRollup) {
        this.healthRollup = healthRollup;
    }
}
