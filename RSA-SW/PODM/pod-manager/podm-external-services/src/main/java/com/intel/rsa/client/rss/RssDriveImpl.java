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

package com.intel.rsa.client.rss;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.rss.RssDrive;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.StorageControllerInterface;

import static com.google.common.base.MoreObjects.toStringHelper;

@JsonIgnoreProperties(ignoreUnknown = true)
public class RssDriveImpl extends ResourceImpl implements RssDrive {

    @JsonProperty("Interface")
    private StorageControllerInterface controllerInterface;
    @JsonProperty("CapacityGB")
    private int capacityGb;
    @JsonProperty("Type")
    private DriveType type;
    @JsonProperty("RPM")
    private Integer rpm;
    @JsonProperty("Manufacturer")
    private String manufacturer;
    @JsonProperty("Model")
    private String model;
    @JsonProperty("SerialNumber")
    private String serialNumber;
    @JsonProperty("Status")
    private StatusDto status = new StatusDto();

    @Override
    public StorageControllerInterface getInterface() {
        return controllerInterface;
    }

    @Override
    public int getCapacityGb() {
        return capacityGb;
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

    @Override
    public String toString() {
        return toStringHelper(this)
                .add("capacityGb", capacityGb)
                .add("controllerInterface", controllerInterface)
                .add("type", type)
                .add("rpm", rpm)
                .add("manufacturer", manufacturer)
                .add("model", model)
                .add("serialNumber", serialNumber)
                .add("status", status)
                .toString();
    }
}
