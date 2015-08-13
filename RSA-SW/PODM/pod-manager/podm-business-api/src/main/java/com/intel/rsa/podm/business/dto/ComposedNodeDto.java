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

package com.intel.rsa.podm.business.dto;

import com.intel.rsa.common.types.ComposedNodeState;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.Collection;

public interface ComposedNodeDto {
    Id getId();
    String getName();
    String getDescription();
    SystemType getSystemType();
    ComposedNodeState getComposedNodeState();
    OffsetDateTime getModified();
    StatusDto getStatus();

    Processors getProcessors();
    Memory getMemory();
    Context getContext();

    Collection<Context> getProcessorCollection();
    Collection<Context> getMemoryCollection();
    Collection<Context> getRemoteDriveCollection();
    Collection<Context> getLocalDriveCollection();
    Collection<Context> getEthernetInterfaceCollection();
    Collection<Context> getManagedByCollection();

    public interface Processors {
        int getCount();
        Integer getProcessorSockets();
        String getProcessorFamily();
        StatusDto getStatus();
    }

    public interface Memory {
        int getTotalSystemMemoryGB();
        Integer getMemorySockets();
        StatusDto getStatus();
    }

    public interface StatusDto {
        State getState();
        Health getHealth();
        Health getHealthRollup();
    }
}
