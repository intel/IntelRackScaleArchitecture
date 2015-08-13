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

package com.intel.rsa.podm.business.entities.assets.properties;

import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.common.types.BootSourceState;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;

import java.util.Collection;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class Boot extends DomainObject {
    public static final DomainObjectProperty<BootSourceState> BOOT_SOURCE_OVERRIDE_ENABLED = enumProperty("bootSourceOverrideEnabled", BootSourceState.class);
    public static final DomainObjectProperty<BootSourceType> BOOT_SOURCE_OVERRIDE_TARGET = enumProperty("bootSourceOverrideTarget", BootSourceType.class);
    public static final DomainObjectProperty<String> UEFI_TARGET_BOOT_SOURCE_OVERRIDE = stringProperty("uefiTargetBootSourceOverride");

    public BootSourceState getBootSourceOverrideEnabled() {
        return getProperty(BOOT_SOURCE_OVERRIDE_ENABLED);
    }

    public void setBootSourceOverrideEnabled(BootSourceState bootSourceOverrideEnabled) {
        setProperty(BOOT_SOURCE_OVERRIDE_ENABLED, bootSourceOverrideEnabled);
    }

    public BootSourceType getBootSourceOverrideTarget() {
        return getProperty(BOOT_SOURCE_OVERRIDE_TARGET);
    }

    public void setBootSourceOverrideTarget(BootSourceType bootSourceOverrideTarget) {
        setProperty(BOOT_SOURCE_OVERRIDE_TARGET, bootSourceOverrideTarget);
    }

    public Collection<BootSource> getBootSourceOverrideSupported() {
        return getLinked(AssetLink.CONTAINS, BootSource.class);
    }

    public void setBootSourceOverrideSupported(Collection<BootSource> bootSources) {
        bootSources.forEach(bootSource -> link(AssetLink.CONTAINS, bootSource));
    }

    public BootSource addBootSourceOverrideSupported() {
        return addDomainObject(AssetLink.CONTAINS, BootSource.class);
    }

    public String getUefiTargetBootSourceOverride() {
        return getProperty(UEFI_TARGET_BOOT_SOURCE_OVERRIDE);
    }

    public void setUefiTargetBootSourceOverride(String uefiTargetBootSourceOverride) {
        setProperty(UEFI_TARGET_BOOT_SOURCE_OVERRIDE, uefiTargetBootSourceOverride);
    }
}
