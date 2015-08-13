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
import com.intel.rsa.client.dto.LocationDto;
import com.intel.rsa.client.api.reader.psme.PsmeDrive;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.StorageControllerInterface;
import com.intel.rsa.common.types.locations.Location;

import static com.intel.rsa.common.types.locations.Locations.bladeLocation;

public class PsmeDriveImpl extends ResourceImpl implements PsmeDrive  {
    @JsonProperty ("Interface")
    private StorageControllerInterface controllerInterface;
    @JsonProperty ("CapacityGB")
    private int capacityGB;
    @JsonProperty ("Type")
    private DriveType type;
    @JsonProperty("RPM")
    private Integer rpm;
    @JsonProperty ("Manufacturer")
    private String manufacturer;
    @JsonProperty ("Model")
    private String model;
    @JsonProperty ("SerialNumber")
    private String serialNumber;
    @JsonProperty("Location")
    private LocationDto location;
    @JsonProperty ("Status")
    private StatusDto status;

    @Override
    public StorageControllerInterface getControllerInterface() {
        return controllerInterface;
    }

    @Override
    public int getCapacityGB() {
        return capacityGB;
    }

    @Override
    public DriveType getType() {
        return type;
    }

    @Override
    public Integer getRpm() {
        return rpm;
    }

    @Override
    public String getManufacturer() {
        return manufacturer;
    }

    @Override
    public String getModel() {
        return model;
    }

    @Override
    public String getSerialNumber() {
        return serialNumber;
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

    @Override
    public State getState() {
        return status.getState();
    }

    @Override
    public Health getHealth() {
        return status.getHealth();
    }

    @Override
    public Health getHealthRollup() {
        return status.getHealthRollup();
    }
}
