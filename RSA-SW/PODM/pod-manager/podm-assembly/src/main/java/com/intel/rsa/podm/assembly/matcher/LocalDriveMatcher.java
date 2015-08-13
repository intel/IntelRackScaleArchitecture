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

import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.podm.business.dto.RequestedLocalDrive;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.entities.assets.Drive;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

public class LocalDriveMatcher {
    public boolean matches(RequestedNode template, Collection<Drive> drives) {
        if (template.getLocalDrives() != null && !template.getLocalDrives().isEmpty()) {
            if  (template.getLocalDrives().size() > drives.size()) {
                return false;
            }

            return areTemplateDrivesMatchDrives(template.getLocalDrives(), drives);
        }

        return true;
    }

    private boolean areTemplateDrivesMatchDrives(List<RequestedLocalDrive> templateDrives, Collection<Drive> drives) {
        List<Drive> filteredDrives = new ArrayList<>(drives);
        for (RequestedLocalDrive templateDrive : templateDrives) {
            List<Drive> localFilteredDrives = new ArrayList<>(filteredDrives);

            localFilteredDrives = filterDrivesByType(localFilteredDrives, templateDrive.getType());
            localFilteredDrives = filterDrivesByCapacity(localFilteredDrives, templateDrive.getCapacityGB());

            if (localFilteredDrives.isEmpty()) {
                return false;
            }

            Collections.sort(localFilteredDrives, (Drive d1, Drive d2) -> d1.getCapacityGb() - d2.getCapacityGb());
            filteredDrives.remove(localFilteredDrives.get(0));
        }

        return true;
    }

    private List<Drive> filterDrivesByType(List<Drive> drives, DriveType driveType) {
        if (driveType == null) {
            return drives;
        }

        return drives.stream()
                .filter(drive -> drive.getType() == driveType)
                .collect(Collectors.toList());
    }

    private List<Drive> filterDrivesByCapacity(List<Drive> drives, BigDecimal capacityGb) {
        if (capacityGb == null) {
            return drives;
        }

        return drives.stream()
                .filter(drive -> drive.getCapacityGb() >= capacityGb.doubleValue())
                .collect(Collectors.toList());
    }
}
