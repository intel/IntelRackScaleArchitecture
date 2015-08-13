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

package com.intel.rsa.client.api.reader.psme;

import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Power;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.common.types.locations.Location;

/**
 * Blade resource details provided by PSME.
 */
public interface PsmeBlade extends PsmeManagedResource {
    String getModel();
    String getManufacturer();
    String getSerialNumber();
    Location getLocation();
    State getState();
    Health getHealth();
    Health getHealthRollup();

    Power getPower();

    boolean isStorageCapable();

    Integer getStorageControllerCount();

    SystemType getSystemType();

    Integer getProcessorSockets();

    String getProcessorsFamily();

    Health getProcessorsHealth();

    Health getProcessorsHealthRollup();

    State getProcessorsState();

    int getProcessorsCount();

    Integer getTotalSystemMemoryGB();

    Integer getMemorySockets();

    State getMemoryState();

    Health getMemoryHealth();

    Health getMemoryHealthRollup();

    String getCurrentBiosVersion();

    Iterable<ResourceSupplier<PsmeProcessor>> getProcessors() throws RsaApiReaderException;

    Iterable<ResourceSupplier<PsmeStorageController>> getStorageControllers() throws RsaApiReaderException;

    Iterable<ResourceSupplier<PsmeComputeModule>> getChassis() throws RsaApiReaderException;

    Iterable<ResourceSupplier<PsmeMemory>> getMemory() throws RsaApiReaderException;

    PsmeBoot getBoot();

    Iterable<ResourceSupplier<PsmeNetworkInterface>> getEthernetInterfaces() throws RsaApiReaderException;

    EnumStatus getEnumStatus();
}
