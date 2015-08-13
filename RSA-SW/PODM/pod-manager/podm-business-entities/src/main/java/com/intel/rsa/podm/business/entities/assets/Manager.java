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
import com.intel.rsa.common.types.ManagerType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.assets.links.ManagerConsoleLink;
import com.intel.rsa.podm.business.entities.assets.properties.Console;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.net.URI;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.common.utils.IterableHelper.singleOrNull;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.booleanProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

/**
 * Manager asset.
 * TODO Split (StorageManager, RsaManager)
 */
@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class Manager extends Asset implements Discoverable {
    public static final DomainObjectProperty<String> MODEL = stringProperty("model");
    public static final DomainObjectProperty<ManagerType> TYPE = enumProperty("type", ManagerType.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<String> FIRMWARE_VERSION = stringProperty("firmwareVersion");
    public static final DomainObjectProperty<Boolean> STORAGE_MANAGER = booleanProperty("storageManager"); // TODO workaround

    @Override
    public URI getSourceUri() {
        return getProperty(SOURCE_URI);
    }

    @Override
    public void setSourceUri(URI sourceUri) {
        setProperty(SOURCE_URI, sourceUri);
    }

    public void setGraphicalConsole(Console console) {
        link(ManagerConsoleLink.GRAPHICAL_CONSOLE, console);
    }

    public Console getGraphicalConsole() {
        return singleOrNull(getLinked(ManagerConsoleLink.GRAPHICAL_CONSOLE));
    }

    public void setSerialConsole(Console console) {
        link(ManagerConsoleLink.SERIAL_CONSOLE, console);
    }

    public Console getSerialConsole() {
        return singleOrNull(getLinked(ManagerConsoleLink.SERIAL_CONSOLE));
    }

    public void setCommandShell(Console console) {
        link(ManagerConsoleLink.COMMAND_SHELL, console);
    }

    public Console getCommandShell() {
        return singleOrNull(getLinked(ManagerConsoleLink.COMMAND_SHELL));
    }

    public String getModel() {
        return getProperty(MODEL);
    }

    public void setModel(String model) {
        setProperty(MODEL, model);
    }

    public ManagerType getType() {
        return getProperty(TYPE);
    }

    public void setType(ManagerType type) {
        setProperty(TYPE, type);
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

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public String getFirmwareVersion() {
        return getProperty(FIRMWARE_VERSION);
    }

    public void setFirmwareVersion(String firmwareVersion) {
        setProperty(FIRMWARE_VERSION, firmwareVersion);
    }

    public boolean isStorageManager() {
        // TODO workaround
        Boolean storageManager = getProperty(STORAGE_MANAGER);
        return storageManager != null && storageManager;
    }

    public void setStorageManager(boolean storageManager) {
        setProperty(STORAGE_MANAGER, storageManager);
    }

    public Collection<Pod> getManagedPods() {
        return getAssetsOfClass(Pod.class);
    }

    public Collection<Drawer> getManagedDrawers() {
        return getAssetsOfClass(Drawer.class);
    }

    public Collection<ComputeModule> getManagedComputeModules() {
        return getAssetsOfClass(ComputeModule.class);
    }

    public Collection<FabricModule> getManagedFabricModules() {
        return getAssetsOfClass(FabricModule.class);
    }

    public Collection<Blade> getManagedBlades() {
        return getAssetsOfClass(Blade.class);
    }

    public Collection<Switch> getManagedSwitches() {
        return getAssetsOfClass(Switch.class);
    }

    public Collection<StorageService> getManagedServices() {
        return getAssetsOfClass(StorageService.class);
    }

    public Collection<RemoteTarget> getManagedRemoteTargets() {
        return getAssetsOfClass(RemoteTarget.class);
    }

    public Collection<LogicalDrive> getManagedLogicalDrives() {
        return getAssetsOfClass(LogicalDrive.class);
    }

    public NetworkService getNetworkService() {
        return single(getLinked(AssetLink.CONTAINS, NetworkService.class));
    }

    public Collection<NetworkInterface> getNetworkInterfaces() {
        return getLinked(AssetLink.CONTAINS, NetworkInterface.class);
    }

    public NetworkInterface getNetworkInterface(Id id) {
        return DomainObjectHelper.findById(id, getNetworkInterfaces());
    }

    public boolean manages(ManagedAsset asset) {
        return DomainObjectHelper.contains(asset, getManagedAssets());
    }

    public void setNetworkService(NetworkService networkService) {
        link(AssetLink.CONTAINS, networkService);
    }

    private <T extends ManagedAsset> Collection<T> getAssetsOfClass(Class<T> managedAssetClass) {
        return getLinked(AssetLink.MANAGES, managedAssetClass);
    }

    private Collection<ManagedAsset> getManagedAssets() {
        return getLinked(AssetLink.MANAGES);
    }

    public NetworkInterface addNetworkInterface() {
        return addDomainObject(AssetLink.CONTAINS, NetworkInterface.class);
    }

    public NetworkService addNetworkService() {
        return addDomainObject(AssetLink.CONTAINS, NetworkService.class);
    }
}
