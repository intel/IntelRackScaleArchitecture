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

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.actions.ResetType;
import com.intel.rsa.podm.actions.ActionException;
import com.intel.rsa.podm.actions.NodeReset;
import com.intel.rsa.podm.assembly.AllocationException;
import com.intel.rsa.podm.assembly.AssemblyException;
import com.intel.rsa.podm.assembly.DisassemblyException;
import com.intel.rsa.podm.assembly.NodeAssembler;
import com.intel.rsa.podm.assembly.NodeDisassembler;
import com.intel.rsa.podm.assembly.allocation.NodeAllocator;
import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.ComposedNodeDto;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.entities.components.ComposedNode;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.exceptions.EntityOperationException;
import com.intel.rsa.podm.business.services.context.Context;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.services.context.Context.contextOf;
import static com.intel.rsa.podm.business.services.context.ContextType.COMPOSED_NODE;

@RequestScoped
@Transactional(Transactional.TxType.REQUIRED)
public class ComposedNodeServiceImpl implements ComposedNodeService {

    @Inject
    private ComposedNodeReader composedNodeReader;

    @Inject
    private NodeAssembler nodeAssembler;

    @Inject
    private NodeDisassembler nodeDisassembler;

    @Inject
    private ComposedNodeRemover composedNodeRemover;

    @Inject
    private NodeReset nodeReset;

    @Inject
    private NodeAllocator nodeAllocator;

    @Inject
    private Logger logger;

    @Override
    public CollectionDto getCollection() throws EntityNotFoundException {
        return composedNodeReader.getCollection();
    }

    @Override
    public ComposedNodeDto getComposedNode(Context composedNodeContext) throws EntityNotFoundException {
        return composedNodeReader.getComposedNode(composedNodeContext);
    }

    @Override
    public Context createComposedNode(RequestedNode template) throws EntityOperationException {
        try {
            Id composedNodeId = nodeAllocator.allocate(template);
            return contextOf(composedNodeId, COMPOSED_NODE);
        } catch (AllocationException e) {
            String msg = "Creation failed due to allocation failure";
            logger.d(msg, e);
            throw new EntityOperationException(msg, e);
        }
    }

    @Override
    public void assembleComposedNode(Context composedNodeContext) throws EntityNotFoundException, EntityOperationException {
        ComposedNode composedNode = composedNodeReader.getComposedNodeByContext(composedNodeContext);
        try {
            nodeAssembler.assemble(composedNode);
        } catch (AssemblyException e) {
            String msg = "Assembly failed";
            logger.d(msg, e);
            throw new EntityOperationException(msg, e);
        }
    }

    @Override
    public void deleteComposedNode(Context composedNodeContext) throws EntityNotFoundException, EntityOperationException {
        ComposedNode composedNode = composedNodeReader.getComposedNodeByContext(composedNodeContext);
        try {
            nodeDisassembler.disassemble(composedNode);
        } catch (DisassemblyException e) {
            String msg = "Disassembly failed";
            logger.d(msg, e);
            throw new EntityOperationException(msg, e);
        }
        composedNodeRemover.remove(composedNode);
    }

    @Override
    public void reset(Context composedNodeContext, ResetType resetType) throws EntityNotFoundException, EntityOperationException {
        ComposedNode composedNode = composedNodeReader.getComposedNodeByContext(composedNodeContext);
        try {
            nodeReset.reset(composedNode, resetType);
        } catch (ActionException e) {
            String msg = "Reset action failed";
            logger.d(msg, e);
            throw new EntityOperationException(msg, e);
        }
    }
}
