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

package com.intel.rsa.client.rss;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.LinkName;
import com.intel.rsa.client.ResourceImpl;
import com.intel.rsa.client.RsaNetworkInterfaceImpl;
import com.intel.rsa.client.RsaNetworkServiceImpl;
import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.RsaNetworkInterface;
import com.intel.rsa.client.api.reader.RsaNetworkService;
import com.intel.rsa.client.api.reader.rss.RssStorageManager;
import com.intel.rsa.client.dto.ManagerLinksDto;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.client.psme.PsmeConsoleImpl;
import com.intel.rsa.client.psme.dto.FirmwareDto;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.ManagerType;
import com.intel.rsa.common.types.State;

import static java.util.Collections.emptyList;


@JsonIgnoreProperties(ignoreUnknown = true)
public final class RssStorageManagerImpl extends ResourceImpl implements RssStorageManager {
    @JsonProperty("Status")
    private StatusDto status = new StatusDto();

    @JsonProperty ("Firmware")
    private FirmwareDto firmware;

    @JsonProperty("ManagerType")
    private ManagerType type;

    @JsonProperty("Links")
    private ManagerLinksDto links;

    @JsonProperty ("GraphicalConsole")
    private PsmeConsoleImpl graphicalConsole;

    @JsonProperty ("SerialConsole")
    private PsmeConsoleImpl serialConsole;

    @JsonProperty ("CommandShell")
    private PsmeConsoleImpl commandShell;

    @Override
    public ManagerType getType() {
        return type;
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
    public PsmeConsoleImpl getGraphicalConsole() {
        return graphicalConsole;
    }

    @Override
    public PsmeConsoleImpl getSerialConsole() {
        return serialConsole;
    }

    @Override
    public PsmeConsoleImpl getCommandShell() {
        return commandShell;
    }

    @Override
    public String getFirmwareVersion() {
        return firmware.getCurrentVersion().getVersionString();
    }

    @LinkName("networkService")
    @Override
    public ResourceSupplier<RsaNetworkService> getNetworkService() {
        if (links.getNetworkService() == null) {
            return null;
        }

        return toSupplier(links.getNetworkService(), (Class) RsaNetworkServiceImpl.class);
    }

    @LinkName("simpleNetwork")
    @Override
    public Iterable<ResourceSupplier<RsaNetworkInterface>> getNetworkInterfaces() throws RsaApiReaderException {
        if (links.getNetworkInterfaces() == null) {
           return emptyList();
        }

        return processCollectionResource(links.getNetworkInterfaces(), (Class) RsaNetworkInterfaceImpl.class);
    }
}
