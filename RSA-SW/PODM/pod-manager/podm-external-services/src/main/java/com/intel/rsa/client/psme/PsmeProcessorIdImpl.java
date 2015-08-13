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

package com.intel.rsa.client.psme;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.psme.PsmeProcessorId;

public class PsmeProcessorIdImpl extends ResourceImpl implements PsmeProcessorId  {
    @JsonProperty("VendorId")
    private String vendorId;

    @JsonProperty("NumericId")
    private Integer numericId;

    @JsonProperty("Family")
    private Integer family;

    @JsonProperty("Model")
    private Integer model;

    @JsonProperty("Step")
    private Integer step;

    @JsonProperty("MicrocodeInfo")
    private Long microcodeInfo;

    @Override
    public String getVendorId() {
        return vendorId;
    }

    @Override
    public Integer getNumericId() {
        return numericId;
    }

    @Override
    public Integer getFamily() {
        return family;
    }

    @Override
    public Integer getModel() {
        return model;
    }

    @Override
    public Integer getStep() {
        return step;
    }

    @Override
    public Long getMicrocodeInfo() {
        return microcodeInfo;
    }
}
