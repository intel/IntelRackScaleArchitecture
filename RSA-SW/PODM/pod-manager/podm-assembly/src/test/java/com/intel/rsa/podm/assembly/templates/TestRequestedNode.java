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

package com.intel.rsa.podm.assembly.templates;

import com.intel.rsa.podm.business.dto.RequestedLocalDrive;
import com.intel.rsa.podm.business.dto.RequestedMemory;
import com.intel.rsa.podm.business.dto.RequestedNetworkInterface;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.dto.RequestedProcessor;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;

import java.util.Collections;
import java.util.List;

public class TestRequestedNode implements RequestedNode {

    private static final String DEFAULT_NAME = "";
    private static final String DEFAULT_DESCRIPTION = "";

    private List<RequestedProcessor> processors = Collections.emptyList();
    private List<RequestedMemory> memoryModules = Collections.emptyList();
    private List<RequestedLocalDrive> localDrives = Collections.emptyList();
    private List<RequestedNetworkInterface> networkInterfaces = Collections.emptyList();

    public TestRequestedNode() {
    }

    public TestRequestedNode(List<RequestedProcessor> processors, List<RequestedMemory> memoryModules,
                             List<RequestedLocalDrive> localDrives, List<RequestedNetworkInterface> networkInterfaces) {
        this.processors = processors;
        this.memoryModules = memoryModules;
        this.localDrives = localDrives;
        this.networkInterfaces = networkInterfaces;
    }

    @Override
    public String getName() {
        return DEFAULT_NAME;
    }

    @Override
    public String getDescription() {
        return DEFAULT_DESCRIPTION;
    }

    @Override
    public List<RequestedProcessor> getProcessors() {
        return processors;
    }

    @Override
    public List<RequestedMemory> getMemory() {
        return memoryModules;
    }

    @Override
    public List<RequestedRemoteDrive> getRemoteDrives() {
        return Collections.emptyList();
    }

    @Override
    public List<RequestedLocalDrive> getLocalDrives() {
        return localDrives;
    }

    @Override
    public List<RequestedNetworkInterface> getNetworkInterfaces() {
        return networkInterfaces;
    }

    @Override
    public Object getOem() {
        return null;
    }
}
