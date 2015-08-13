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

package com.intel.rsa.client.psme.dto;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.dto.ODataId;

/**
 * Dto for Blade's resources links, provided by PSME.
 */
@JsonIgnoreProperties(ignoreUnknown = true)
public class BladeLinksDto extends ManagedResourceLinksDto {
    @JsonProperty("Processors")
    private ODataId processors;

    @JsonProperty("Memory")
    private ODataId memory;

    @JsonProperty("Chassis")
    private Iterable<ODataId> chassis;

    @JsonProperty("StorageControllers")
    private ODataId storageControllers;

    @JsonProperty("EthernetInterfaces")
    private ODataId ethernetInterfaces;

    public ODataId getProcessors() {
        return processors;
    }

    public ODataId getMemory() {
        return memory;
    }

    public Iterable<ODataId> getChassis() {
        return chassis;
    }

    public ODataId getStorageControllers() {
        return storageControllers;
    }

    public ODataId getEthernetInterfaces() {
        return ethernetInterfaces;
    }
}
