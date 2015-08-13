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
import com.intel.rsa.common.types.FormFactor;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.MemoryType;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.math.BigDecimal;

@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
    "manufacturer", "socket", "bank", "type", "sizeGB", "speedMHz", "voltageVolt",
    "dataWidthBits", "totalWidthBits", "formFactor", "serialNumber", "assetTag",
    "partNumber", "rank", "configuredSpeedMHz", "minimumVoltageVolt", "maximumVoltageVolt", "status",
    "location", "oem", "links"
})
public final class MemoryJson extends BaseJson {

    public Id id;
    public String name;
    public String manufacturer;
    public String socket;
    public String bank;
    public MemoryType type;
    public Integer sizeGB;
    public Integer speedMHz;
    public BigDecimal voltageVolt;
    public Integer dataWidthBits;
    public Integer totalWidthBits;
    public FormFactor formFactor;
    public String serialNumber;
    public String assetTag;
    public String partNumber;
    public String rank;
    public Integer configuredSpeedMHz;
    public BigDecimal minimumVoltageVolt;
    public BigDecimal maximumVoltageVolt;

    public final StatusJson status = new StatusJson();
    public LocationJson location;

    public final Links links = new Links();
    public final Oem oem = new Oem();

    public static final class Oem {
    }

    @JsonPropertyOrder({"containedBy", "oem"})
    public static final class Links extends BaseLinksJson {
        public ODataId containedBy;
    }

    public MemoryJson() {
        super("#RSAMemory.1.0.0.RSAMemory");
    }

}
