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

import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.fasterxml.jackson.annotation.JsonUnwrapped;
import com.intel.rsa.common.types.rmm.thermal.FanMeterState;
import com.intel.rsa.common.types.rmm.thermal.FanState;

/**
 * Class for representing thermal fan information in JSON
 */
@JsonPropertyOrder({
    "desiredSpeedRPM", "presentTachMeterReading", "healthState",
    "assetTag", "fruInfo", "meterState", "fanState"
})
public final class ThermalFanJson {
    public Integer desiredSpeedRPM;
    public Integer presentTachMeterReading;
    public Integer healthState;
    @JsonUnwrapped
    public AssetTagJson assetTag;
    public FruInfoJson fruInfo;
    public FanMeterState meterState;
    public FanState fanState;
}
