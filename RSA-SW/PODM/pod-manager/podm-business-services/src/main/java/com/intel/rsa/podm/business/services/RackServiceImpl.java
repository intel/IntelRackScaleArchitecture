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
import com.intel.rsa.podm.business.dto.RackAttributesDto;
import com.intel.rsa.podm.business.dto.RackDto;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.Rack;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;
import java.util.Collections;
import java.util.LinkedList;
import java.util.stream.Collectors;

@Transactional(value = Transactional.TxType.REQUIRED)
public class RackServiceImpl implements RackService {
    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getRackCollection(Context context) throws EntityNotFoundException {
        Pod pod = (Pod) traverser.traverse(context);
        Collection<Rack> rackCollection = pod.getRacks();

        return new CollectionDto(
                CollectionDtoType.Rack,
                pod.getModified(), // TODO: get the latest date from child, or parent if no child present
                Contexts.getAsIdList(rackCollection)
        );
    }

    @Override
    public RackDto getRack(Context context) throws EntityNotFoundException {
        Rack rack = (Rack) traverser.traverse(context);
        return map(context, rack);
    }

    private RackDto map(Context context, Rack rack) {
        return RackDto
                .newBuilder()
                .id(rack.getId())
                .name(rack.getName())
                .location(rack.getLocation())
                .state(rack.getState())
                .health(rack.getHealth())
                .attributes(getRackAttributes(rack))
                .modified(rack.getModified())
                .context(context)
                .managedBy(Collections.<Context>emptyList())
                .build();
    }

    private RackAttributesDto getRackAttributes(Rack rack) {
        Collection<RackAttributesDto.RackTray> presentTrays = rack.getRackAttributes().getRuidsOfPresentTrays()
                .stream().map(RackAttributesDto.RackTray::new).collect(Collectors.toCollection(LinkedList::new));

        return RackAttributesDto
                  .newBuilder()
                  .isRmmPresent(rack.getRackAttributes().isRmmPresent())
                  .addPresentTrays(presentTrays)
                  .build();
    }
}
