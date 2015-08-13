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
import com.intel.rsa.podm.business.dto.FabricModuleDto;
import com.intel.rsa.podm.business.entities.assets.Drawer;
import com.intel.rsa.podm.business.entities.assets.FabricModule;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;

@Transactional(Transactional.TxType.REQUIRED)
public class FabricModuleServiceImpl implements FabricModuleService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getFabricModuleCollection(Context context) throws EntityNotFoundException {
            Drawer drawer = (Drawer) traverser.traverse(context);

            Collection<FabricModule> fabricModuleCollection = drawer.getFabricModules();

            return new CollectionDto(
                    CollectionDtoType.FabricModule,
                    drawer.getModified(), // TODO: get the latest date from child, or parent if no child present
                    getAsIdList(fabricModuleCollection)
            );
    }

    @Override
    public FabricModuleDto getFabricModule(Context context) throws EntityNotFoundException {
        FabricModule fabricModule = (FabricModule) traverser.traverse(context);
        return map(context, fabricModule);
    }

    private FabricModuleDto map(Context context, FabricModule fabricModule) {
        return FabricModuleDto.newBuilder()
                .id(fabricModule.getId())
                .context(context)
                .name(fabricModule.getName())
                .location(fabricModule.getLocation())
                .modified(fabricModule.getModified())
                .health(fabricModule.getHealth())
                .state(fabricModule.getState())
                .managedBy(asManagerContexts(fabricModule.getManagers()))
                .build();
    }
}
