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
import com.intel.rsa.client.RsaNetworkInterfaceImpl;
import com.intel.rsa.client.RsaNetworkServiceImpl;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.psme.PsmeConsole;
import com.intel.rsa.client.api.reader.psme.PsmeManager;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkInterface;
import com.intel.rsa.client.api.reader.psme.PsmeNetworkService;
import com.intel.rsa.client.psme.dto.FirmwareDto;
import com.intel.rsa.client.dto.ManagerLinksDto;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.ManagerType;
import com.intel.rsa.common.types.State;


public class PsmeManagerImpl extends ResourceImpl implements PsmeManager  {
    @JsonProperty ("Model")
    private String model;

    @JsonProperty ("Firmware")
    private FirmwareDto firmware;

    @JsonProperty ("Status")
    private StatusDto status;

    @JsonProperty ("ManagerType")
    private ManagerType managerType;

    @JsonProperty ("GraphicalConsole")
    private PsmeConsoleImpl graphicalConsole;

    @JsonProperty ("SerialConsole")
    private PsmeConsoleImpl serialConsole;

    @JsonProperty ("CommandShell")
    private PsmeConsoleImpl commandShell;

    @JsonProperty("Links")
    private ManagerLinksDto links;

    @Override
    public String getModel() {
        return model;
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
    public ManagerType getType() {
        return managerType;
    }

    @Override
    public String getFirmwareVersion() {
        return firmware.getCurrentVersion().getVersionString();
    }

    @Override
    public PsmeConsole getGraphicalConsole() {
        return graphicalConsole;
    }

    @Override
    public PsmeConsole getSerialConsole() {
        return serialConsole;
    }

    @Override
    public PsmeConsole getCommandShell() {
        return commandShell;
    }

    @Override
    @LinkName("networkService")
    public ResourceSupplier<PsmeNetworkService> getNetworkService() {
        return toSupplier(links.getNetworkService(), (Class) RsaNetworkServiceImpl.class);
    }

    @Override
    @LinkName("simpleNetwork")
    public Iterable<ResourceSupplier<PsmeNetworkInterface>> getNetworkInterfaces() throws RsaApiReaderException {
        return processCollectionResource(links.getNetworkInterfaces(), (Class) RsaNetworkInterfaceImpl.class);
    }
}
