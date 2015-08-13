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

import com.intel.rsa.common.types.actions.ResetType;
import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.ComposedNodeDto;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.exceptions.EntityOperationException;
import com.intel.rsa.podm.business.services.context.Context;

public interface ComposedNodeService {
    /**
     * @return Global collection of composed nodes.
     */
    CollectionDto getCollection() throws EntityNotFoundException;

    /**
     * Returns representation of a specific composed nodes.
     * @param composedNodeContext
     * @return DTO representation of found ComposedNode
     * @throws  EntityNotFoundException if composed node is not found in the context specified by the argument
     */
    ComposedNodeDto getComposedNode(Context composedNodeContext) throws EntityNotFoundException;

    /**
     * Creates a single ComposedNode element based on ComposedNodeTemplate. Returns the Context of created entity.
     * @param requestedNode
     * @return Context of created ComposedNode
     * @throws EntityOperationException
     */
    Context createComposedNode(RequestedNode requestedNode) throws EntityOperationException;

    /**
     * Try to perform an assembly action on a single ComposedNode defined by its context
     *
     * @param composedNodeContext
     * @throws EntityNotFoundException if composed node is not found in the context specified by the argument
     * @throws EntityOperationException if action failed to be performed
     */
    void assembleComposedNode(Context composedNodeContext) throws EntityNotFoundException, EntityOperationException;

    /**
     * Try to delete a single ComposedNode element defined by its context
     *
     * @param composedNodeContext
     * @throws EntityNotFoundException if composed node is not found in the context specified by the argument
     * @throws EntityOperationException if action delete operation failed
     */
    void deleteComposedNode(Context composedNodeContext) throws EntityNotFoundException, EntityOperationException;

    /**
     * Try to perform reset action on a single ComposedNode defined by its context
     *
     * @param composedNodeContext
     * @param resetType
     * @throws EntityNotFoundException if composed node is not found in the context specified by the argument
     * @throws EntityOperationException if action failed to be performed
     */
    void reset(Context composedNodeContext, ResetType resetType) throws EntityNotFoundException, EntityOperationException;
}
