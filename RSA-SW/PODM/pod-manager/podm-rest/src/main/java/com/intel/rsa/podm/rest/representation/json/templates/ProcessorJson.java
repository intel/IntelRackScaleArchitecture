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

package com.intel.rsa.podm.rest.representation.json.templates;

import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.ProcessorModel;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.LinkedList;
import java.util.List;


@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
    "socket", "model", "manufacturer", "processorType", "processorArchitecture",
    "instructionSet", "instructionSetExtensions", "processorId", "maxSpeedMHz", "totalCores",
    "enabledCores", "totalThreads", "enabledThreads", "status", "location", "oem", "links"
})
public final class ProcessorJson extends BaseJson {
    public Id id;
    public String name;
    public LocationJson location;
    public String manufacturer;
    public ProcessorModel model;
    public String socket;

    public String processorType;
    public String processorArchitecture;
    public String instructionSet;
    public final List<String> instructionSetExtensions = new LinkedList<>();
    public ProcessorId processorId;

    public Integer maxSpeedMHz;
    public Integer totalCores;
    public Integer enabledCores;
    public Integer totalThreads;
    public Integer enabledThreads;

    public final StatusJson status = new StatusJson();
    public final Links links = new Links();
    public final Oem oem = new Oem();

    public static final class Oem {
    }

    public ProcessorJson() {
        super("#RSAProcessor.1.0.0.RSAProcessor");
    }

    @JsonPropertyOrder({"vendorId", "numericId", "family", "model", "step", "microcodeInfo"})
    public static class ProcessorId {
        public String vendorId;
        public Integer numericId;
        public Integer family;
        public Integer model;
        public Integer step;
        public Long microcodeInfo;
    }

    @JsonPropertyOrder({"containedBy", "managedBy", "oem"})
    public static final class Links extends BaseLinksJson {
        public ODataId containedBy;
    }
}

