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
import com.intel.rsa.podm.business.dto.RequestedNode;

public interface NodeAllocator {
    /**
     * Tries to find resources matching request and marks them as allocated. Composed Node will be created.
     * If Remote target must be created it will be performed asynchronously - ComposedState of Composed Node will be "Allocating"
     * until Remote Target creation will be completed.
     * @param request request defining requirements for allocated Composed Node
     * @return ID of allocated {@link com.intel.rsa.podm.business.entities.components.ComposedNode}
     * @throws AllocationException if allocation could not be completed successfully because required resources are unavailable.
     */
    Id allocate(RequestedNode request) throws AllocationException;
}
