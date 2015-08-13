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
import com.intel.rsa.podm.business.dto.SwitchDto;
import com.intel.rsa.podm.business.entities.assets.FabricModule;
import com.intel.rsa.podm.business.entities.assets.Switch;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.dto.CollectionDtoType.Switch;
import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Transactional(REQUIRED)
public class SwitchServiceImpl implements SwitchService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getSwitchCollection(Context context) throws EntityNotFoundException {
        FabricModule fabricModule = (FabricModule) traverser.traverse(context);
        return new CollectionDto(Switch, fabricModule.getModified(), getAsIdList(fabricModule.getSwitches()));
    }

    @Override
    public SwitchDto getSwitch(Context context) throws EntityNotFoundException {
        Switch aSwitch = (Switch) traverser.traverse(context);
        return map(context, aSwitch);
    }

    private SwitchDto map(Context context, Switch aSwitch) {
        return SwitchDto
                .newBuilder()
                .id(aSwitch.getId())
                .context(context)
                .name(aSwitch.getName())
                .location(aSwitch.getLocation())
                .modified(aSwitch.getModified())
                .health(aSwitch.getHealth())
                .healthRollup(aSwitch.getHealthRollup())
                .state(aSwitch.getState())
                .managedBy(asManagerContexts(aSwitch.getManagers()))
                .containedBy(context.getParent())
                .build();
    }
}
