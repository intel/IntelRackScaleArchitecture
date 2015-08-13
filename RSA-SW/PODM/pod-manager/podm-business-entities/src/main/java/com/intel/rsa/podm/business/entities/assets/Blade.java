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
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.Power;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.assets.properties.Boot;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.common.utils.IterableHelper.singleOrNull;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.booleanProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class Blade extends ManagedAsset {

    public static final DomainObjectProperty<String> MANUFACTURER = stringProperty("manufacturer");
    public static final DomainObjectProperty<String> MODEL = stringProperty("model");
    public static final DomainObjectProperty<String> SERIAL_NUMBER = stringProperty("serialNumber");
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);
    public static final DomainObjectProperty<SystemType> SYSTEM_TYPE = enumProperty("systemType", SystemType.class);
    public static final DomainObjectProperty<Boolean> STORAGE_CAPABLE = booleanProperty("storageCapable");
    public static final DomainObjectProperty<Integer> STORAGE_CONTROLLER_COUNT = integerProperty("storageControllerCount");
    public static final DomainObjectProperty<Power> POWER = enumProperty("power", Power.class);
    public static final DomainObjectProperty<String> CURRENT_BIOS_VERSION = stringProperty("currentBiosVersion");

    public static final DomainObjectProperty<Integer> PROCESSORS_SOCKETS = integerProperty("processorSocket");
    public static final DomainObjectProperty<String> PROCESSORS_FAMILY = stringProperty("processorsFamily");
    public static final DomainObjectProperty<State> PROCESSORS_STATE = enumProperty("processorsState", State.class);
    public static final DomainObjectProperty<Health> PROCESSORS_HEALTH = enumProperty("processorsHealth", Health.class);
    public static final DomainObjectProperty<Health> PROCESSORS_HEALTH_ROLLUP = enumProperty("processorsHealthRollup", Health.class);
    public static final DomainObjectProperty<Integer> PROCESSORS_COUNT = integerProperty("processorsCount");

    public static final DomainObjectProperty<Integer> TOTAL_SYSTEM_MEMORY_GB = integerProperty("totalSystemMemoryGB");
    public static final DomainObjectProperty<Integer> MEMORY_SOCKETS = integerProperty("memorySockets");
    public static final DomainObjectProperty<State> MEMORY_STATE = enumProperty("memoryState", State.class);
    public static final DomainObjectProperty<Health> MEMORY_HEALTH = enumProperty("memoryHealth", Health.class);
    public static final DomainObjectProperty<Health> MEMORY_HEALTH_ROLLUP = enumProperty("memoryHealthRollup", Health.class);

    public static final DomainObjectProperty<EnumStatus> ENUM_STATUS = enumProperty("enumStatus", EnumStatus.class);
    public static final DomainObjectProperty<Boolean> IS_ALLOCATED = booleanProperty("isAllocated");

    public ComputeModule getComputeModule() {
        try {
            return single(getLinked(AssetLink.CONTAINED_BY, ComputeModule.class));
        } catch (IllegalStateException e) {
            throw new IllegalStateException("Blade has to be contained by exactly one Compute Module", e);
        }
    }

    public Collection<Memory> getMemory() {
        return getLinked(AssetLink.CONTAINS, Memory.class);
    }

    public void setBoot(Boot boot) {
        link(AssetLink.CONTAINS, boot);
    }

    public Boot getBoot() {
        return singleOrNull(getLinked(AssetLink.CONTAINS, Boot.class));
    }

    public Collection<Processor> getProcessors() {
        return getLinked(AssetLink.CONTAINS, Processor.class);
    }

    public Processor getProcessor(Id id) {
        return DomainObjectHelper.findById(id, getProcessors());
    }

    public Collection<StorageController> getStorageControllers() {
        return getLinked(AssetLink.CONTAINS, StorageController.class);
    }

    public Collection<NetworkInterface> getEthernetInterfaces() {
        return getLinked(AssetLink.CONTAINS, NetworkInterface.class);
    }

    public StorageController getStorageController(Id id) {
        return DomainObjectHelper.findById(id, getStorageControllers());
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

    public SystemType getSystemType() {
        return getProperty(SYSTEM_TYPE);
    }

    public void setSystemType(SystemType systemType) {
        setProperty(SYSTEM_TYPE, systemType);
    }

    public Boolean isStorageCapable() {
        return getProperty(STORAGE_CAPABLE);
    }

    public void setStorageCapable(Boolean storageCapable) {
        setProperty(STORAGE_CAPABLE, storageCapable);
    }

    public Integer getStorageControllerCount() {
        return getProperty(STORAGE_CONTROLLER_COUNT);
    }

    public void setStorageControllerCount(Integer storageControllerCount) {
        setProperty(STORAGE_CONTROLLER_COUNT, storageControllerCount);
    }

    public Power getPower() {
        return getProperty(POWER);
    }

    public void setPower(Power power) {
        setProperty(POWER, power);
    }

    public String getCurrentBiosVersion() {
        return getProperty(CURRENT_BIOS_VERSION);
    }

    public void setCurrentBiosVersion(String currentBiosVersion) {
        setProperty(CURRENT_BIOS_VERSION, currentBiosVersion);
    }

    public Integer getProcessorSockets() {
        return getProperty(PROCESSORS_SOCKETS);
    }

    public void setProcessorSockets(Integer processorSockets) {
        setProperty(PROCESSORS_SOCKETS, processorSockets);
    }

    public String getProcessorsFamily() {
        return getProperty(PROCESSORS_FAMILY);
    }

    public void setProcessorsFamily(String processorsFamily) {
        setProperty(PROCESSORS_FAMILY, processorsFamily);
    }

    public Health getProcessorsHealth() {
        return getProperty(PROCESSORS_HEALTH);
    }

    public void setProcessorsHealth(Health processorsHealth) {
        setProperty(PROCESSORS_HEALTH, processorsHealth);
    }

    public Health getProcessorsHealthRollup() {
        return getProperty(PROCESSORS_HEALTH_ROLLUP);
    }

    public void setProcessorsHealthRollup(Health processorsHealthRollup) {
        setProperty(PROCESSORS_HEALTH_ROLLUP, processorsHealthRollup);
    }

    public int getProcessorsCount() {
        return getProperty(PROCESSORS_COUNT);
    }

    public void setProcessorsCount(int processorsCount) {
        setProperty(PROCESSORS_COUNT, processorsCount);
    }

    public State getProcessorsState() {
        return getProperty(PROCESSORS_STATE);
    }

    public void setProcessorsState(State processorsState) {
        setProperty(PROCESSORS_STATE, processorsState);
    }

    public Integer getTotalSystemMemoryGB() {
        return getProperty(TOTAL_SYSTEM_MEMORY_GB);
    }

    public void setTotalSystemMemoryGB(Integer totalSystemMemoryGB) {
        setProperty(TOTAL_SYSTEM_MEMORY_GB, totalSystemMemoryGB);
    }

    public Integer getMemorySockets() {
        return getProperty(MEMORY_SOCKETS);
    }

    public void setMemorySockets(Integer memorySockets) {
        setProperty(MEMORY_SOCKETS, memorySockets);
    }

    public Health getMemoryHealth() {
        return getProperty(MEMORY_HEALTH);
    }

    public void setMemoryHealth(Health memoryHealth) {
        setProperty(MEMORY_HEALTH, memoryHealth);
    }

    public Health getMemoryHealthRollup() {
        return getProperty(MEMORY_HEALTH_ROLLUP);
    }

    public void setMemoryHealthRollup(Health memoryHealthRollup) {
        setProperty(MEMORY_HEALTH_ROLLUP, memoryHealthRollup);
    }

    public State getMemoryState() {
        return getProperty(MEMORY_STATE);
    }

    public void setMemoryState(State memoryState) {
        setProperty(MEMORY_STATE, memoryState);
    }

    public EnumStatus getEnumStatus() {
        return getProperty(ENUM_STATUS);
    }

    public void setEnumStatus(EnumStatus enumStatus) {
        setProperty(ENUM_STATUS, enumStatus);
    }

    public Boolean isAllocated() {
        return getProperty(IS_ALLOCATED);
    }

    public void setAllocated(Boolean allocated) {
        setProperty(IS_ALLOCATED, allocated);
    }

    public ExternalService getService() {
        return single(getLinked(OWNED_BY, ExternalService.class));
    }
}
