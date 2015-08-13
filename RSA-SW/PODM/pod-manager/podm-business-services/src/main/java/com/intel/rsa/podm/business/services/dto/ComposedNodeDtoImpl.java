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

import com.intel.rsa.common.types.ComposedNodeState;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.SystemType;
import com.intel.rsa.podm.business.dto.ComposedNodeDto;
import com.intel.rsa.podm.business.services.context.Context;

import java.time.OffsetDateTime;
import java.util.ArrayList;
import java.util.Collection;

/**
 * Implementation of composedNodeDto
 */
public class ComposedNodeDtoImpl implements ComposedNodeDto {

    private final Id id;
    private final Context context;
    private String name;
    private String description;
    private SystemType systemType;
    private ComposedNodeState composedNodeState;
    private OffsetDateTime modified;
    private StatusDto status;
    private Processors processors;
    private Memory memory;

    private final Collection<Context> processorCollection = new ArrayList<>();
    private final Collection<Context> memoryCollection = new ArrayList<>();
    private final Collection<Context> remoteDriveCollection = new ArrayList<>();
    private final Collection<Context> localDriveCollection = new ArrayList<>();
    private final Collection<Context> ethernetInterfaceCollection = new ArrayList<>();
    private final Collection<Context> managedByCollection = new ArrayList<>();

    public ComposedNodeDtoImpl(Id id, Context context) {
        this.id = id;
        this.context = context;
    }

    @Override
    public Id getId() {
        return id;
    }

    @Override
    public Context getContext() {
        return context;
    }

    @Override
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    @Override
    public SystemType getSystemType() {
        return systemType;
    }

    public void setSystemType(SystemType systemType) {
        this.systemType = systemType;
    }

    @Override
    public ComposedNodeState getComposedNodeState() {
        return composedNodeState;
    }

    public void setComposedNodeState(ComposedNodeState composedNodeState) {
        this.composedNodeState = composedNodeState;
    }

    @Override
    public OffsetDateTime getModified() {
        return modified;
    }

    public void setModified(OffsetDateTime modified) {
        this.modified = modified;
    }

    @Override
    public StatusDto getStatus() {
        return status;
    }

    public void setStatus(StatusDto status) {
        this.status = status;
    }

    @Override
    public Processors getProcessors() {
        return processors;
    }

    public void setProcessors(Processors processors) {
        this.processors = processors;
    }

    @Override
    public Memory getMemory() {
        return memory;
    }

    public void setMemory(Memory memory) {
        this.memory = memory;
    }

    @Override
    public Collection<Context> getProcessorCollection() {
        return processorCollection;
    }

    public void addProcessorContext(Context processorContext) {
        processorCollection.add(processorContext);
    }

    @Override
    public Collection<Context> getMemoryCollection() {
        return memoryCollection;
    }

    public void addMemoryContext(Context memoryContext) {
        memoryCollection.add(memoryContext);
    }

    @Override
    public Collection<Context> getRemoteDriveCollection() {
        return remoteDriveCollection;
    }

    public void addRemoteDriveContext(Context remoteDriveContext) {
        remoteDriveCollection.add(remoteDriveContext);
    }

    @Override
    public Collection<Context> getLocalDriveCollection() {
        return localDriveCollection;
    }

    public void addLocalDriveContext(Context localDriveContext) {
        localDriveCollection.add(localDriveContext);
    }

    @Override
    public Collection<Context> getEthernetInterfaceCollection() {
        return ethernetInterfaceCollection;
    }

    public void addEthernetInterfaceContext(Context ethernetInterfaceContext) {
        ethernetInterfaceCollection.add(ethernetInterfaceContext);
    }

    @Override
    public Collection<Context> getManagedByCollection() {
        return managedByCollection;
    }

    public void addManagedByContext(Context managedByContext) {
        managedByCollection.add(managedByContext);
    }
}
