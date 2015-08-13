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

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.intel.rsa.common.types.BootSourceState;
import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.Power;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.actions.ResetActionJson;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.ArrayList;
import java.util.Collection;
import java.util.LinkedList;
import java.util.List;

@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
    "systemType", "manufacturer", "model", "serialNumber", "status", "enumStatus",
    "location", "oem", "power", "bios", "processors", "memory", "memoryDetails",
    "storageCapable", "storageControllersCount", "boot", "links", "actions"
})
public final class BladeJson extends BaseJson {
    public Id id;
    public String name;
    public LocationJson location;
    public SystemType systemType;
    public String manufacturer;
    public String model;
    public String serialNumber;
    public final StatusJson status = new StatusJson();
    public boolean storageCapable;
    public int storageControllersCount;
    public Power power;
    public Bios bios = new Bios();
    public Processors processors = new Processors();
    public Memory memory = new Memory();
    public Boot boot;
    public EnumStatus enumStatus;
    public final Links links = new Links();
    public final Oem oem = new Oem();
    public Actions actions = new Actions();

    public static final class Oem {
    }

    public BladeJson() {
        super("#RSABlade.1.0.0.RSABlade");
    }

    @JsonPropertyOrder({"processors", "memory", "Chassis", "storageControllers", "ethernetInterfaces", "managedBy", "oem"})
    public static final class Links extends BaseLinksJson {
        public ODataId processors;
        public ODataId memory;
        public ODataId storageControllers;
        public ODataId ethernetInterfaces;
        @JsonProperty("Chassis")
        public Collection<ODataId> containedBy = new ArrayList<>();
        public Collection<ODataId> managedBy = new ArrayList<>();
    }

    public static final class Bios {
        public Current current = new Current();
        public static final class Current {
            public String versionString;
        }
    }

    @JsonPropertyOrder({"count", "processorSockets", "processorFamily", "status"})
    public static final class Processors {
        public int count;
        public Integer processorSockets;
        public String processorFamily;
        public final StatusJson status = new StatusJson();
    }

    @JsonPropertyOrder({"totalSystemMemoryGB", "memorySockets", "status"})
    public static final class Memory {
        public int totalSystemMemoryGB;
        public Integer memorySockets;
        public final StatusJson status = new StatusJson();
    }

    @JsonPropertyOrder({"bootSourceOverrideEnabled", "bootSourceOverrideTarget", "bootSourceOverrideSupported", "uefiTargetBootSourceOverride"})
    public static class Boot {
        public BootSourceState bootSourceOverrideEnabled;
        public BootSourceType bootSourceOverrideTarget;
        public List<BootSourceType> bootSourceOverrideSupported  = new LinkedList<>();
        public String uefiTargetBootSourceOverride;
    }

    @JsonPropertyOrder({"resetAction"})
    public static final class Actions {
        @JsonProperty("#RSABlade.Reset")
        public ResetActionJson resetAction = new ResetActionJson();
    }
}
