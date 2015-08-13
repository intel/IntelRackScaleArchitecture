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

package com.intel.rsa.podm.rest.representation.json.templates.assembly;

import com.intel.rsa.podm.business.dto.RequestedLocalDrive;
import com.intel.rsa.podm.business.dto.RequestedMemory;
import com.intel.rsa.podm.business.dto.RequestedNetworkInterface;
import com.intel.rsa.podm.business.dto.RequestedNode;
import com.intel.rsa.podm.business.dto.RequestedProcessor;
import com.intel.rsa.podm.business.dto.RequestedRemoteDrive;

import java.util.Collections;
import java.util.List;

public class RequestedNodeDefaultImpl implements RequestedNode {

    private static final String DEFAULT_NAME = "Default name";
    private static final String DEFAULT_DESCRIPTION = "Default description";

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
        return Collections.emptyList();
    }

    @Override
    public List<RequestedMemory> getMemory() {
        return Collections.emptyList();
    }

    @Override
    public List<RequestedRemoteDrive> getRemoteDrives() {
        return Collections.emptyList();
    }

    @Override
    public List<RequestedLocalDrive> getLocalDrives() {
        return Collections.emptyList();
    }

    @Override
    public List<RequestedNetworkInterface> getNetworkInterfaces() {
        return Collections.emptyList();
    }

    @Override
    public Object getOem() {
        return null;
    }
}
