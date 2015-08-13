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

package com.intel.rsa.podm.business.entities.assets;

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.VolumeMode;
import com.intel.rsa.common.types.VolumeType;
import com.intel.rsa.podm.business.entities.assets.links.StorageLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.net.URI;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.common.utils.IterableHelper.singleOrNull;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.booleanProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

/**
 * Logical Drive asset.
 */
@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class LogicalDrive extends ManagedAsset implements Discoverable {
    public static final DomainObjectProperty<VolumeMode> MODE = enumProperty("mode", VolumeMode.class);
    public static final DomainObjectProperty<VolumeType> TYPE = enumProperty("type", VolumeType.class);
    public static final DomainObjectProperty<Boolean> WRITE_PROTECTED = booleanProperty("writeProtected");
    public static final DomainObjectProperty<Boolean> SNAPSHOT = booleanProperty("snapshot");
    public static final DomainObjectProperty<Integer> CAPACITY_GB = integerProperty("capacityGb");
    public static final DomainObjectProperty<String> ASSET_TAG = stringProperty("assetTag");
    public static final DomainObjectProperty<String> IMAGE = stringProperty("image");
    public static final DomainObjectProperty<Boolean> BOOTABLE = booleanProperty("bootable");
    public static final DomainObjectProperty<State> STATE = enumProperty("State", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("Health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);

    public VolumeMode getMode() {
        return getProperty(MODE);
    }

    public void setMode(VolumeMode volumeMode) {
        setProperty(MODE, volumeMode);
    }

    public Boolean getWriteProtected() {
        return getProperty(WRITE_PROTECTED);
    }

    public void setWriteProtected(Boolean writeProtected) {
        setProperty(WRITE_PROTECTED, writeProtected);
    }

    public Boolean getSnapshot() {
        return getProperty(SNAPSHOT);
    }

    public void setSnapshot(Boolean snapshot) {
        setProperty(SNAPSHOT, snapshot);
    }

    public int getCapacityGb() {
        return getProperty(CAPACITY_GB);
    }

    public void setCapacityGb(int capacityGb) {
        setProperty(CAPACITY_GB, capacityGb);
    }

    public VolumeType getType() {
        return getProperty(TYPE);
    }

    public void setType(VolumeType volumeType) {
        setProperty(TYPE, volumeType);
    }

    public String getAssetTag() {
        return getProperty(ASSET_TAG);
    }

    public void setAssetTag(String assetTag) {
        setProperty(ASSET_TAG, assetTag);
    }

    public String getImage() {
        return getProperty(IMAGE);
    }

    public void setImage(String image) {
        setProperty(IMAGE, image);
    }

    public Boolean getBootable() {
        return getProperty(BOOTABLE);
    }

    public void setBootable(Boolean bootable) {
        setProperty(BOOTABLE, bootable);
    }

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public Health getHealth() {
        return getProperty(HEALTH);
    }

    public Health getHealthRollup() {
        return getProperty(HEALTH_ROLLUP);
    }

    public void setHealthRollup(Health healthRollup) {
        setProperty(HEALTH_ROLLUP, healthRollup);
    }

    public void setHealth(Health health) {
        setProperty(HEALTH, health);
    }

    public void addPhysicalDrive(PhysicalDrive physicalDrive) {
        link(StorageLink.USES, physicalDrive);
    }

    public Collection<PhysicalDrive> getPhysicalDrives() {
        return getLinked(StorageLink.USES, PhysicalDrive.class);
    }

    public void setMasterDrive(LogicalDrive masterDrive) {
        link(StorageLink.MASTERED_BY, masterDrive);
    }

    public LogicalDrive getMasterDrive() {
        return singleOrNull(getLinked(StorageLink.MASTERED_BY, LogicalDrive.class));
    }

    public void use(LogicalDrive logicalDrive) {
        link(StorageLink.USES, logicalDrive);
    }

    public Collection<LogicalDrive> getUsedLogicalDrives() {
        return getLinked(StorageLink.USES, LogicalDrive.class);
    }

    public Collection<LogicalDrive> getUsedBy() {
        return getLinked(StorageLink.USED_BY, LogicalDrive.class);
    }

    public StorageService getStorageService() {
        return single(getLinked(StorageLink.CONTAINED_BY, StorageService.class));
    }

    @Override
    public URI getSourceUri() {
        return getProperty(SOURCE_URI);
    }

    @Override
    public void setSourceUri(URI sourceUri) {
        setProperty(SOURCE_URI, sourceUri);
    }

    public Collection<RemoteTarget> getRemoteTargets() {
        return getLinked(StorageLink.USED_BY, RemoteTarget.class);
    }
}
