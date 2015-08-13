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
import com.intel.rsa.podm.business.dto.DrawerDto;
import com.intel.rsa.podm.business.entities.assets.Drawer;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;
import static com.intel.rsa.podm.business.services.Contexts.getAsIdList;

@Transactional(value = Transactional.TxType.REQUIRED)
public class DrawerServiceImpl implements DrawerService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getDrawerCollection(Context context) throws EntityNotFoundException {
        Rack rack = (Rack) traverser.traverse(context);

        Collection<Drawer> drawerCollection = rack.getDrawers();

        return new CollectionDto(
                CollectionDtoType.Drawer,
                rack.getModified(), // TODO: get the latest date from child, or parent if no child present
                getAsIdList(drawerCollection)
        );
    }

    @Override
    public DrawerDto getDrawer(Context context) throws EntityNotFoundException {
        Drawer drawer = (Drawer) traverser.traverse(context);
        return map(context, drawer);
    }

    private DrawerDto map(Context context, Drawer drawer) {
        return DrawerDto
                .newBuilder()
                .id(drawer.getId())
                .name(drawer.getName())
                .location(drawer.getLocation())
                .modified(drawer.getModified())
                .manufacturer(drawer.getManufacturer())
                .model(drawer.getModel())
                .serialNumber(drawer.getSerialNumber())
                .enumStatus(drawer.getEnumStatus())
                .health(drawer.getHealth())
                .state(drawer.getState())
                .healthRollup(drawer.getHealthRollup())
                .context(context)
                .managedBy(asManagerContexts(drawer.getManagers()))
                .build();
    }
}
