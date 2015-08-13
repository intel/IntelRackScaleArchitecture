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
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.StorageControllerInterface;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;

/**
 * Storage controller asset.
 */
@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class StorageController extends Asset {
    public static final DomainObjectProperty<StorageControllerInterface> CONTROLLER_INTERFACE =
            enumProperty("controllerInterface", StorageControllerInterface.class);
    public static final DomainObjectProperty<Integer> DRIVE_COUNT = integerProperty("driveCount");
    public static final DomainObjectProperty<State> STATE = enumProperty("State", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("Health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("HealthRollup", Health.class);

    public StorageControllerInterface getControllerInterface() {
        return getProperty(CONTROLLER_INTERFACE);
    }

    public void setControllerInterface(StorageControllerInterface storageControllerInterface) {
        setProperty(CONTROLLER_INTERFACE, storageControllerInterface);
    }

    public int getDriveCount() {
        return getProperty(DRIVE_COUNT);
    }

    public void setDriveCount(int driveCount) {
        setProperty(DRIVE_COUNT, driveCount);
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

    public void setHealthRollup(Health healthRollup) {
        setProperty(HEALTH_ROLLUP, healthRollup);
    }

    public Collection<Drive> getDrives() {
        return getLinked(AssetLink.CONTAINS, Drive.class);
    }

    public Drive getDrive(Id id) {
        return DomainObjectHelper.findById(id, getDrives());
    }

    public Blade getBlade() {
        return single(getLinked(AssetLink.CONTAINED_BY, Blade.class));
    }
}
