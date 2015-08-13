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

import com.intel.rsa.podm.assembly.matcher.BladeMatcherTestImpl;
import com.intel.rsa.podm.assembly.templates.LocalDriveOnlyTestRequestedNode;
import com.intel.rsa.podm.assembly.templates.NetworkInterfaceOnlyTestRequestedNode;
import com.intel.rsa.podm.assembly.templates.TemplateBuilder;
import com.intel.rsa.podm.assembly.templates.TestRequestedNode;
import com.intel.rsa.podm.assembly.templates.MemoryOnlyTestRequestedNode;
import com.intel.rsa.podm.assembly.templates.ProcessorOnlyTestRequestedNode;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import org.testng.annotations.Test;

import java.util.ArrayList;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertEquals;

public class NodeComposerTest {
    private AssetBuilder assetBuilder = new AssetBuilder();
    private TemplateBuilder templateBuilder = new TemplateBuilder();

    private Blade blade = assetBuilder.buildBlade();

    private NodeComposer nodeComposer = buildNodeComposer(blade);


    @Test
    public void whenEmptyTemplateUsed_shouldReturnProperBlade() throws AllocationException {
        Blade returnedBlade = nodeComposer.findBlade(new TestRequestedNode());
        assertEquals(blade, returnedBlade);
    }

    @Test
    public void whenValidProcessorTemplateUsed_shouldReturnProperBlade() throws AllocationException {
        Blade returnedBlade = nodeComposer.findBlade(new ProcessorOnlyTestRequestedNode(templateBuilder.getProcessors(
                1,
                AssetBuilder.PROCESSOR_QUANTITY,
                AssetBuilder.PROCESSOR_MODEL,
                AssetBuilder.PROCESSOR_TOTAL_CORES - 1,
                AssetBuilder.PROCESSOR_MAX_SPEED - 1,
                AssetBuilder.PROCESSOR_INSTRUCTION_SET,
                AssetBuilder.PROCESSOR_INSTRUCTION_SET_EXTENSIONS.toList(),
                null
        )));
        assertEquals(blade, returnedBlade);
    }

    @Test(expectedExceptions = AllocationException.class, expectedExceptionsMessageRegExp = "No assets available for allocation!")
    public void whenInvalidProcessorTemplateUsed_shouldThrowException() throws AllocationException {
        nodeComposer.findBlade(new ProcessorOnlyTestRequestedNode(templateBuilder.getProcessors(
                1,
                AssetBuilder.PROCESSOR_QUANTITY,
                AssetBuilder.PROCESSOR_MODEL,
                AssetBuilder.PROCESSOR_TOTAL_CORES,
                AssetBuilder.PROCESSOR_MAX_SPEED + 1,
                AssetBuilder.PROCESSOR_INSTRUCTION_SET,
                AssetBuilder.PROCESSOR_INSTRUCTION_SET_EXTENSIONS.toList(),
                null
        )));
    }

    @Test(expectedExceptions = AllocationException.class, expectedExceptionsMessageRegExp = "Allocation of more than one processor type is not supported")
    public void whenTooManyProcessorElementsUsed_shouldThrowException() throws AllocationException {
        nodeComposer.findBlade(new ProcessorOnlyTestRequestedNode(templateBuilder.getProcessors(
                2,
                AssetBuilder.PROCESSOR_QUANTITY,
                AssetBuilder.PROCESSOR_MODEL,
                AssetBuilder.PROCESSOR_TOTAL_CORES,
                AssetBuilder.PROCESSOR_MAX_SPEED,
                AssetBuilder.PROCESSOR_INSTRUCTION_SET,
                AssetBuilder.PROCESSOR_INSTRUCTION_SET_EXTENSIONS.toList(),
                null
        )));
    }

    @Test
    public void whenValidMemoryTemplateUsed_shouldReturnProperBlade() throws AllocationException {
        Blade returnedBlade = nodeComposer.findBlade(new MemoryOnlyTestRequestedNode(templateBuilder.getMemoryModules(
                1,
                AssetBuilder.MEMORY_SIZE_GB,
                AssetBuilder.MEMORY_SPEED - 1,
                AssetBuilder.MEMORY_TYPE,
                null
        )));
        assertEquals(blade, returnedBlade);
    }

    @Test(expectedExceptions = AllocationException.class, expectedExceptionsMessageRegExp = "No assets available for allocation!")
    public void whenInvalidMemoryTemplateUsed_shouldThrowException() throws AllocationException {
        nodeComposer.findBlade(new MemoryOnlyTestRequestedNode(templateBuilder.getMemoryModules(
                1,
                AssetBuilder.MEMORY_SIZE_GB,
                AssetBuilder.MEMORY_SPEED + 1,
                AssetBuilder.MEMORY_TYPE,
                null
        )));
    }

    @Test(expectedExceptions = AllocationException.class, expectedExceptionsMessageRegExp = "Allocation of more than one memory type is not supported")
    public void whenTooManyMemoryElementsUsed_shouldThrowException() throws AllocationException {
        nodeComposer.findBlade(new MemoryOnlyTestRequestedNode(templateBuilder.getMemoryModules(
                2,
                AssetBuilder.MEMORY_SIZE_GB,
                AssetBuilder.MEMORY_SPEED,
                AssetBuilder.MEMORY_TYPE,
                null
        )));
    }

    @Test
    public void whenValidLocalDriveTemplateUsed_shouldReturnProperBlade() throws AllocationException {
        Blade returnedBlade = nodeComposer.findBlade(new LocalDriveOnlyTestRequestedNode(templateBuilder.getLocalDrives(
                AssetBuilder.LOCAL_DRIVE_QUANTITY * AssetBuilder.STORAGE_CONTROLLER_QUANTITY,
                AssetBuilder.LOCAL_DRIVE_CAPACITY_GB,
                AssetBuilder.LOCAL_DRIVE_TYPE,
                null,
                null
        )));
        assertEquals(blade, returnedBlade);
    }

    @Test(expectedExceptions = AllocationException.class, expectedExceptionsMessageRegExp = "No assets available for allocation!")
    public void whenInvalidLocalDriveTemplateUsed_shouldThrowException() throws AllocationException {
        nodeComposer.findBlade(new LocalDriveOnlyTestRequestedNode(templateBuilder.getLocalDrives(
                AssetBuilder.LOCAL_DRIVE_QUANTITY * AssetBuilder.STORAGE_CONTROLLER_QUANTITY,
                AssetBuilder.LOCAL_DRIVE_CAPACITY_GB.add(AssetBuilder.LOCAL_DRIVE_CAPACITY_GB),
                AssetBuilder.LOCAL_DRIVE_TYPE,
                null,
                null
        )));
    }

    @Test
    public void whenValidNetworkInterfaceTemplateUsed_shouldReturnProperBlade() throws AllocationException {
        Blade returnedBlade = nodeComposer.findBlade(new NetworkInterfaceOnlyTestRequestedNode(templateBuilder.getNetworkInterfaces(
                AssetBuilder.NETWORK_INTERFACE_QUANTITY,
                AssetBuilder.NETWORK_INTERFACE_SPEED,
                null,
                null
        )));
        assertEquals(blade, returnedBlade);
    }

    @Test(expectedExceptions = AllocationException.class, expectedExceptionsMessageRegExp = "No assets available for allocation!")
    public void whenInvalidNetworkInterfaceTemplateUsed_shouldThrowException() throws AllocationException {
        Blade returnedBlade = nodeComposer.findBlade(new NetworkInterfaceOnlyTestRequestedNode(templateBuilder.getNetworkInterfaces(
                AssetBuilder.NETWORK_INTERFACE_QUANTITY,
                AssetBuilder.NETWORK_INTERFACE_SPEED + 2,
                null,
                null
        )));
        assertEquals(blade, returnedBlade);
    }

    @Test
    public void whenValidFullTemplateUsed_shouldReturnProperBlade() throws AllocationException {
        Blade returnedBlade = nodeComposer.findBlade(new TestRequestedNode(
                templateBuilder.getProcessors(
                        1,
                        AssetBuilder.PROCESSOR_QUANTITY,
                        AssetBuilder.PROCESSOR_MODEL,
                        AssetBuilder.PROCESSOR_TOTAL_CORES,
                        AssetBuilder.PROCESSOR_MAX_SPEED,
                        AssetBuilder.PROCESSOR_INSTRUCTION_SET,
                        AssetBuilder.PROCESSOR_INSTRUCTION_SET_EXTENSIONS.toList(),
                        null
                ),
                templateBuilder.getMemoryModules(
                        1,
                        AssetBuilder.MEMORY_SIZE_GB,
                        AssetBuilder.MEMORY_SPEED,
                        AssetBuilder.MEMORY_TYPE,
                        null
                ),
                templateBuilder.getLocalDrives(
                        AssetBuilder.LOCAL_DRIVE_QUANTITY * AssetBuilder.STORAGE_CONTROLLER_QUANTITY,
                        AssetBuilder.LOCAL_DRIVE_CAPACITY_GB,
                        AssetBuilder.LOCAL_DRIVE_TYPE,
                        null,
                        null
                ),
                templateBuilder.getNetworkInterfaces(
                    AssetBuilder.NETWORK_INTERFACE_QUANTITY,
                    AssetBuilder.NETWORK_INTERFACE_SPEED,
                    null,
                    null
        )));
        assertEquals(blade, returnedBlade);
    }

    @Test(expectedExceptions = AllocationException.class, expectedExceptionsMessageRegExp = "No assets available for allocation!")
    public void whenInvalidFullTemplateUsed_shouldThrowException() throws AllocationException {
        Blade returnedBlade = nodeComposer.findBlade(new TestRequestedNode(
                templateBuilder.getProcessors(
                        1,
                        AssetBuilder.PROCESSOR_QUANTITY,
                        AssetBuilder.PROCESSOR_MODEL,
                        AssetBuilder.PROCESSOR_TOTAL_CORES,
                        AssetBuilder.PROCESSOR_MAX_SPEED + 1,
                        AssetBuilder.PROCESSOR_INSTRUCTION_SET,
                        AssetBuilder.PROCESSOR_INSTRUCTION_SET_EXTENSIONS.toList(),
                        null
                ),
                templateBuilder.getMemoryModules(
                        1,
                        AssetBuilder.MEMORY_SIZE_GB,
                        AssetBuilder.MEMORY_SPEED,
                        AssetBuilder.MEMORY_TYPE,
                        null
                ),
                templateBuilder.getLocalDrives(
                        AssetBuilder.LOCAL_DRIVE_QUANTITY * AssetBuilder.STORAGE_CONTROLLER_QUANTITY,
                        AssetBuilder.LOCAL_DRIVE_CAPACITY_GB,
                        AssetBuilder.LOCAL_DRIVE_TYPE,
                        null,
                        null
                ),
                templateBuilder.getNetworkInterfaces(
                        AssetBuilder.NETWORK_INTERFACE_QUANTITY,
                        AssetBuilder.NETWORK_INTERFACE_SPEED,
                        null,
                        null
                )));
        assertEquals(blade, returnedBlade);
    }

    private NodeComposer buildNodeComposer(Blade blade) {
        NodeComposer nodeComposer = new NodeComposer();
        nodeComposer.repository = mock(DomainObjectRepository.class);
        nodeComposer.bladeMatcher = new BladeMatcherTestImpl();

        when(nodeComposer.repository.getAllByFlag(Blade.class, Blade.IS_ALLOCATED, false)).thenReturn(
                new ArrayList<Blade>() { {
                    add(blade);
                } }
        );

        return nodeComposer;
    }
}
