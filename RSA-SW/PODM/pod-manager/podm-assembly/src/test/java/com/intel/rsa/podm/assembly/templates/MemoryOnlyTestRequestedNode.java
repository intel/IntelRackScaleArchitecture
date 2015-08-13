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

import com.intel.rsa.podm.business.dto.RequestedMemory;

import java.util.List;

public class MemoryOnlyTestRequestedNode extends TestRequestedNode {
    private final List<RequestedMemory> memoryModules;

    public MemoryOnlyTestRequestedNode(List<RequestedMemory> memoryModules) {
        this.memoryModules = memoryModules;
    }

    @Override
    public List<RequestedMemory> getMemory() {
        return memoryModules;
    }
}
