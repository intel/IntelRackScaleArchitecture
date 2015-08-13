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

package com.intel.rsa.podm.business.services;

import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.CollectionDtoType;
import com.intel.rsa.podm.business.dto.SwitchPortDto;
import com.intel.rsa.podm.business.entities.assets.Switch;
import com.intel.rsa.podm.business.entities.assets.SwitchPort;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;


@Transactional(REQUIRED)
public class SwitchPortServiceImpl implements SwitchPortService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getSwitchPortCollection(Context context) throws EntityNotFoundException {
        Switch aSwitch = (Switch) traverser.traverse(context);
        return new CollectionDto(CollectionDtoType.SwitchPort, aSwitch.getModified(), getAsIdList(aSwitch.getSwitchPorts()));
    }

    @Override
    public SwitchPortDto getSwitchPort(Context context) throws EntityNotFoundException {
        SwitchPort port = (SwitchPort) traverser.traverse(context);

        return map(context, port);
    }

    private SwitchPortDto map(Context context, SwitchPort port) {
        return SwitchPortDto
                .newBuilder()
                .id(port.getId())
                .name(port.getName())
                .location(port.getLocation())
                .modified(port.getModified())
                .health(port.getHealth())
                .healthRollup(port.getHealthRollup())
                .state(port.getState())
                .operationalState(port.getOperationalState())
                .administrativeState(port.getAdministrativeState())
                .linkType(port.getLinkType())
                .linkSpeedGbps(port.getLinkSpeedGbps())
                .neighbourPort(port.getNeighbourPort())
                .containedBy(context.getParent())
                .context(context)
                .build();
    }
}
