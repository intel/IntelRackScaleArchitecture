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

package com.intel.rsa.podm.assembly.matcher;

import com.intel.rsa.common.types.MemoryType;
import com.intel.rsa.podm.assembly.AllocationException;
import com.intel.rsa.podm.business.dto.RequestedMemory;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.entities.assets.Memory;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.stream.Collectors;

public class MemoryMatcher {
    public boolean matches(RequestedNode template, Collection<Memory> memoryModules) {
        if (template.getMemory() != null && !template.getMemory().isEmpty()) {
            return isTemplateMemoryMatchesMemoryModules(template.getMemory().get(0), memoryModules);
        }

        return !memoryModules.isEmpty();
    }

    public void validate(RequestedNode template) throws AllocationException {
        if (template.getMemory() != null && template.getMemory().size() > 1) {
            throw new AllocationException("Allocation of more than one memory type is not supported");
        }
    }

    private boolean isTemplateMemoryMatchesMemoryModules(RequestedMemory requestedMemory, Collection<Memory> memoryModules) {
        List<Memory> localFilteredMemoryModules = new ArrayList<>(memoryModules);

        localFilteredMemoryModules = filterMemoryByType(localFilteredMemoryModules, requestedMemory.getType());
        localFilteredMemoryModules = filterMemoryBySpeed(localFilteredMemoryModules, requestedMemory.getSpeedMHz());

        if (localFilteredMemoryModules.isEmpty()) {
            return false;
        }

        return isEnoughMemory(localFilteredMemoryModules, requestedMemory.getSizeGB());
    }

    private List<Memory> filterMemoryByType(List<Memory> memoryModules, MemoryType memoryType) {
        if (memoryType == null) {
            return memoryModules;
        }

        return memoryModules.stream()
                .filter(memory -> memory.getType() == memoryType)
                .collect(Collectors.toList());
    }

    private List<Memory> filterMemoryBySpeed(List<Memory> memoryModules, Integer speedMHz) {
        if (speedMHz == null) {
            return memoryModules;
        }

        return memoryModules.stream()
                .filter(memory -> memory.getSpeedMHz() >= speedMHz)
                .collect(Collectors.toList());
    }

    private boolean isEnoughMemory(List<Memory> memoryModules, BigDecimal sizeGB) {
        if (sizeGB == null) {
            return true;
        }

        return memoryModules.stream().mapToInt(Memory::getSizeGB).sum() >= sizeGB.doubleValue();
    }
}
