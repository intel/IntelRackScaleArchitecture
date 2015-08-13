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
import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.StorageControllerInterface;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
    "controllerInterface", "capacityGB", "type", "rpm", "manufacturer",
    "model", "location", "serialNumber", "status", "oem", "links"
})
public final class DriveJson extends BaseJson {
    public Id id;
    public String name;
    @JsonProperty("Interface")
    public StorageControllerInterface controllerInterface;
    public int capacityGB;
    public DriveType type;
    @JsonProperty("RPM")
    public Integer rpm;
    public String manufacturer;
    public String model;
    public String serialNumber;
    public LocationJson location;
    public final StatusJson status = new StatusJson();
    public final Links links = new Links();
    public final Oem oem = new Oem();


    public static final class Oem {
    }

    public DriveJson() {
        super("#Drive.1.0.0.Drive");
    }

    public static final class Links extends BaseLinksJson {
        public ODataId containedBy;
    }
}
