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
package com.intel.rsa.podm.rest.representation.json.templates;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.VolumeMode;
import com.intel.rsa.common.types.VolumeType;
import com.intel.rsa.podm.rest.odataid.ODataId;
import com.intel.rsa.podm.rest.representation.json.templates.attributes.StatusJson;

import java.util.ArrayList;
import java.util.Collection;

@JsonPropertyOrder({
    "@odata.context", "@odata.id", "@odata.type", "id", "name", "modified",
    "status", "type", "mode", "writeProtected", "snapshot", "capacityGB", "assetTag",
    "image", "bootable", "oem", "links"
})
public final class LogicalDriveJson extends BaseJson {
    public Id id;
    public String name;
    public VolumeType type;
    public VolumeMode mode;
    @JsonProperty("Protected")
    public boolean writeProtected;
    public Boolean snapshot;
    public String assetTag;
    public int capacityGB;
    public String image;
    public boolean bootable;
    public final StatusJson status = new StatusJson();
    public final Oem oem = new Oem();
    public final Links links = new Links();

    public static final class Oem {
    }

    public LogicalDriveJson() {
        super("#RSALogicalDrive.1.0.0.RSALogicalDrive");
    }

    @JsonPropertyOrder({
      "logicalDrives", "physicalDrives", "masterDrive", "usedBy", "targets", "managedBy", "oem"
    })
    public static final class Links extends BaseLinksJson {
        public Collection<ODataId> logicalDrives = new ArrayList<>();
        public Collection<ODataId> physicalDrives = new ArrayList<>();
        public ODataId masterDrive;
        public Collection<ODataId> usedBy = new ArrayList<>();
        public Collection<ODataId> targets = new ArrayList<>();
        public Collection<ODataId> managedBy = new ArrayList<>();
    }
}
