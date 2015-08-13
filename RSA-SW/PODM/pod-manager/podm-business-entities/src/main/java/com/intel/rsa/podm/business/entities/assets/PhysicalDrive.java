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

import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.StorageControllerInterface;
import com.intel.rsa.podm.business.entities.assets.links.StorageLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.net.URI;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

/**
 * Drive asset.
 */
@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class PhysicalDrive extends ManagedAsset implements Discoverable {
    public static final DomainObjectProperty<StorageControllerInterface> CONTROLLER_INTERFACE =
            enumProperty("controllerInterface", StorageControllerInterface.class);
    public static final DomainObjectProperty<Integer> CAPACITY_GB = integerProperty("capacityGb");
    public static final DomainObjectProperty<DriveType> TYPE = enumProperty("type", DriveType.class);
    public static final DomainObjectProperty<Integer> RPM = integerProperty("rpm");
    public static final DomainObjectProperty<String> MANUFACTURER = stringProperty("manufacturer");
    public static final DomainObjectProperty<String> MODEL = stringProperty("model");
    public static final DomainObjectProperty<String> SERIAL_NUMBER = stringProperty("serialNumber");
    public static final DomainObjectProperty<State> STATE = enumProperty("State", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("Health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("HealthRollup", Health.class);

    public StorageControllerInterface getControllerInterface() {
        return getProperty(CONTROLLER_INTERFACE);
    }

    public void setControllerInterface(StorageControllerInterface storageControllerInterface) {
        setProperty(CONTROLLER_INTERFACE, storageControllerInterface);
    }

    public int getCapacityGb() {
        return getProperty(CAPACITY_GB);
    }

    public void setCapacityGb(int capacityGb) {
        setProperty(CAPACITY_GB, capacityGb);
    }

    public DriveType getType() {
        return getProperty(TYPE);
    }

    public void setType(DriveType driveType) {
        setProperty(TYPE, driveType);
    }

    public Integer getRpm() {
        return getProperty(RPM);
    }

    public void setRpm(Integer rpm) {
        setProperty(RPM, rpm);
    }

    public String getManufacturer() {
        return getProperty(MANUFACTURER);
    }

    public void setManufacturer(String manufacturer) {
        setProperty(MANUFACTURER, manufacturer);
    }

    public String getModel() {
        return getProperty(MODEL);
    }

    public void setModel(String model) {
        setProperty(MODEL, model);
    }

    public String getSerialNumber() {
        return getProperty(SERIAL_NUMBER);
    }

    public void setSerialNumber(String serialNumber) {
        setProperty(SERIAL_NUMBER, serialNumber);
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

    public StorageService getStorageService() {
        return single(getLinked(StorageLink.CONTAINED_BY, StorageService.class));
    }

    public Collection<LogicalDrive> getUsedBy() {
        return getLinked(StorageLink.USED_BY, LogicalDrive.class);
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
