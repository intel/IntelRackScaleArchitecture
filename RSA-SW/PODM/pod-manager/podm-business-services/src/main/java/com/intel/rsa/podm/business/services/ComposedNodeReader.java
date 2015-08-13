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

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.CollectionDtoType;
import com.intel.rsa.podm.business.dto.ComposedNodeDto;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.Memory;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.Processor;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.components.ComposedNode;
import com.intel.rsa.podm.business.entities.components.ComposedNodeCollection;
import com.intel.rsa.podm.business.entities.components.ComposedNodeCollectionProvider;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;
import com.intel.rsa.podm.business.services.context.DomainObjectTreeTraverser;
import com.intel.rsa.podm.business.services.dto.ComposedNodeDtoImpl;
import com.intel.rsa.podm.business.services.dto.MemoryImpl;
import com.intel.rsa.podm.business.services.dto.ProcessorsImpl;
import com.intel.rsa.podm.business.services.dto.StatusDtoImpl;

import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.stream.Collectors;

import static com.intel.rsa.podm.business.services.Contexts.toContext;

@Transactional(Transactional.TxType.REQUIRED)
public class ComposedNodeReader {

    @Inject
    private ComposedNodeCollectionProvider collectionProvider;

    @Inject
    private DomainObjectTreeTraverser traverser;

    public CollectionDto getCollection() throws EntityNotFoundException {
        ComposedNodeCollection composedNodes = getComposedNodeCollection();
        return new CollectionDto(
                CollectionDtoType.ComposedNode,
                composedNodes.getModified(),
                composedNodes.getMembers().stream().map(ComposedNode::getId).collect(Collectors.toList())
        );
    }

    private ComposedNodeCollection getComposedNodeCollection() {
        return collectionProvider.getInstance();
    }

    public ComposedNodeDto getComposedNode(Context composedNodeContext) throws EntityNotFoundException {
        ComposedNode composedNode = getComposedNodeByContext(composedNodeContext);
        ComposedNodeDtoImpl result = new ComposedNodeDtoImpl(composedNodeContext.getId(), composedNodeContext);
        result.setName(composedNode.getName());
        result.setDescription(composedNode.getDescription());
        result.setModified(composedNode.getModified());
        result.setSystemType(composedNode.getSystemType());
        result.setStatus(getComposedNodeStatus(composedNode));
        result.setComposedNodeState(composedNode.getComposedNodeState());

        Blade blade = composedNode.getBlade();

        summarizeProcessors(composedNode, result, blade);
        summarizeMemory(result, blade);

        mapLinks(composedNode, result);
        return result;
    }

    public ComposedNode getComposedNodeByContext(Context composedNodeContext) throws EntityNotFoundException {
        return (ComposedNode) traverser.traverse(composedNodeContext);
    }

    private void summarizeProcessors(ComposedNode composedNode, ComposedNodeDtoImpl result, Blade blade) {
        ProcessorsImpl processorSummary = new ProcessorsImpl();
        // TODO figure out how to determine combined status of group of processors
        processorSummary.setStatus(new StatusDtoImpl(State.ENABLED, Health.OK, null));
        processorSummary.setCount(composedNode.getProcessors().size());
        processorSummary.setProcessorSockets(blade != null ? blade.getProcessorSockets() : null);
        processorSummary.setProcessorFamily(blade != null ? blade.getProcessorsFamily() : null);
        result.setProcessors(processorSummary);
    }

    private void summarizeMemory(ComposedNodeDtoImpl result, Blade blade) {
        MemoryImpl memorySummary = new MemoryImpl();
        // TODO figure out how to determine combined status of memory modules
        memorySummary.setStatus(new StatusDtoImpl(State.ENABLED, Health.OK, null));
        memorySummary.setMemorySockets(blade != null ? blade.getMemorySockets() : null);
        memorySummary.setTotalSystemMemoryGB(blade != null ? blade.getTotalSystemMemoryGB() : 0);
        result.setMemory(memorySummary);
    }

    private ComposedNodeDto.StatusDto getComposedNodeStatus(ComposedNode composedNode) {
        return new StatusDtoImpl(
                composedNode.getState(),
                composedNode.getHealth(),
                composedNode.getHealthRollup());
    }

    private void mapLinks(ComposedNode composedNode, ComposedNodeDtoImpl result) {
        for (Processor processor : composedNode.getProcessors()) {
            result.addProcessorContext(toContext(processor));
        }
        for (Memory memory : composedNode.getMemory()) {
            result.addMemoryContext(toContext(memory));
        }
        for (NetworkInterface networkInterface : composedNode.getNetworkInterfaces()) {
            result.addEthernetInterfaceContext(toContext(networkInterface));
        }
        for (Manager manager : composedNode.getManagers()) {
            result.addManagedByContext(toContext(manager));
        }
        mapStorageLinks(composedNode, result);
    }

    private void mapStorageLinks(ComposedNode composedNode, ComposedNodeDtoImpl result) {
        for (RemoteTarget remoteTarget : composedNode.getRemoteTargets()) {
            result.addRemoteDriveContext(toContext(remoteTarget));
        }
        for (Drive drive : composedNode.getLocalDrives()) {
            result.addLocalDriveContext(toContext(drive));
        }
    }
}
