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

import com.intel.rsa.common.types.BootSourceState;
import com.intel.rsa.common.types.BootSourceType;

import java.util.LinkedList;
import java.util.List;

public final class BootDto {
    private BootSourceState bootSourceOverrideEnabled;
    private BootSourceType bootSourceOverrideTarget;
    private List<BootSourceType> bootSourceOverrideSupported;
    private String uefiTargetBootSourceOverride;

    private BootDto(Builder builder) {
        bootSourceOverrideEnabled = builder.bootSourceOverrideEnabled;
        bootSourceOverrideTarget = builder.bootSourceOverrideTarget;
        bootSourceOverrideSupported = builder.bootSourceOverrideSupported;
        uefiTargetBootSourceOverride = builder.uefiTargetBootSourceOverride;
    }

    public BootSourceState getBootSourceOverrideEnabled() {
        return bootSourceOverrideEnabled;
    }

    public BootSourceType getBootSourceOverrideTarget() {
        return bootSourceOverrideTarget;
    }

    public List<BootSourceType> getBootSourceOverrideSupported() {
        return bootSourceOverrideSupported;
    }

    public String getUefiTargetBootSourceOverride() {
        return uefiTargetBootSourceOverride;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private BootSourceState bootSourceOverrideEnabled;
        private BootSourceType bootSourceOverrideTarget;
        private List<BootSourceType> bootSourceOverrideSupported = new LinkedList<>();
        private String uefiTargetBootSourceOverride;

        private Builder() {
        }

        public Builder bootSourceOverrideEnabled(BootSourceState bootSourceOverrideEnabled) {
            this.bootSourceOverrideEnabled = bootSourceOverrideEnabled;
            return this;
        }

        public Builder bootSourceOverrideTarget(BootSourceType bootSourceOverrideTarget) {
            this.bootSourceOverrideTarget = bootSourceOverrideTarget;
            return this;
        }

        public Builder bootSourceOverrideSupported(List<BootSourceType> bootSourceOverrideSupported) {
            if (bootSourceOverrideSupported != null) {
                this.bootSourceOverrideSupported = bootSourceOverrideSupported;
            }
            return this;
        }

        public Builder uefiTargetBootSourceOverride(String uefiTargetBootSourceOverride) {
            this.uefiTargetBootSourceOverride = uefiTargetBootSourceOverride;
            return this;
        }

        public BootDto build() {
            return new BootDto(this);
        }
    }
}
