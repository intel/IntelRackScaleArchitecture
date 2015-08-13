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
import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.rss.RssDrive;
import com.intel.rsa.client.api.reader.rss.RssLogicalDrive;
import com.intel.rsa.client.api.reader.rss.RssRemoteTarget;
import com.intel.rsa.client.api.reader.rss.RssStorageManager;
import com.intel.rsa.client.api.reader.rss.RssStorageService;
import com.intel.rsa.client.dto.LocationDto;
import com.intel.rsa.client.dto.ODataId;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;

import java.util.List;

@JsonIgnoreProperties(ignoreUnknown = true)
public final class RssStorageServiceImpl extends ResourceImpl implements RssStorageService {
    @JsonProperty("Status")
    private StatusDto status = new StatusDto();

    @JsonProperty ("EnumStatus")
    private EnumStatus enumStatus;

    @JsonProperty("Location")
    private LocationDto location;

    @JsonProperty("Links")
    private Links links = new Links();

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

    public EnumStatus getEnumStatus() {
        return enumStatus;
    }

    @Override
    public Location getLocation() {
        return new Location(location.toMap());
    }

    @LinkName("drives")
    @Override
    public Iterable<ResourceSupplier<RssDrive>> getDrives() throws RsaApiReaderException {
        return processCollectionResource(links.drives, (Class) RssDriveImpl.class);
    }

    @LinkName("logicalDrives")
    @Override
    public Iterable<ResourceSupplier<RssLogicalDrive>> getLogicalDrives() throws RsaApiReaderException {
        return processCollectionResource(links.logicalDrives, (Class) RssLogicalDriveImpl.class);
    }

    @LinkName("targets")
    @Override
    public Iterable<ResourceSupplier<RssRemoteTarget>> getRemoteTargets() throws RsaApiReaderException {
        return processCollectionResource(links.targets, (Class) RssRemoteTargetImpl.class);
    }

    @LinkName("managedBy")
    @Override
    public Iterable<ResourceSupplier<RssStorageManager>> getManagers() throws RsaApiReaderException {
        return toSuppliers(links.managedBy, (Class) RssStorageManagerImpl.class);
    }

    @JsonIgnoreProperties(ignoreUnknown = true)
    private static final class Links {
        @JsonProperty("Drives")
        private ODataId drives;
        @JsonProperty("LogicalDrives")
        private ODataId logicalDrives;
        @JsonProperty("RemoteTargets")
        private ODataId targets;
        @JsonProperty("ManagedBy")
        private List<ODataId> managedBy;
    }
}
