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
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.ArrayList;
import java.util.Collection;

@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
    "chassisType", "manufacturer", "model", "serialNumber", "status", "enumStatus",
    "location", "oem", "links"
})
@JsonInclude(JsonInclude.Include.NON_NULL)
public final class PodJson extends BaseJson {
    public Id id;
    public String name;
    public LocationJson location;
    public final String chassisType = "Pod";
    public final StatusJson status = new StatusJson();
    public EnumStatus enumStatus;
    public final Oem oem = new Oem();
    public final Links links = new Links();


    public PodJson() {
        super("#RSAPod.1.0.0.RSAPod");
    }

    @JsonPropertyOrder({ "racks", "managedBy", "oem" })
    public static final class Links extends BaseLinksJson {
        public ODataId racks;
        public Collection<ODataId> managedBy = new ArrayList<>();
    }

    public static final class Oem {
    }
}
