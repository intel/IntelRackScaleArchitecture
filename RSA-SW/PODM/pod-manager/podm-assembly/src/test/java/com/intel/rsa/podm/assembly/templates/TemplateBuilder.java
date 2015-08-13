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

package com.intel.rsa.podm.assembly.templates;

import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.common.types.InstructionSet;
import com.intel.rsa.common.types.InstructionSetExtension;
import com.intel.rsa.common.types.MemoryType;
import com.intel.rsa.common.types.ProcessorModel;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.dto.RequestedLocalDrive;
import com.intel.rsa.podm.business.dto.RequestedMemory;
import com.intel.rsa.podm.business.dto.RequestedNetworkInterface;
import com.intel.rsa.podm.business.dto.RequestedProcessor;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

public class TemplateBuilder {
    public List<RequestedProcessor> getProcessors(int processorElements, Long quantity, ProcessorModel model, Integer totalCores,
                                                              Integer maxSpeedMHz, InstructionSet instructionSet,
                                                              List<InstructionSetExtension> instructionSetExtensions, Location location) {
        List<RequestedProcessor> processors = new ArrayList<>();
        if (processorElements > 0) {
            for (int i = 0; i < processorElements; i++) {
                processors.add(getProcessor(quantity, model, totalCores, maxSpeedMHz, instructionSet, instructionSetExtensions, location));
            }
        }

        return processors;
    }

    public RequestedProcessor getProcessor(Long quantity, ProcessorModel model, Integer totalCores, Integer maxSpeedMHz,
                                                       InstructionSet instructionSet, List<InstructionSetExtension> instructionSetExtensions,
                                                       Location location) {
        return new RequestedProcessorImpl(quantity, model, totalCores, maxSpeedMHz, instructionSet, instructionSetExtensions, location);
    }

    public List<RequestedMemory> getMemoryModules(int memoryElements, BigDecimal sizeGB, Integer speedMHz, MemoryType type, Location location) {
        List<RequestedMemory> memoryModules = new ArrayList<>();
        if (memoryElements > 0) {
            for (int i = 0; i < memoryElements; i++) {
                memoryModules.add(getMemory(sizeGB, speedMHz, type, location));
            }
        }

        return memoryModules;
    }

    public RequestedMemory getMemory(BigDecimal sizeGB, Integer speedMHz, MemoryType type, Location location) {
        return new RequestedMemoryImpl(sizeGB, speedMHz, type, location);
    }

    public List<RequestedLocalDrive> getLocalDrives(int drivesElements, BigDecimal capacityGB, DriveType type,
                                                                Integer bootOrder, Location location) {
        List<RequestedLocalDrive> localDrives = new ArrayList<>();
        if (drivesElements > 0) {
            for (int i = 0; i < drivesElements; i++) {
                localDrives.add(getLocalDrive(capacityGB, type, bootOrder, location));
            }
        }

        return localDrives;
    }

    public RequestedLocalDrive getLocalDrive(BigDecimal capacityGB, DriveType type, Integer bootOrder, Location location) {
        return new RequestedLocalDriveImpl(capacityGB, type, bootOrder, location);
    }

    public List<RequestedNetworkInterface> getNetworkInterfaces(int interfacesElements, Integer speedMbps,
                                                                            List<RequestedNetworkInterface.Vlan> vlans, Location location) {
        List<RequestedNetworkInterface> networkInterfaces = new ArrayList<>();
        if (interfacesElements > 0) {
            for (int i = 0; i < interfacesElements; i++) {
                networkInterfaces.add(getNetworkInterface(speedMbps, vlans, location));
            }
        }

        return networkInterfaces;
    }

    public RequestedNetworkInterface getNetworkInterface(Integer speedMbps, List<RequestedNetworkInterface.Vlan> vlans,
                                                                     Location location) {
        return new RequestedNetworkInterfaceImpl(speedMbps, vlans, location);
    }

    private class RequestedProcessorImpl implements RequestedProcessor {
        final Long quantity;
        final ProcessorModel model;
        final Integer totalCores;
        final Integer maxSpeedMHz;
        final InstructionSet instructionSet;
        final List<InstructionSetExtension> instructionSetExtensions;
        final Location location;

        public RequestedProcessorImpl(Long quantity, ProcessorModel model, Integer totalCores, Integer maxSpeedMHz, InstructionSet instructionSet,
                                      List<InstructionSetExtension> instructionSetExtensions, Location location) {
            this.quantity = quantity;
            this.model = model;
            this.totalCores = totalCores;
            this.maxSpeedMHz = maxSpeedMHz;
            this.instructionSet = instructionSet;
            this.instructionSetExtensions = instructionSetExtensions;
            this.location = location;
        }

        @Override
        public Long getQuantity() {
            return quantity;
        }

        @Override
        public ProcessorModel getModel() {
            return model;
        }

        @Override
        public Integer getTotalCores() {
            return totalCores;
        }

        @Override
        public Integer getMaxSpeedMHz() {
            return maxSpeedMHz;
        }

        @Override
        public InstructionSet getInstructionSet() {
            return instructionSet;
        }

        @Override
        public List<InstructionSetExtension> getInstructionSetExtensions() {
            return instructionSetExtensions;
        }

        @Override
        public Location getLocation() {
            return location;
        }
    }

    private class RequestedMemoryImpl implements RequestedMemory {
        private final BigDecimal sizeGB;
        private final Integer speedMHz;
        private final MemoryType type;
        private final Location location;

        public RequestedMemoryImpl(BigDecimal sizeGB, Integer speedMHz, MemoryType type, Location location) {
            this.sizeGB = sizeGB;
            this.speedMHz = speedMHz;
            this.type = type;
            this.location = location;
        }

        @Override
        public BigDecimal getSizeGB() {
            return sizeGB;
        }

        @Override
        public Integer getSpeedMHz() {
            return speedMHz;
        }

        @Override
        public MemoryType getType() {
            return type;
        }

        @Override
        public Location getLocation() {
            return location;
        }
    }

    private class RequestedLocalDriveImpl implements RequestedLocalDrive {
        private final BigDecimal capacityGB;
        private final DriveType type;
        private final Integer bootOrder;
        private final Location location;

        public RequestedLocalDriveImpl(BigDecimal capacityGB, DriveType type, Integer bootOrder, Location location) {
            this.capacityGB = capacityGB;
            this.type = type;
            this.bootOrder = bootOrder;
            this.location = location;
        }

        @Override
        public BigDecimal getCapacityGB() {
            return capacityGB;
        }

        @Override
        public DriveType getType() {
            return type;
        }

        @Override
        public Integer getBootOrder() {
            return bootOrder;
        }

        @Override
        public Location getLocation() {
            return location;
        }
    }

    private class RequestedNetworkInterfaceImpl implements RequestedNetworkInterface {
        private final Integer speedMbps;
        private final List<Vlan> vlans;
        private final Location location;

        public RequestedNetworkInterfaceImpl(Integer speedMbps, List<Vlan> vlans, Location location) {
            this.speedMbps = speedMbps;
            this.vlans = vlans;
            this.location = location;
        }

        @Override
        public Integer getSpeedMbps() {
            return speedMbps;
        }

        @Override
        public List<Vlan> getVlans() {
            return vlans;
        }

        @Override
        public Location getLocation() {
            return location;
        }
    }
}
