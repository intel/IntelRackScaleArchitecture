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
import com.intel.rsa.podm.business.entities.assets.links.CollectionLink;
import com.intel.rsa.podm.business.entities.assets.links.StorageLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.net.URI;
import java.util.Collection;
import java.util.List;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.booleanProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class RemoteTarget extends ManagedAsset implements Discoverable {

    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);
    public static final DomainObjectProperty<EnumStatus> ENUM_STATUS = enumProperty("enumStatus", EnumStatus.class);
    public static final DomainObjectProperty<String> TYPE = stringProperty("type");
    public static final DomainObjectProperty<Boolean> IS_ALLOCATED = booleanProperty("isAllocated");

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

    public String getType() {
        return getProperty(TYPE);
    }

    public void setType(String type) {
        setProperty(TYPE, type);
    }

    @Override
    public URI getSourceUri() {
        return getProperty(SOURCE_URI);
    }

    @Override
    public void setSourceUri(URI sourceUri) {
        setProperty(SOURCE_URI, sourceUri);
    }

    public Boolean isAllocated() {
        return getProperty(IS_ALLOCATED);
    }

    public void setAllocated(Boolean allocated) {
        setProperty(IS_ALLOCATED, allocated);
    }

    public Collection<RemoteTargetIscsiAddress> getRemoteTargetIscsiAddresses() {
        return getLinked(CollectionLink.COLLECTS, RemoteTargetIscsiAddress.class);
    }

    public void linkRemoteTargetIscsiAddresses(RemoteTargetIscsiAddress iscsiAddress) {
        link(CollectionLink.COLLECTS, iscsiAddress);
    }

    public void setAddresses(List<RemoteTargetIscsiAddress> iscsiAddresses) {
        iscsiAddresses.forEach(address -> link(CollectionLink.COLLECTS, address));
    }

    public Collection<RemoteTargetIscsiInitiator> getRemoteTargetIscsiInitiators() {
        return getLinked(CollectionLink.COLLECTS, RemoteTargetIscsiInitiator.class);
    }

    public void linkRemoteTargetIscsiInitiators(RemoteTargetIscsiInitiator iscsiInitiator) {
        link(CollectionLink.COLLECTS, iscsiInitiator);
    }

    public void setInitiator(List<RemoteTargetIscsiInitiator> iscsiInitiators) {
        iscsiInitiators.forEach(initiator -> link(CollectionLink.COLLECTS, initiator));
    }

    public Collection<LogicalDrive> getLogicalDrives() {
        return getLinked(StorageLink.USES, LogicalDrive.class);
    }

    public StorageService getStorageService() {
        return single(getLinked(StorageLink.CONTAINED_BY, StorageService.class));
    }

    public void addLogicalDrive(LogicalDrive logicalDrive) {
        link(StorageLink.USES, logicalDrive);
    }
}
