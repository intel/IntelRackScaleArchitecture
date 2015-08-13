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

package com.intel.rsa.client.psme;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.LinkName;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.RsaNetworkInterfaceImpl;
import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.psme.PsmeBlade;
import com.intel.rsa.client.api.reader.psme.PsmeBoot;
import com.intel.rsa.client.api.reader.psme.PsmeComputeModule;
import com.intel.rsa.client.api.reader.psme.PsmeManager;
import com.intel.rsa.client.api.reader.psme.PsmeMemory;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkInterface;
import com.intel.rsa.client.api.reader.psme.PsmeProcessor;
import com.intel.rsa.client.api.reader.psme.PsmeStorageController;
import com.intel.rsa.client.dto.LocationDto;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.client.psme.dto.BiosDto;
import com.intel.rsa.client.psme.dto.BladeLinksDto;
import com.intel.rsa.client.psme.dto.MemoryDto;
import com.intel.rsa.client.psme.dto.ProcessorsDto;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Power;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.common.types.locations.Location;

import static com.intel.rsa.common.types.locations.Locations.bladeLocation;

public class PsmeBladeImpl extends ResourceImpl implements PsmeBlade {
    @JsonProperty("Model")
    private String model;

    @JsonProperty ("Manufacturer")
    private String manufacturer;

    @JsonProperty ("SerialNumber")
    private String serialNumber;

    @JsonProperty ("Status")
    private StatusDto status;

    @JsonProperty ("EnumStatus")
    private EnumStatus enumStatus;

    @JsonProperty ("Power")
    private Power power;

    @JsonProperty ("StorageCapable")
    private boolean storageCapable;

    @JsonProperty ("StorageControllersCount")
    private Integer storageControllersCount;

    @JsonProperty ("SystemType")
    private SystemType systemType;

    @JsonProperty ("Location")
    private LocationDto location;

    @JsonProperty ("Bios")
    private BiosDto bios;

    @JsonProperty("Processors")
    private ProcessorsDto processorsDto;

    @JsonProperty("Memory")
    private MemoryDto memoryDto;

    @JsonProperty("Boot")
    private PsmeBootImpl boot;

    @JsonProperty("Links")
    private BladeLinksDto links;

    @Override
    public String getModel() {
        return model;
    }

    @Override
    public String getManufacturer() {
        return manufacturer;
    }

    @Override
    public String getSerialNumber() {
        return serialNumber;
    }

    @Override
    public Location getLocation() {
        return bladeLocation(
                location.getPod(),
                location.getRack(),
                location.getDrawer(),
                location.getModule(),
                location.getBlade()
        );
    }

    @Override
    public State getState() {
        return status.getState();
    }

    @Override
    public Health getHealth() {
        return status.getHealth();
    }

    @Override
    public Health getHealthRollup() {
        return status.getHealthRollup();
    }

    @Override
    public Power getPower() {
        return power;
    }

    @Override
    public boolean isStorageCapable() {
        return storageCapable;
    }

    @Override
    public Integer getStorageControllerCount() {
        return storageControllersCount;
    }

    @Override
    public SystemType getSystemType() {
        return systemType;
    }

    @Override
    public String getProcessorsFamily() {
        return processorsDto.getProcessorFamily();
    }

    @Override
    public Integer getProcessorSockets() {
        return processorsDto.getProcessorSockets();
    }

    @Override
    public Health getProcessorsHealth() {
        return processorsDto.getStatusDto().getHealth();
    }

    @Override
    public Health getProcessorsHealthRollup() {
        return processorsDto.getStatusDto().getHealthRollup();
    }

    @Override
    public State getProcessorsState() {
        return processorsDto.getStatusDto().getState();
    }

    @Override
    public int getProcessorsCount() {
        return processorsDto.getCount();
    }

    @Override
    public Integer getTotalSystemMemoryGB() {
        return  memoryDto.getTotalSystemMemoryGB();
    }

    @Override
    public Integer getMemorySockets() {
        return memoryDto.getMemorySockets();
    }

    @Override
    public State getMemoryState() {
        return memoryDto.getStatusDto().getState();
    }

    @Override
    public Health getMemoryHealth() {
        return memoryDto.getStatusDto().getHealth();
    }

    @Override
    public Health getMemoryHealthRollup() {
        return memoryDto.getStatusDto().getHealthRollup();
    }

    @Override
    public String getCurrentBiosVersion() {
        return bios.getCurrentVersion().getVersionString();
    }

    @Override
    @LinkName("processors")
    public Iterable<ResourceSupplier<PsmeProcessor>> getProcessors() throws RsaApiReaderException {
        return processCollectionResource(links.getProcessors(), (Class) PsmeProcessorImpl.class);
    }

    @Override
    @LinkName("storageControllers")
    public Iterable<ResourceSupplier<PsmeStorageController>> getStorageControllers() throws RsaApiReaderException {
        return processCollectionResource(links.getStorageControllers(), (Class) PsmeStorageControllerImpl.class);
    }

    @Override
    @LinkName("managedBy")
    public Iterable<ResourceSupplier<PsmeManager>> getManagers() throws RsaApiReaderException {
        return toSuppliers(links.getManagers(), (Class) PsmeManagerImpl.class);
    }

    @Override
    public Iterable<ResourceSupplier<PsmeComputeModule>> getChassis() throws RsaApiReaderException {
        return toSuppliers(links.getChassis(), (Class) PsmeComputeModule.class);
    }

    @Override
    @LinkName("memory")
    public Iterable<ResourceSupplier<PsmeMemory>> getMemory() throws RsaApiReaderException {
        return processCollectionResource(links.getMemory(), (Class) PsmeMemoryImpl.class);
    }

    @Override
    public PsmeBoot getBoot() {
        return boot;
    }

    @Override
    @LinkName("simpleNetwork")
    public Iterable<ResourceSupplier<PsmeNetworkInterface>> getEthernetInterfaces() throws RsaApiReaderException {
        return processCollectionResource(links.getEthernetInterfaces(), (Class) RsaNetworkInterfaceImpl.class);
    }

    @Override
    public EnumStatus getEnumStatus() {
        return enumStatus;
    }
}
