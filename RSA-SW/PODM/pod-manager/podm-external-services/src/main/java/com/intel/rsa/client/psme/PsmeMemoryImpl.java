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

package com.intel.rsa.client.psme;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.psme.PsmeMemory;
import com.intel.rsa.client.dto.LocationDto;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.FormFactor;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.MemoryType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;

import java.math.BigDecimal;

import static com.intel.rsa.common.types.locations.Locations.bladeLocation;

public class PsmeMemoryImpl extends ResourceImpl implements PsmeMemory  {

    @JsonProperty("Manufacturer")
    private String manufacturer;

    @JsonProperty("Socket")
    private String socket;

    @JsonProperty("Bank")
    private String bank;

    @JsonProperty("Type")
    private MemoryType type;

    @JsonProperty("SizeGB")
    private int sizeGB;

    @JsonProperty("SpeedMHz")
    private int speedMhz;

    @JsonProperty("VoltageVolt")
    private BigDecimal voltageVolt;

    @JsonProperty("DataWidthBits")
    private Integer dataWidthBits;

    @JsonProperty("TotalWidthBits")
    private Integer totalWidthBits;

    @JsonProperty("FormFactor")
    private FormFactor formFactor;

    @JsonProperty("SerialNumber")
    private String serialNumber;

    @JsonProperty("AssetTag")
    private String assetTag;

    @JsonProperty("PartNumber")
    private String partNumber;

    @JsonProperty("Rank")
    private String rank;

    @JsonProperty("ConfiguredSpeedMHz")
    private Integer configuredSpeedMHz;

    @JsonProperty("MinimumVoltageVolt")
    private BigDecimal minimumVoltageVolt;

    @JsonProperty("MaximumVoltageVolt")
    private BigDecimal maximumVoltageVolt;

    @JsonProperty("Status")
    private StatusDto status;

    @JsonProperty("Location")
    private LocationDto location;

    @Override
    public String getManufacturer() {
        return manufacturer;
    }

    @Override
    public String getSocket() {
        return socket;
    }

    @Override
    public String getBank() {
        return bank;
    }

    @Override
    public MemoryType getType() {
        return type;
    }

    @Override
    public int getSizeGB() {
        return sizeGB;
    }

    @Override
    public Integer getSpeedMHz() {
        return speedMhz;
    }

    @Override
    public BigDecimal getVoltageVolt() {
        return voltageVolt;
    }

    @Override
    public Integer getDataWidthBits() {
        return dataWidthBits;
    }

    @Override
    public Integer getTotalWidthBits() {
        return totalWidthBits;
    }

    @Override
    public FormFactor getFormFactor() {
        return formFactor;
    }

    @Override
    public String getSerialNumber() {
        return serialNumber;
    }

    @Override
    public String getAssetTag() {
        return assetTag;
    }

    @Override
    public String getPartNumber() {
        return partNumber;
    }

    @Override
    public String getRank() {
        return rank;
    }

    @Override
    public Integer getConfiguredSpeedMHz() {
        return configuredSpeedMHz;
    }

    @Override
    public BigDecimal getMinimumVoltageVolt() {
        return minimumVoltageVolt;
    }

    @Override
    public BigDecimal getMaximumVoltageVolt() {
        return maximumVoltageVolt;
    }

    @Override
    public State getState() {
        return status.getState();
    }

    @Override
    public Health getHealth() {
        return status.getHealth();
    }

    @Override
    public Location getLocation() {
        return bladeLocation(
                location.getPod(),
                location.getRack(),
                location.getDrawer(),
                location.getModule(),
                location.getBlade()
        );
    }
}
