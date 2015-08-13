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

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.intel.rsa.common.types.ComposedNodeState;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.actions.AssembleActionJson;
import com.intel.rsa.podm.rest.representation.json.templates.actions.ResetActionJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.ArrayList;
import java.util.Collection;

@JsonPropertyOrder({
        "@odata.context", "@odata.id", "@odata.type",
        "id", "name", "description", "modified",
        "systemType", "status", "composedNodeState", "processors", "memory",
        "links", "actions"
})
public final class ComposedNodeJson extends BaseJson {

    public Id id;
    public String name;
    public String description;
    public SystemType systemType;
    public StatusJson status = new StatusJson();
    public ComposedNodeState composedNodeState;

    public Processors processors = new Processors();
    public Memory memory = new Memory();

    public Links links = new Links();
    public Actions actions = new Actions();

    public ComposedNodeJson() {
        super("#RSAComposedNode.1.0.0.RSAComposedNode");
    }

    @JsonPropertyOrder({"count", "processorSockets", "processorFamily", "status"})
    @JsonInclude(JsonInclude.Include.NON_NULL)
    public static final class Processors {
        public int count;
        public Integer processorSockets;
        public String processorFamily;
        public StatusJson status = new StatusJson();
    }

    @JsonPropertyOrder({"totalSystemMemoryGB", "memorySockets", "status"})
    @JsonInclude(JsonInclude.Include.NON_NULL)
    public static final class Memory {
        public int totalSystemMemoryGB;
        public Integer memorySockets;
        public StatusJson status = new StatusJson();
    }

    @JsonPropertyOrder({"processors", "memory", "remoteDrives", "localDrives", "ethernetInterfaces", "managedBy", "oem"})
    public static final class Links extends BaseLinksJson {
        public Collection<ODataId> processors = new ArrayList<>();
        public Collection<ODataId> memory = new ArrayList<>();
        public Collection<ODataId> remoteDrives = new ArrayList<>();
        public Collection<ODataId> localDrives = new ArrayList<>();
        public Collection<ODataId> ethernetInterfaces = new ArrayList<>();
        public Collection<ODataId> managedBy = new ArrayList<>();
    }

    @JsonPropertyOrder({"resetAction"})
    public static final class Actions {
        @JsonProperty("#RSAComposedNode.Reset")
        public ResetActionJson resetAction = new ResetActionJson();
        @JsonProperty("#RSAComposedNode.Assemble")
        public AssembleActionJson assembleAction = new AssembleActionJson();
    }
}
