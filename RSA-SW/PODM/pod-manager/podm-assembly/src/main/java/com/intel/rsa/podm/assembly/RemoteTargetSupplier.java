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

import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.VolumeMode;
import com.intel.rsa.podm.business.dto.RequestedMasterDrive;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;
import com.intel.rsa.podm.business.entities.DomainObjectNotFoundException;
import com.intel.rsa.podm.business.entities.assets.LogicalDrive;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.RemoteTargetIscsiAddress;
import com.intel.rsa.podm.business.entities.assets.StorageService;
import com.intel.rsa.podm.business.entities.base.DomainObjectRepository;
import com.intel.rsa.podm.business.entities.components.ComposedNode;
import com.intel.rsa.podm.business.services.context.Context;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.math.BigDecimal;
import java.net.URI;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Objects;

import static com.intel.rsa.common.types.RemoteStorageType.ISCSI;
import static com.intel.rsa.podm.business.entities.assets.RemoteTargetIscsiAddress.TARGET_IQN;
import static java.util.Objects.isNull;
import static java.util.Objects.nonNull;
import static java.util.stream.Collectors.toList;

@Dependent
public class RemoteTargetSupplier {

    @Inject
    private DomainObjectRepository repository;

    @Inject
    private RemoteTargetCreationDelegate delegate;

    /**
     * Initializes creation of remote target based on RequestedRemoteDrive
     * on any LogicalVolumeGroup with enough free capacity to accommodate such target.
     *
     * @param node
     * @param template
     * @throws AllocationException if no LVG with enough capacity was found
     */

    // TODO: There is currently no cleanup if logical drive creation succeeds and target creation fails
    public void initializeNewTargetCreation(ComposedNode node, RequestedRemoteDrive template) throws AllocationException {
        validateDriveTemplate(template);
        StorageService storageService = getStorageService(template);
        List<LogicalDrive> matchingLvgs = findLvgsMatchingFreeSpaceRequirement(storageService, template.getCapacityGB());
        validateAvailibityOfLvg(matchingLvgs);
        LogicalDrive lvg = matchingLvgs.get(0);

        URI masterDriveSourceUri = extractMasterDriveSourceUri(template);

        delegate.createRemoteTarget(node, lvg, masterDriveSourceUri, template);
    }

    private URI extractMasterDriveSourceUri(RequestedRemoteDrive template) throws AllocationException {
        RequestedMasterDrive masterTemplate = template.getMaster();
        if (masterTemplate == null) {
            return null;
        }

        validateMasterDriveTemplate(masterTemplate);
        Context masterDriveContext = masterTemplate.getAddress();

        Id logicalDriveId = masterDriveContext.getId();
        Id serviceId = masterDriveContext.getParent().getId();

        LogicalDrive logicalDrive = repository.get(LogicalDrive.class, logicalDriveId);
        if (!logicalDrive.getStorageService().getId().equals(serviceId)) {
            throw new AllocationException("Unable to create target, Invalid Logical Drive specified as Master Drive");
        }

        return logicalDrive.getSourceUri();
    }

    private void validateMasterDriveTemplate(RequestedMasterDrive master) throws AllocationException {
        boolean invalidMasterDrive = isNull(master.getAddress()) || isNull(master.getAddressType()) || isNull(master.getType());

        if (invalidMasterDrive) {
            throw new AllocationException("Unable to create target, Invalid Master Drive specified");
        }
    }

    private void validateAvailibityOfLvg(List<LogicalDrive> matchingLvgs) throws AllocationException {
        if (matchingLvgs.isEmpty()) {
            throw new AllocationException("Unable to create target, no LVGs available");
        }
    }

    private List<LogicalDrive> findLvgsMatchingFreeSpaceRequirement(StorageService storageService, BigDecimal capacityGB) {
        List<LogicalDrive> result = new ArrayList<>();

        List<LogicalDrive> logicalVolumeGroups = getLogicalVolumeGroups(storageService);

        for (LogicalDrive lvg : logicalVolumeGroups) {
            BigDecimal freeSpace = getFreeSpaceOnLogicalVolumeGroup(lvg);
            if (capacityGB.compareTo(freeSpace) <= 0) {
                result.add(lvg);
            }
        }

        return result;
    }

    private StorageService getStorageService(RequestedRemoteDrive template) throws AllocationException {
        Context logicalDriveContext = template.getMaster().getAddress();
        Context storageServiceContext = logicalDriveContext.getParent();
        Id storageServiceId = storageServiceContext.getId();
        try {
            return repository.get(StorageService.class, storageServiceId);
        } catch (DomainObjectNotFoundException e) {
            throw new AllocationException("Could not find storage service for master drive", e);
        }
    }

    private List<LogicalDrive> getLogicalVolumeGroups(StorageService storageService) {
        return storageService.getLogicalDrives()
                             .stream()
                             .filter(ld -> ld.getMode().equals(VolumeMode.LVG))
                             .collect(toList());
    }

    private void validateDriveTemplate(RequestedRemoteDrive template) throws AllocationException {
        BigDecimal requestedCapacity = template.getCapacityGB();
        if (requestedCapacity == null) {
            throw new AllocationException("Unable to create new target, capacity for new drive not defined in template");
        }
    }

    private BigDecimal getFreeSpaceOnLogicalVolumeGroup(LogicalDrive logicalVolumeGroup) {
        Collection<LogicalDrive> usedBy = logicalVolumeGroup.getUsedBy();
        long freeSpaceGB = logicalVolumeGroup.getCapacityGb();

        for (LogicalDrive logicalDrive : usedBy) {
            freeSpaceGB -= logicalDrive.getCapacityGb();
        }

        return BigDecimal.valueOf(freeSpaceGB);
    }

    /**
     * Method finds a specific target based on its specified address.
     *
     * @param remoteDriveTemplate
     * @return
     * @throws AllocationException
     */
    public RemoteTarget findRemoteTarget(RequestedRemoteDrive remoteDriveTemplate)
            throws AllocationException {
        List<RemoteTarget> remoteTargets = repository.getAllByFlag(RemoteTarget.class, RemoteTarget.IS_ALLOCATED, false);
        for (RemoteTarget remoteTarget : remoteTargets) {
            if (isMatching(remoteTarget, remoteDriveTemplate)) {
                return remoteTarget;
            }
        }

        // TODO add implementation
        throw new AllocationException("Specified remote target (" + remoteDriveTemplate.getAddress() + ") was not found");
    }

    private boolean isMatching(RemoteTarget target, RequestedRemoteDrive template) {
        for (RemoteTargetIscsiAddress address : target.getRemoteTargetIscsiAddresses()) {
            if (Objects.equals(address.getTargetIqn(), template.getAddress())) {
                return true;
            }
        }

        return false;
    }

    /**
     * Method checks if RemoteDriveTemplate defines an existing target to be allocated, or a new one to be created.
     *
     * @param remoteDriveTemplate
     * @return
     */
    public boolean isDefiningExistingTarget(RequestedRemoteDrive remoteDriveTemplate) {
        return ISCSI.equals(remoteDriveTemplate.getAddressType())
                && nonNull(remoteDriveTemplate.getAddress())
                && isNull(remoteDriveTemplate.getMaster());
    }

    /**
     * Checks if target for provided RequestedRemoteDrive already exists
     * @param requestedRemoteDrive
     * @return
     */
    public boolean targetExists(RequestedRemoteDrive requestedRemoteDrive) {
        String address = requestedRemoteDrive.getAddress();
        int targetCount = repository.getAllByProperty(RemoteTargetIscsiAddress.class, TARGET_IQN, address).size();
        return targetCount != 0;
    }
}
