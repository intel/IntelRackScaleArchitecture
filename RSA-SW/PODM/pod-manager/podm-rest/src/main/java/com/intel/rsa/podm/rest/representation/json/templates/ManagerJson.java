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
import com.intel.rsa.common.types.ConnectionType;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.ManagerType;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.ArrayList;
import java.util.Collection;

/**
 * Template class for producing Json from Manager asset. According to PSME API spec 20150330 fixed.
 */
@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified"
})
public final class ManagerJson extends BaseJson {
    public Id id;
    public String name;
    public ManagerType managerType;
    public String model;
    public final StatusJson status = new StatusJson();
    public final Console graphicalConsole = new Console();
    public final Console serialConsole = new Console();
    public final Console commandShell = new Console();
    public final Firmware firmware = new Firmware();
    public final Links links = new Links();
    public final Actions actions = new Actions();

    public final Oem oem = new Oem();

    public static final class Oem {
    }

    public ManagerJson() {
        super("#RSAManager.1.0.0.RSAManager");
    }

    @JsonInclude(JsonInclude.Include.NON_NULL)
    public static final class Console {
        public boolean enabled;
        public Integer maxConcurrentSessions;
        public ConnectionType connectTypesSupported;
    }

    public static final class Firmware {
        public Current current = new Current();
        public static final class Current {
            public String versionString;
        }
    }

    @JsonPropertyOrder({
        "managerForDrawers", "managerForComputeModules", "managerForFabricModules",
        "managerForBlades", "managerForSwitches", "networkService", "simpleNetwork", "oem"
    })
    public static final class Links extends BaseLinksJson {
        public ODataId networkService;
        public ODataId simpleNetwork;

        @JsonProperty("ManagerForChassis")
        public Collection<ODataId> managerForDrawers = new ArrayList<>();
        public Collection<ODataId> managerForComputeModules = new ArrayList<>();
        public Collection<ODataId> managerForFabricModules = new ArrayList<>();
        public Collection<ODataId> managerForBlades = new ArrayList<>();
        public Collection<ODataId> managerForSwitches = new ArrayList<>();
    }

    public static final class Actions {

    }
}
