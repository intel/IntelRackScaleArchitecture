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

package com.intel.rsa.client.psme.actions;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.api.actions.psme.BootSourceOverrideRequest;
import com.intel.rsa.common.types.BootSourceState;
import com.intel.rsa.common.types.BootSourceType;

public class BootSourceOverrideRequestImpl implements BootSourceOverrideRequest {

    @JsonProperty("Boot")
    private BootSourceOverride bootSourceOverride;

    public BootSourceOverrideRequestImpl(BootSourceType bootSourceType, BootSourceState bootSourceState) {
        this.bootSourceOverride = new BootSourceOverrideImpl(bootSourceType, bootSourceState);
    }

    @Override
    public BootSourceOverride getBootSourceOverride() {
        return bootSourceOverride;
    }

    private class BootSourceOverrideImpl implements BootSourceOverride {

        @JsonProperty("BootSourceOverrideEnabled")
        private BootSourceState bootSourceState;

        @JsonProperty("BootSourceOverrideTarget")
        private BootSourceType bootSourceType;

        public BootSourceOverrideImpl(BootSourceType bootSourceType, BootSourceState bootSourceState) {
            this.bootSourceState = bootSourceState;
            this.bootSourceType = bootSourceType;
        }

        @Override
        public BootSourceState getBootSourceState() {
            return bootSourceState;
        }

        @Override
        public BootSourceType getBootSourceType() {
            return bootSourceType;
        }
    }
}
