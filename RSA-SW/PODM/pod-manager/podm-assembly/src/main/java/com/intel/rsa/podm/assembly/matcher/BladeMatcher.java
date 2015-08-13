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

import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.assembly.AllocationException;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.StorageController;

import javax.inject.Inject;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public class BladeMatcher {
    @Inject
    protected ProcessorMatcher processorMatcher;
    @Inject
    protected MemoryMatcher memoryMatcher;
    @Inject
    protected LocalDriveMatcher localDriveMatcher;
    @Inject
    protected NetworkInterfaceMatcher networkInterfaceMatcher;

    public List<Blade> matches(RequestedNode template, Set<Location> locations, Collection<Blade> blades) throws AllocationException {
        validateMatchers(template);
        Location location = validateAndGetBladeLocation(locations);

        return blades.stream()
                .filter(blade -> matchBladeLocation(location, blade))
                .filter(blade -> processorMatcher.matches(template, blade.getProcessors()))
                .filter(blade -> memoryMatcher.matches(template, blade.getMemory()))
                .filter(blade -> localDriveMatcher.matches(template, collectDrivesFromBlade(blade)))
                .filter(blade -> networkInterfaceMatcher.matches(template, blade.getEthernetInterfaces()))
                .collect(Collectors.toList());
    }

    private void validateMatchers(RequestedNode template) throws AllocationException {
        processorMatcher.validate(template);
        memoryMatcher.validate(template);
    }

    private Location validateAndGetBladeLocation(Set<Location> locations) throws AllocationException {
        if (locations != null && locations.size() > 1) {
            throw new AllocationException("Allocation of assets on multiple locations is not supported");
        }

        if (locations == null || locations.isEmpty()) {
            return null;
        }

        return locations.iterator().next();
    }

    private List<Drive> collectDrivesFromBlade(Blade blade) {
        List<Drive> drives = new ArrayList<>();
        for (StorageController storageController : blade.getStorageControllers()) {
            drives.addAll(storageController.getDrives());
        }
        return drives;
    }

    private boolean matchBladeLocation(Location location, Blade blade) {
        return location == null || location.equals(blade.getLocation());

    }
}
