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

public final class ProcessorIdDto {
    private final String vendorId;
    private final Integer numericId;
    private final Integer family;
    private final Integer model;
    private final Integer step;
    private final Long microcodeInfo;

    private ProcessorIdDto(Builder builder) {
        vendorId = builder.vendorId;
        numericId = builder.numericId;
        family = builder.family;
        model = builder.model;
        step = builder.step;
        microcodeInfo = builder.microcodeInfo;
    }

    public String getVendorId() {
        return vendorId;
    }

    public Integer getNumericId() {
        return numericId;
    }

    public Integer getFamily() {
        return family;
    }

    public Integer getModel() {
        return model;
    }

    public Integer getStep() {
        return step;
    }

    public Long getMicrocodeInfo() {
        return microcodeInfo;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public static final class Builder {
        private String vendorId;
        private Integer numericId;
        private Integer family;
        private Integer model;
        private Integer step;
        private Long microcodeInfo;

        private Builder() {
        }

        public Builder vendorId(String vendorId) {
            this.vendorId = vendorId;
            return this;
        }

        public Builder numericId(Integer numericId) {
            this.numericId = numericId;
            return this;
        }

        public Builder family(Integer family) {
            this.family = family;
            return this;
        }

        public Builder model(Integer model) {
            this.model = model;
            return this;
        }

        public Builder step(Integer step) {
            this.step = step;
            return this;
        }

        public Builder microcodeInfo(Long microcodeInfo) {
            this.microcodeInfo = microcodeInfo;
            return this;
        }

        public ProcessorIdDto build() {
            return new ProcessorIdDto(this);
        }
    }
}
