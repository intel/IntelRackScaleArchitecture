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
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.ArrayList;
import java.util.Collection;

@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
    "status", "location", "oem", "links", "actions"
})
public final class FabricModuleJson extends BaseJson {

    public Id id;
    public String name;
    public final StatusJson status = new StatusJson();
    public LocationJson location;
    public final Oem oem = new Oem();
    public final Links links = new Links();
    public final Actions actions = new Actions();

    public FabricModuleJson() {
        super("#RSAFabricModule.1.0.0.RSAFabricModule");
    }

    @JsonPropertyOrder({"switches", "containedBy", "managedBy", "oem"})
    public static final class Links extends BaseLinksJson {
        public ODataId switches;
        public ODataId containedBy;
        public Collection<ODataId> managedBy = new ArrayList<>();
    }

    public static final class Oem {
    }

    public static final class Actions {
    }
}
