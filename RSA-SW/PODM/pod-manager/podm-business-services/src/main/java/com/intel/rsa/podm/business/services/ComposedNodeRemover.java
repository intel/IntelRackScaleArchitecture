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

import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import javax.transaction.Transactional;

@Dependent
public class ComposedNodeRemover {

    @Inject
    private DomainObjectRepository repository;

    @Transactional(Transactional.TxType.REQUIRED)
    public void remove(ComposedNode composedNode) {
        // TODO handle optimistic locking on removal
        repository.delete(ComposedNode.class, composedNode.getId());
    }
}
