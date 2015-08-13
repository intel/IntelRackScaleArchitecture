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

package com.intel.rsa.podm.assembly.allocation;

import com.intel.rsa.common.types.Id;
import com.intel.rsa.podm.assembly.AllocationException;
import com.intel.rsa.podm.assembly.NodeComposer;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.entities.components.ComposedNode;

import javax.inject.Inject;
import javax.transaction.Transactional;

import static javax.transaction.Transactional.TxType.REQUIRED;

/**
 * @deprecated It will be removed
 */
@Transactional(REQUIRED)
@Deprecated
public class DeprecatedNodeAllocatorImpl implements NodeAllocator {
    @Inject
    private NodeComposer nodeComposer;

    @Override
    public Id allocate(RequestedNode template) throws AllocationException {
        ComposedNode composedNode = nodeComposer.allocate(template);
        nodeComposer.allocateRemoteTargetIfRequired(template, composedNode.getId());
        return composedNode.getId();
    }
}
