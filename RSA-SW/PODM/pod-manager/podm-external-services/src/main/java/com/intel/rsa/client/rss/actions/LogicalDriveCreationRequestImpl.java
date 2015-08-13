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

package com.intel.rsa.client.rss.actions;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.client.api.rss.LogicalDriveCreationRequest;
import com.intel.rsa.client.api.rss.RequestLinks;
import com.intel.rsa.common.types.VolumeMode;
import com.intel.rsa.common.types.VolumeType;

import java.math.BigDecimal;


/*

4.6.3.4 POST
Request:
POST /rest/v1/Services/1/LogicalDrives
Content-Type: application/json
{
    "Name": "RSA Logical Drive",
    “Type”: “LVM”,
    “Mode”: “LV”,
    “Protected”: false,
    “AssetTag”: “CustomerWritableThing”,
    “CapacityGB”: 8096, “Image”: "Ubuntu 12.04.4LTS / Linux 3.11 / 2014.1",
    “Bootable”: true,
    “Snapshot”: true,
    "Links": {
        "LogicalDrives": [
            "@odata.id": "/rest/v1/Services/1/LogicalDrives/4
        ],
        “MasterDrive”: {
             "@odata.id": "/rest/v1/Services/1/LogicalDrives/12"
        },
    }
}
Note: LogicalDrives must contain single link to LogicalDrive which is Logical Volume Group (“Mode” = ”LVG”)
Response:
HTTP/1.1 201 Created
Location: http://<IP>:<PORT>/rest/v1/Services/1/LogicalDrives/2

*/

public class LogicalDriveCreationRequestImpl implements LogicalDriveCreationRequest {

    @JsonProperty("Name") //Get Set
    private final String name = "RSA Logical Drive";

    @JsonProperty("Type")
    private final VolumeType type = VolumeType.LVM;

    @JsonProperty("Mode")
    private final VolumeMode mode = VolumeMode.LV;

    @JsonProperty("Protected")
    private final boolean protection = false;

    @JsonProperty("AssetTag") //Get Set
    private String assetTag;

    @JsonProperty("CapacityGB")
    private BigDecimal capacityGB;

    @JsonProperty("Image") //Get Set
    private String image;

    @JsonProperty("Bootable")
    private final boolean bootable = true;

    @JsonProperty("Snapshot")
    private boolean snapshot;

    @JsonProperty("Links")
    private final RequestLinks links = new RequestLinksImpl();

    public String getAssetTag() {
        return assetTag;
    }

    public void setAssetTag(String assetTag) {
        this.assetTag = assetTag;
    }

    public BigDecimal getCapacityGB() {
        return capacityGB;
    }

    public void setCapacityGB(BigDecimal capacityGB) {
        this.capacityGB = capacityGB;
    }

    public String getImage() {
        return image;
    }

    public void setImage(String image) {
        this.image = image;
    }

    public RequestLinks getLinks() {
        return links;
    }

    @Override
    public void setSnapshot(boolean isSnapshot) {
        snapshot = isSnapshot;
    }
}
