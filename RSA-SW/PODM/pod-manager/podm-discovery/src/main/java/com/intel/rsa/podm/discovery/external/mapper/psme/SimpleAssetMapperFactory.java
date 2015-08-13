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

import com.intel.rsa.client.api.reader.psme.PsmeDrive;
import com.intel.rsa.client.api.reader.psme.PsmeSwitch;
import com.intel.rsa.client.api.reader.psme.PsmeSwitchPort;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.Switch;
import com.intel.rsa.podm.business.entities.assets.SwitchPort;
import com.intel.rsa.podm.discovery.external.mapper.DomainObjectMapper;

import javax.enterprise.context.Dependent;
import javax.enterprise.inject.Produces;

/**
 * Factory for Asset Mappers. Produces simple implementation of Asset Mappers.
 */
@Dependent
public class SimpleAssetMapperFactory {

    @Produces
    public DomainObjectMapper<PsmeDrive, Drive> createDriveMapper() {
        return new DomainObjectMapper<PsmeDrive, Drive>(PsmeDrive.class, Drive.class) {
            @Override
            protected void performNotAutomatedMapping(PsmeDrive source, Drive target) {
                target.updateModified();
            }
        };
    }

    @Produces
    public DomainObjectMapper<PsmeSwitch, Switch> createSwitchMapper() {
        return new DomainObjectMapper<PsmeSwitch, Switch>(PsmeSwitch.class, Switch.class) {
            @Override
            protected void performNotAutomatedMapping(PsmeSwitch source, Switch target) {
                target.updateModified();
            }
        };
    }

    @Produces
    public DomainObjectMapper<PsmeSwitchPort, SwitchPort> createSwitchPortMapper() {
        return new DomainObjectMapper<PsmeSwitchPort, SwitchPort>(PsmeSwitchPort.class, SwitchPort.class) {
            @Override
            protected void performNotAutomatedMapping(PsmeSwitchPort source, SwitchPort target) {
                target.updateModified();
            }
        };
    }

}
