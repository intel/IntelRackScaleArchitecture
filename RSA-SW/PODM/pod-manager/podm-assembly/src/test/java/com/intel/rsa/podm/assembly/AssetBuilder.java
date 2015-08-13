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

package com.intel.rsa.podm.assembly;

import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.common.types.InstructionSet;
import com.intel.rsa.common.types.InstructionSetExtension;
import com.intel.rsa.common.types.MemoryType;
import com.intel.rsa.common.types.ProcessorModel;
import com.intel.rsa.common.types.helpers.ConvertableEnumListHolder;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.Memory;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.Processor;
import com.intel.rsa.podm.business.entities.assets.StorageController;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class AssetBuilder {
    static final long PROCESSOR_QUANTITY = 4;
    static final ProcessorModel PROCESSOR_MODEL = ProcessorModel.X3;
    static final int PROCESSOR_TOTAL_CORES = 8;
    static final int PROCESSOR_MAX_SPEED = 2400;
    static final InstructionSet PROCESSOR_INSTRUCTION_SET = InstructionSet.X86_64;
    static final ConvertableEnumListHolder<InstructionSetExtension> PROCESSOR_INSTRUCTION_SET_EXTENSIONS =
            new ConvertableEnumListHolder<>(InstructionSetExtension.class, new ArrayList<InstructionSetExtension>() { {
                add(InstructionSetExtension.X86_AES);
                add(InstructionSetExtension.X86_AVX);
                add(InstructionSetExtension.X86_AVX2);
                add(InstructionSetExtension.X86_SSE);
                add(InstructionSetExtension.X86_MMX);
            } });

    static final int MEMORY_QUANTITY = 4;
    static final BigDecimal MEMORY_SIZE_GB = new BigDecimal(8);
    static final MemoryType MEMORY_TYPE = MemoryType.DDR4;
    static final int MEMORY_SPEED = 1600;

    static final int STORAGE_CONTROLLER_QUANTITY = 2;
    static final int LOCAL_DRIVE_QUANTITY = 2;
    static final BigDecimal LOCAL_DRIVE_CAPACITY_GB = new BigDecimal(160);
    static final DriveType LOCAL_DRIVE_TYPE = DriveType.SSD;

    static final int NETWORK_INTERFACE_QUANTITY = 2;
    static final int NETWORK_INTERFACE_SPEED = 1000;

    Blade buildBlade() {
        Blade blade = mock(Blade.class);
        Collection<Processor> processors = getProcessors();
        Collection<Memory> memoryModules = getMemoryModules();
        Collection<StorageController> storageControllers = getStorageControllers();
        Collection<NetworkInterface> networkInterfaces = getNetworkInterfaces();

        when(blade.getProcessors()).thenReturn(processors);
        when(blade.getMemory()).thenReturn(memoryModules);
        when(blade.getStorageControllers()).thenReturn(storageControllers);
        when(blade.getEthernetInterfaces()).thenReturn(networkInterfaces);

        return blade;
    }

    private Collection<Processor> getProcessors() {
        List<Processor> processors = new ArrayList<>();

        for (int i = 0; i < PROCESSOR_QUANTITY; i++) {
            processors.add(getProcessor());
        }

        return processors;
    }

    private Processor getProcessor() {
        Processor processor = mock(Processor.class);
        when(processor.getModel()).thenReturn(PROCESSOR_MODEL);
        when(processor.getTotalCores()).thenReturn(PROCESSOR_TOTAL_CORES);
        when(processor.getMaxSpeedMhz()).thenReturn(PROCESSOR_MAX_SPEED);
        when(processor.getInstructionSet()).thenReturn(PROCESSOR_INSTRUCTION_SET);
        when(processor.getInstructionSetExtensions()).thenReturn(PROCESSOR_INSTRUCTION_SET_EXTENSIONS);
        return processor;
    }

    private Collection<Memory> getMemoryModules() {
        List<Memory> memoryModules = new ArrayList<>();

        for (int i = 0; i < MEMORY_QUANTITY; i++) {
            memoryModules.add(getMemory());
        }

        return memoryModules;
    }

    private Memory getMemory() {
        Memory memory = mock(Memory.class);
        when(memory.getSizeGB()).thenReturn(MEMORY_SIZE_GB.intValue());
        when(memory.getType()).thenReturn(MEMORY_TYPE);
        when(memory.getSpeedMHz()).thenReturn(MEMORY_SPEED);
        return memory;
    }

    private Collection<StorageController> getStorageControllers() {
        List<StorageController> storageControllers = new ArrayList<>();

        for (int i = 0; i < STORAGE_CONTROLLER_QUANTITY; i++) {
            storageControllers.add(getStorageController());
        }

        return storageControllers;
    }

    private StorageController getStorageController() {
        StorageController storageController = mock(StorageController.class);
        Collection<Drive> localDrives = getLocalDrives();

        when(storageController.getDrives()).thenReturn(localDrives);

        return storageController;
    }

    private Collection<Drive> getLocalDrives() {
        List<Drive> localDrives = new ArrayList<>();

        for (int i = 0; i < LOCAL_DRIVE_QUANTITY; i++) {
            localDrives.add(getLocalDrive());
        }

        return localDrives;
    }

    private Drive getLocalDrive() {
        Drive drive = mock(Drive.class);
        when(drive.getType()).thenReturn(LOCAL_DRIVE_TYPE);
        when(drive.getCapacityGb()).thenReturn(LOCAL_DRIVE_CAPACITY_GB.intValue());

        return drive;
    }

    private Collection<NetworkInterface> getNetworkInterfaces() {
        List<NetworkInterface> networkInterfaces = new ArrayList<>();

        for (int i = 0; i < NETWORK_INTERFACE_QUANTITY; i++) {
            networkInterfaces.add(getNetworkInterface());
        }

        return networkInterfaces;
    }

    private NetworkInterface getNetworkInterface() {
        NetworkInterface networkInterface = mock(NetworkInterface.class);
        when(networkInterface.getSpeedMbps()).thenReturn(NETWORK_INTERFACE_SPEED);

        return networkInterface;
    }
}
