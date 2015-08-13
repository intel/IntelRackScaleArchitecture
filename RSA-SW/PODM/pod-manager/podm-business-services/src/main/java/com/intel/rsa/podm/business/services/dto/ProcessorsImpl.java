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

public class ProcessorsImpl implements ComposedNodeDto.Processors {

    private int count;
    private Integer processorSockets;
    private String processorFamily;
    private ComposedNodeDto.StatusDto status;

    @Override
    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    @Override
    public Integer getProcessorSockets() {
        return processorSockets;
    }

    public void setProcessorSockets(Integer processorSockets) {
        this.processorSockets = processorSockets;
    }

    @Override
    public String getProcessorFamily() {
        return processorFamily;
    }

    public void setProcessorFamily(String processorFamily) {
        this.processorFamily = processorFamily;
    }

    @Override
    public ComposedNodeDto.StatusDto getStatus() {
        return status;
    }

    public void setStatus(ComposedNodeDto.StatusDto status) {
        this.status = status;
    }
}
