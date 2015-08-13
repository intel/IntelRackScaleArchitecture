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

package com.intel.rsa.podm.discovery.external.mapper.psme;

import com.intel.rsa.client.api.reader.psme.PsmeBlade;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.properties.Boot;
import com.intel.rsa.podm.business.entities.assets.properties.BootSource;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.discovery.external.mapper.DomainObjectMapper;
import com.intel.rsa.common.types.BootSourceType;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;

@Dependent
public class BladeMapper extends DomainObjectMapper<PsmeBlade, Blade> {
    @Inject
    private DomainObjectRepository repository;

    public BladeMapper() {
        super(PsmeBlade.class, Blade.class);
        registerProvider(Boot.class, source -> provideBoot());
        registerProvider(BootSource.class, this::provideBootSource);
    }

    @Override
    protected void performNotAutomatedMapping(PsmeBlade source, Blade target) {
        target.updateModified();
    }

    private Boot provideBoot() {
        Boot boot = target.getBoot();

        if (boot == null) {
            boot = repository.create(Boot.class);
        }

        return boot;
    }

    private BootSource provideBootSource(BootSourceType bootSourceType) {
        for (BootSource bootSource : target.getBoot().getBootSourceOverrideSupported()) {
            if (bootSource.getBootSourceType() == bootSourceType) {
                return bootSource;
            }
        }

        BootSource bootSource = repository.create(BootSource.class);
        bootSource.setBootSourceType(bootSourceType);
        return bootSource;
    }
}
