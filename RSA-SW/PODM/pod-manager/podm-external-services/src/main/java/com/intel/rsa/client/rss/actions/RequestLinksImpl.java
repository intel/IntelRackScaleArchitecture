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
import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.intel.rsa.client.api.rss.ODataIdWrapper;
import com.intel.rsa.client.api.rss.RequestLinks;
import java.util.ArrayList;
import java.util.List;

public class RequestLinksImpl implements RequestLinks {

    @JsonProperty("LogicalDrives")
    @JsonSerialize(contentAs = ODataIdWrapperImpl.class)
    private final List<ODataIdWrapper> logicalDrives = new ArrayList<ODataIdWrapper>();

    @JsonProperty("MasterDrive")
    @JsonSerialize(as = ODataIdWrapperImpl.class)
    private ODataIdWrapper masterDrive;

    @Override
    public List<ODataIdWrapper> getLogicalDrivesList() {
        return logicalDrives;
    }

    @Override
    public ODataIdWrapper getMasterDrive() {
        return masterDrive;
    }

    @Override
    public void setMasterDrive(ODataIdWrapper masterDrive) {
        this.masterDrive = masterDrive;
    }
}
