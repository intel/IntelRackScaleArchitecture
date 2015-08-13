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

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.psme.PsmeProcessor;
import com.intel.rsa.client.api.reader.psme.PsmeProcessorId;
import com.intel.rsa.client.dto.LocationDto;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.InstructionSet;
import com.intel.rsa.common.types.InstructionSetExtension;
import com.intel.rsa.common.types.helpers.ConvertableEnumListHolder;
import com.intel.rsa.common.types.ProcessorModel;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;

import java.util.List;

import static com.intel.rsa.common.types.locations.Locations.bladeLocation;

@JsonIgnoreProperties(ignoreUnknown = true)
public class PsmeProcessorImpl extends ResourceImpl implements PsmeProcessor  {
    @JsonProperty("Socket")
    private String socket;

    @JsonProperty("Model")
    private ProcessorModel model;

    @JsonProperty("Manufacturer")
    private String manufacturer;

    @JsonProperty("ProcessorType")
    private String processorType;

    @JsonProperty("ProcessorArchitecture")
    private String processorArchitecture;

    @JsonProperty("InstructionSet")
    private InstructionSet instructionSet;

    @JsonProperty("InstructionSetExtensions")
    private List<InstructionSetExtension> instructionSetExtensionList;

    @JsonProperty("ProcessorId")
    private PsmeProcessorIdImpl processorId;

    @JsonProperty("MaxSpeedMHz")
    private Integer maxSpeedMHz;

    @JsonProperty("TotalCores")
    private Integer totalCores;

    @JsonProperty("EnabledCores")
    private Integer enabledCores;

    @JsonProperty("TotalThreads")
    private Integer totalThreads;

    @JsonProperty("EnabledThreads")
    private Integer enabledThreads;

    @JsonProperty("Status")
    private StatusDto statusDto;

    @JsonProperty("Location")
    private LocationDto location;

    @Override
    public ProcessorModel getModel() {
        return model;
    }

    @Override
    public String getManufacturer() {
        return manufacturer;
    }

    @Override
    public String getSocket() {
        return socket;
    }

    @Override
    public Integer getMaxSpeedMhz() {
        return maxSpeedMHz;
    }

    @Override
    public Integer getTotalCores() {
        return totalCores;
    }

    @Override
    public State getState() {
        return statusDto.getState();
    }

    @Override
    public Health getHealth() {
        return statusDto.getHealth();
    }

    @Override
    public Health getHealthRollup() {
        return statusDto.getHealthRollup();
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

    public String getProcessorType() {
        return processorType;
    }

    public String getProcessorArchitecture() {
        return processorArchitecture;
    }

    public InstructionSet getInstructionSet() {
        return instructionSet;
    }

    @Override
    public ConvertableEnumListHolder<InstructionSetExtension> getInstructionSetExtensions() {
        return new ConvertableEnumListHolder<>(InstructionSetExtension.class, instructionSetExtensionList);
    }

    @Override
    public PsmeProcessorId getProcessorId() {
        return processorId;
    }

    public Integer getEnabledCores() {
        return enabledCores;
    }

    public Integer getTotalThreads() {
        return totalThreads;
    }

    public Integer getEnabledThreads() {
        return enabledThreads;
    }
}
