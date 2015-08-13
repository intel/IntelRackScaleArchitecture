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

package com.intel.rsa.podm.assembly;

import com.intel.rsa.common.types.ComposedNodeState;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.assembly.matcher.BladeMatcher;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.dto.Localizable;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.StorageController;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.business.entities.components.ComposedNode;
import com.intel.rsa.podm.business.entities.components.ComposedNodeCollectionProvider;

import javax.enterprise.context.RequestScoped;
import javax.inject.Inject;
import javax.transaction.Transactional;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static java.util.Collections.emptyList;

/**
 * TODO handle possible optimistic lock exceptions (at least by performing allocations sequentially)
 */
@RequestScoped
public class NodeComposer {
    @Inject
    DomainObjectRepository repository;

    @Inject
    BladeMatcher bladeMatcher;

    @Inject
    private ComposedNodeCollectionProvider composedNodeCollectionProvider;

    @Inject
    private RemoteTargetSupplier remoteTargetSupplier;

    /**
     * Simple implementation of allocating Composed Nodes by template
     *
     * @param template
     * @return
     * @throws AllocationException
     */
    @Transactional(value = Transactional.TxType.REQUIRES_NEW, rollbackOn = AllocationException.class)
    public ComposedNode allocate(RequestedNode template) throws AllocationException {
        verifyTemplate(template);
        Blade blade = findBlade(template);

        ComposedNode node = createComposedNodeFromBlade(blade);
        node.setName(template.getName());
        node.setDescription(template.getDescription());
        node.setComposedNodeState(ComposedNodeState.ALLOCATING);

        return node;
    }

    @Transactional(value = Transactional.TxType.REQUIRES_NEW, rollbackOn = AllocationException.class)
    public ComposedNode allocateRemoteTargetIfRequired(RequestedNode template, Id composedNodeId) throws AllocationException {
        ComposedNode node = repository.get(ComposedNode.class, composedNodeId);
        if (isRemoteDriveRequired(template)) {
            RequestedRemoteDrive remoteDriveTemplate = single(template.getRemoteDrives());
            if (remoteTargetSupplier.isDefiningExistingTarget(remoteDriveTemplate)) {
                allocateRemoteTarget(node, remoteTargetSupplier.findRemoteTarget(remoteDriveTemplate));
            } else {
                if (remoteTargetSupplier.targetExists(remoteDriveTemplate)) {
                    throw new AllocationException("Target with provided iqn already exists.");
                }

                remoteTargetSupplier.initializeNewTargetCreation(node, remoteDriveTemplate);
            }
        } else {
            node.setComposedNodeState(ComposedNodeState.ALLOCATED);
        }
        return node;
    }

    public void handleRemoteTargetCreationFailure(ComposedNode composedNode) {
        composedNode.setComposedNodeState(ComposedNodeState.FAILED);
    }

    public void allocateRemoteTarget(ComposedNode node, RemoteTarget remoteTarget) {
        node.linkRemoteTarget(remoteTarget);
        remoteTarget.setAllocated(true);
        node.setComposedNodeState(ComposedNodeState.ALLOCATED);
    }

    private ComposedNode createComposedNodeFromBlade(Blade blade) {
        ComposedNode composedNode = repository.create(ComposedNode.class);
        composedNode.setSystemType(SystemType.PHYSICAL);
        composedNode.setState(State.ENABLED);
        composedNode.setHealth(Health.OK);
        composedNode.setHealthRollup(Health.OK);
        composedNode.updateModified();
        composedNode.linkBlade(blade);
        blade.getProcessors().forEach(composedNode::linkProcessor);
        blade.getMemory().forEach(composedNode::linkMemory);
        blade.getEthernetInterfaces().forEach(composedNode::linkNetworkInterface);
        for (StorageController storageController : blade.getStorageControllers()) {
            storageController.getDrives().forEach(composedNode::linkLocalDrive);
        }
        blade.setAllocated(true);
        // TODO verify if Pod Manager is the right one here
        blade.getComputeModule().getDrawer().getRack().getPod().getManagers().forEach(composedNode::linkManager);
        composedNodeCollectionProvider.getInstance().link(composedNode);
        return composedNode;
    }

    private boolean isRemoteDriveRequired(RequestedNode template) {
        if (template.getRemoteDrives() == null) {
            return false;
        } else {
            return !template.getRemoteDrives().isEmpty();
        }
    }

    Blade findBlade(RequestedNode template) throws AllocationException {
        Collection<Blade> blades = repository.getAllByFlag(Blade.class, Blade.IS_ALLOCATED, false);
        Set<Location> locations = collectDistinctLocationsFromTemplate(template);
        blades = bladeMatcher.matches(template, locations, blades);

        if (blades.isEmpty()) {
            throw new AllocationException("No assets available for allocation!");
        }

        return blades.iterator().next();
    }

    /**
     * Verifies if template is realizable by NodeComposer.
     *
     * @param template
     * @throws AllocationException
     */
    private void verifyTemplate(RequestedNode template) throws AllocationException {
        if (!canBeRealizedBySingleBlade(template)) {
            throw new AllocationException("Allocation of assets on multiple locations is not supported");
        }
        if (isRemoteDriveRequired(template) && template.getRemoteDrives().size() > 1) {
            throw new AllocationException("Allocation of more than one remote drive is not supported");
        }
    }

    private Set<Location> collectDistinctLocationsFromTemplate(RequestedNode template) {
        //TODO: handle partial locations
        Set<Location> locations = new HashSet<>();
        locations.addAll(getLocations(template.getProcessors()));
        locations.addAll(getLocations(template.getMemory()));
        locations.addAll(getLocations(template.getLocalDrives()));
        locations.addAll(getLocations(template.getNetworkInterfaces()));
        return locations;
    }

    /**
     * Checks if template can be realized by a single Blade - which implies that
     * all template components, except remote drives, should have the same location specified.
     * If template component have no location specified, it's assumed it can be any location.
     *
     * @param template
     * @return
     */
    private boolean canBeRealizedBySingleBlade(RequestedNode template) {
        Set<Location> locations = collectDistinctLocationsFromTemplate(template);
        return locations.size() <= 1;
    }

    private List<Location> getLocations(List<? extends Localizable> localizableList) {
        if (localizableList == null) {
            return emptyList();
        } else {
            return localizableList.stream()
                .filter(localizable -> localizable.getLocation() != null)
                    .map(Localizable::getLocation)
                    .collect(Collectors.toList());
        }
    }
}
