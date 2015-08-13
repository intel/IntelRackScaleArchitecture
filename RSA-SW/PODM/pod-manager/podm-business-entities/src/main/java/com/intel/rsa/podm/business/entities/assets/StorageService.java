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

import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.assets.links.StorageLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.net.URI;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Dependent
@Transactional(REQUIRED)
public class StorageService extends ManagedAsset implements Discoverable {
    public static final DomainObjectProperty<EnumStatus> ENUM_STATUS = enumProperty("enumStatus", EnumStatus.class);
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public Health getHealth() {
        return getProperty(HEALTH);
    }

    public void setHealth(Health health) {
        setProperty(HEALTH, health);
    }

    public Health getHealthRollup() {
        return getProperty(HEALTH_ROLLUP);
    }

    public void setHealthRollup(Health healthRollup) {
        setProperty(HEALTH_ROLLUP, healthRollup);
    }

    public EnumStatus getEnumStatus() {
        return getProperty(ENUM_STATUS);
    }

    public void setEnumStatus(EnumStatus enumStatus) {
        setProperty(ENUM_STATUS, enumStatus);
    }

    public Collection<PhysicalDrive> getPhysicalDrives() {
        return getLinked(AssetLink.CONTAINS, PhysicalDrive.class);
    }

    public Collection<LogicalDrive> getLogicalDrives() {
        return getLinked(AssetLink.CONTAINS, LogicalDrive.class);
    }

    public void addPhysicalDrive(PhysicalDrive physicalDrive) {
        link(StorageLink.CONTAINS, physicalDrive);
    }

    public void addLogicalDrive(LogicalDrive logicalDrive) {
        link(StorageLink.CONTAINS, logicalDrive);
    }

    public void addRemoteTarget(RemoteTarget remoteTarget) {
        link(StorageLink.CONTAINS, remoteTarget);
    }

    public Collection<RemoteTarget> getRemoteTargets() {
        return getLinked(StorageLink.CONTAINS, RemoteTarget.class);
    }

    public ExternalService getService() {
        return single(getLinked(OWNED_BY, ExternalService.class));
    }

    @Override
    public URI getSourceUri() {
        return getProperty(SOURCE_URI);
    }

    @Override
    public void setSourceUri(URI sourceUri) {
        setProperty(SOURCE_URI, sourceUri);
    }
}
