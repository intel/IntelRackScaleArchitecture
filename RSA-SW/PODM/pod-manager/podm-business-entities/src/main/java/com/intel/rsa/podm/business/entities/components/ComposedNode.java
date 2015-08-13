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

package com.intel.rsa.podm.business.entities.components;

import com.intel.rsa.common.types.ComposedNodeState;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.podm.business.entities.assets.Blade;
import com.intel.rsa.podm.business.entities.assets.Drive;
import com.intel.rsa.podm.business.entities.assets.Manager;
import com.intel.rsa.podm.business.entities.assets.Memory;
import com.intel.rsa.podm.business.entities.assets.NetworkInterface;
import com.intel.rsa.podm.business.entities.assets.Processor;
import com.intel.rsa.podm.business.entities.assets.RemoteTarget;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.assets.links.ComposedNodeLink;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.time.OffsetDateTime;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.singleOrNull;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.offsetDateTimeProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class ComposedNode extends DomainObject {

    public static final DomainObjectProperty<String> NAME = stringProperty("name");
    public static final DomainObjectProperty<String> DESCRIPTION = stringProperty("description");
    public static final DomainObjectProperty<OffsetDateTime> MODIFIED = offsetDateTimeProperty("modified");
    public static final DomainObjectProperty<ComposedNodeState> NODE_STATE = enumProperty("composedNodeState", ComposedNodeState.class);
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<SystemType> SYSTEM_TYPE = enumProperty("systemType", SystemType.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);

    public String getName() {
        return getProperty(NAME);
    }

    public void setName(String name) {
        setProperty(NAME, name);
    }

    public String getDescription() {
        return getProperty(DESCRIPTION);
    }

    public void setDescription(String description) {
        setProperty(DESCRIPTION, description);
    }

    public OffsetDateTime getModified() {
        return getProperty(MODIFIED);
    }

    public void updateModified() {
        setProperty(MODIFIED, OffsetDateTime.now());
    }

    public SystemType getSystemType() {
        return getProperty(SYSTEM_TYPE);
    }

    public void setSystemType(SystemType systemType) {
        setProperty(SYSTEM_TYPE, systemType);
    }

    public ComposedNodeState getComposedNodeState() {
        return getProperty(NODE_STATE);
    }

    public void setComposedNodeState(ComposedNodeState composedNodeState) {
        setProperty(NODE_STATE, composedNodeState);
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

    public void setHealth(Health health) {
        setProperty(HEALTH, health);
    }

    public Health getHealthRollup() {
        return getProperty(HEALTH_ROLLUP);
    }

    public void setHealthRollup(Health health) {
        setProperty(HEALTH_ROLLUP, health);
    }

    public Collection<Manager> getManagers() {
        return getLinked(AssetLink.MANAGED_BY);
    }

    public void linkManager(Manager manager) {
        link(AssetLink.MANAGED_BY, manager);
    }

    public Blade getBlade() {
        return singleOrNull(getLinked(ComposedNodeLink.INCLUDES, Blade.class));
    }

    public void linkBlade(Blade blade) {
        link(ComposedNodeLink.INCLUDES, blade);
    }

    public Collection<Processor> getProcessors() {
        return getLinked(ComposedNodeLink.INCLUDES, Processor.class);
    }

    public void linkProcessor(Processor processor) {
        link(ComposedNodeLink.INCLUDES, processor);
    }

    public Collection<Memory> getMemory() {
        return getLinked(ComposedNodeLink.INCLUDES, Memory.class);
    }

    public void linkMemory(Memory memory) {
        link(ComposedNodeLink.INCLUDES, memory);
    }

    public Collection<RemoteTarget> getRemoteTargets() {
        return getLinked(ComposedNodeLink.INCLUDES, RemoteTarget.class);
    }

    public void linkRemoteTarget(RemoteTarget remoteTarget) {
        link(ComposedNodeLink.INCLUDES, remoteTarget);
    }

    public Collection<Drive> getLocalDrives() {
        return getLinked(ComposedNodeLink.INCLUDES, Drive.class);
    }

    public void linkLocalDrive(Drive drive) {
        link(ComposedNodeLink.INCLUDES, drive);
    }

    public Collection<NetworkInterface> getNetworkInterfaces() {
        return getLinked(ComposedNodeLink.INCLUDES, NetworkInterface.class);
    }

    public void linkNetworkInterface(NetworkInterface nic) {
        link(ComposedNodeLink.INCLUDES, nic);
    }
}
