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
import com.intel.rsa.client.api.reader.psme.PsmeBoot;
import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.common.types.BootSourceState;

import java.util.List;

import static java.util.Collections.emptyList;

public class PsmeBootImpl extends ResourceImpl implements PsmeBoot {
    @JsonProperty("BootSourceOverrideEnabled")
    private BootSourceState bootSourceOverrideEnabled;

    @JsonProperty("BootSourceOverrideTarget")
    private BootSourceType bootSourceOverrideTarget;

    @JsonProperty("BootSourceOverrideSupported")
    private List<BootSourceType> bootSourceOverrideSupported;

    @JsonProperty("UefiTargetBootSourceOverride")
    private String uefiTargetBootSourceOverride;

    @Override
    public BootSourceState getBootSourceOverrideEnabled() {
        return bootSourceOverrideEnabled;
    }

    @Override
    public BootSourceType getBootSourceOverrideTarget() {
        return bootSourceOverrideTarget;
    }

    @Override
    public List<BootSourceType> getBootSourceOverrideSupported() {
        if (bootSourceOverrideSupported == null) {
            return emptyList();
        }
        return bootSourceOverrideSupported;
    }

    @Override
    public String getUefiTargetBootSourceOverride() {
        return uefiTargetBootSourceOverride;
    }
}
