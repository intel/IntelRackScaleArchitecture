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
import com.intel.rsa.podm.business.dto.PodDto;
import com.intel.rsa.podm.business.entities.assets.Pod;
import com.intel.rsa.podm.business.entities.assets.PodCollection;
import com.intel.rsa.podm.business.entities.assets.PodCollectionProvider;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.Contexts.asManagerContexts;

@Transactional(value = Transactional.TxType.REQUIRED)
public class PodServiceImpl implements PodService {

    @Inject
    private PodCollectionProvider podCollectionProvider;

    @Inject
    private DomainObjectTreeTraverser traverser;

    @Override
    public CollectionDto getPodCollection() {
        PodCollection podCollection = podCollectionProvider.getInstance();

        return new CollectionDto(
                CollectionDtoType.Pod,
                podCollection.getModified(),
                Contexts.getAsIdList(podCollection.getMembers())
        );
    }

    @Override
    public PodDto getPod(Context context) throws EntityNotFoundException {
        Pod pod = (Pod) traverser.traverse(context);
        return map(context, pod);
    }

    private PodDto map(Context context, Pod pod) {
        return PodDto
                .newBuilder()
                .id(pod.getId())
                .name(pod.getName())
                .location(pod.getLocation())
                .modified(pod.getModified())
                .enumStatus(pod.getEnumStatus())
                .context(context)
                .state(pod.getState())
                .health(pod.getHealth())
                .managedBy(asManagerContexts(pod.getManagers()))
                .build();
    }
}
