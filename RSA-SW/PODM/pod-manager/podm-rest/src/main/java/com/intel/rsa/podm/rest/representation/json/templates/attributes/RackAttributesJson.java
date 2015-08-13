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

package com.intel.rsa.podm.rest.representation.json.templates.attributes;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.fasterxml.jackson.annotation.JsonUnwrapped;

import java.util.Collection;
import java.util.UUID;


@JsonPropertyOrder({
    "RMMPresent", "rackSupportsDisaggregatedPowerCooling", "RackUUID",
    "assetTag", "geoTag", "trayPresent", "FRUInfo"
})
@JsonInclude(JsonInclude.Include.NON_NULL)
public final class RackAttributesJson {
    @JsonProperty("RMMPresent")
    public boolean isRmmPresent;
    public Boolean rackSupportsDisaggregatedPowerCooling;

    @JsonProperty("RackUUID")
    public UUID rackUuid;

    @JsonUnwrapped
    public AssetTagJson assetTag;
    public String geoTag;
    public final TrayPresentJson trayPresent = new TrayPresentJson();

    @JsonProperty("FRUInfo")
    public FruInfoJson fruInfo;

    public Collection<ThermalZoneJson> thermalZones;
    public Collection<PowerZoneJson> powerZones;
}
