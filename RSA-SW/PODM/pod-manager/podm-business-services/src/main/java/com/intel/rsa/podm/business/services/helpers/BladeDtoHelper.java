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

package com.intel.rsa.podm.business.services.helpers;

import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.podm.business.dto.BootDto;
import com.intel.rsa.podm.business.entities.assets.properties.Boot;

import java.util.LinkedList;
import java.util.List;

public class BladeDtoHelper {
    public BladeDtoHelper() {
    }

    public BootDto buildBoot(Boot boot) {
        //Since this value is optional it can be null
        if (boot == null) {
            return null;
        }

        return BootDto
                .newBuilder()
                .bootSourceOverrideEnabled(boot.getBootSourceOverrideEnabled())
                .bootSourceOverrideTarget(boot.getBootSourceOverrideTarget())
                .bootSourceOverrideSupported(getBootSourceOverrideSupportedList(boot))
                .uefiTargetBootSourceOverride(boot.getUefiTargetBootSourceOverride())
                .build();
    }

    private List<BootSourceType> getBootSourceOverrideSupportedList(Boot boot) {
        List<BootSourceType> bootSourceOverrideSupportedList = new LinkedList<>();
        boot.getBootSourceOverrideSupported().forEach(bootSourceOverrideSupported -> bootSourceOverrideSupportedList.add(
                bootSourceOverrideSupported.getBootSourceType()
        ));
        return bootSourceOverrideSupportedList;
    }
}
