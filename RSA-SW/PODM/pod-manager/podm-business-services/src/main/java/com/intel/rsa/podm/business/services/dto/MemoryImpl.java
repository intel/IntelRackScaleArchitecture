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

package com.intel.rsa.podm.business.services.dto;

import com.intel.rsa.podm.business.dto.ComposedNodeDto;

public class MemoryImpl implements ComposedNodeDto.Memory {

    private int totalSystemMemoryGB;
    private Integer memorySockets;
    private ComposedNodeDto.StatusDto status;

    @Override
    public int getTotalSystemMemoryGB() {
        return totalSystemMemoryGB;
    }

    public void setTotalSystemMemoryGB(int totalSystemMemoryGB) {
        this.totalSystemMemoryGB = totalSystemMemoryGB;
    }

    @Override
    public Integer getMemorySockets() {
        return memorySockets;
    }

    public void setMemorySockets(Integer memorySockets) {
        this.memorySockets = memorySockets;
    }

    @Override
    public ComposedNodeDto.StatusDto getStatus() {
        return status;
    }

    public void setStatus(ComposedNodeDto.StatusDto status) {
        this.status = status;
    }
}
