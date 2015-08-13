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
import com.intel.rsa.client.dto.ODataId;
import com.intel.rsa.client.dto.StatusDto;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.VolumeMode;
import com.intel.rsa.common.types.VolumeType;

import java.util.List;

import static com.google.common.base.MoreObjects.toStringHelper;
import static java.util.Collections.emptyList;

public class RssLogicalDriveImpl extends ResourceImpl implements RssLogicalDrive {
    @JsonProperty("Type")
    private VolumeType type;
    @JsonProperty("Mode")
    private VolumeMode mode;
    @JsonProperty("Protected")
    private boolean writeProtected;
    @JsonProperty("AssetTag")
    private String assetTag;
    @JsonProperty("CapacityGB")
    private int capacityGb;
    @JsonProperty("Image")
    private String image;
    @JsonProperty("Bootable")
    private boolean bootable;
    @JsonProperty("Snapshot")
    private Boolean snapshot;
    @JsonProperty("Status")
    private StatusDto status = new StatusDto();
    @JsonProperty("Links")
    private Links links = new Links();

    @Override
    public VolumeType getType() {
        return type;
    }
    @Override
    public VolumeMode getMode() {
        return mode;
    }
    @Override
    public boolean isWriteProtected() {
        return writeProtected;
    }
    @Override
    public String getAssetTag() {
        return assetTag;
    }
    @Override
    public int getCapacityGb() {
        return capacityGb;
    }
    @Override
    public String getImage() {
        return image;
    }
    @Override
    public Boolean isBootable() {
        return bootable;
    }
    @Override
    public Boolean isSnapshot() {
        return snapshot;
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

    @LinkName("logicalDrives")
    @Override
    public Iterable<ResourceSupplier<RssLogicalDrive>> getLogicalDrives() throws RsaApiReaderException {
        return toSuppliers(links.logicalDrives, (Class) RssLogicalDriveImpl.class);
    }

    @LinkName("physicalDrives")
    @Override
    public Iterable<ResourceSupplier<RssDrive>> getPhysicalDrives() throws RsaApiReaderException {
        return toSuppliers(links.physicalDrives, (Class) RssDriveImpl.class);
    }

    @LinkName("masterDrive")
    @Override
    public ResourceSupplier<RssLogicalDrive> getMasterDrive() {
        // TODO masterDrive must be null or URI must be not null.
        if (links.masterDrive == null || links.masterDrive.toUri() == null) {
            return null;
        }

        return toSupplier(links.masterDrive, (Class) RssLogicalDrive.class);
    }

    @Override
    public Iterable<ResourceSupplier<RssLogicalDrive>> getUsedBy() throws RsaApiReaderException {
        return toSuppliers(links.usedBy, (Class) RssLogicalDriveImpl.class);
    }

    @Override
    public Iterable<ResourceSupplier<RssRemoteTarget>> getTargets() throws RsaApiReaderException {
        return toSuppliers(links.targets, (Class) RssRemoteTargetImpl.class);
    }

    @LinkName("managedBy")
    @Override
    public Iterable<ResourceSupplier<RssStorageManager>> getManagers() throws RsaApiReaderException {
        return toSuppliers(links.managedBy, (Class) RssStorageManagerImpl.class);
    }

    @Override
    public String toString() {
        return toStringHelper(this)
                .add("assetTag", assetTag)
                .add("type", type)
                .add("mode", mode)
                .add("writeProtected", writeProtected)
                .add("capacityGb", capacityGb)
                .add("image", image)
                .add("bootable", bootable)
                .add("snapshot", snapshot)
                .add("status", status)
                .add("links", links)
                .toString();
    }

    @JsonIgnoreProperties(ignoreUnknown = true)
    private static final class Links {
        @JsonProperty("LogicalDrives")
        private List<ODataId> logicalDrives = emptyList();
        @JsonProperty("PhysicalDrives")
        private List<ODataId> physicalDrives = emptyList();
        @JsonProperty("MasterDrive")
        private ODataId masterDrive;
        @JsonProperty("UsedBy")
        private List<ODataId> usedBy = emptyList();
        @JsonProperty("Targets")
        private List<ODataId> targets = emptyList();
        @JsonProperty("ManagedBy")
        private List<ODataId> managedBy = emptyList();
    }
}
