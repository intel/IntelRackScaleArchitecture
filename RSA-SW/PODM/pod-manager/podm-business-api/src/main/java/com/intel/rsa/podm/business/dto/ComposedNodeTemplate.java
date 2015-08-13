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

import com.intel.rsa.common.types.DriveType;
import com.intel.rsa.common.types.InstructionSet;
import com.intel.rsa.common.types.InstructionSetExtension;
import com.intel.rsa.common.types.MemoryType;
import com.intel.rsa.common.types.ProcessorModel;
import com.intel.rsa.common.types.RemoteStorageBackend;
import com.intel.rsa.common.types.RemoteStorageType;
import com.intel.rsa.common.types.ReplicationMethod;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.services.context.Context;

import java.math.BigDecimal;
import java.util.List;

/**
 * Interface providing requirements for a Composed Node. Contains all data necessary
 * to perform allocation process of a new Composed Node.
 */
public interface ComposedNodeTemplate {
    String getName();
    String getDescription();
    List<Processor> getProcessors();
    List<Memory> getMemory();
    List<RemoteDrive> getRemoteDrives();
    List<LocalDrive> getLocalDrives();
    List<NetworkInterface> getNetworkInterfaces();
    Object getOem();

    public interface Processor extends Localizable {
        Long getQuantity();
        ProcessorModel getModel();
        Integer getTotalCores();
        Integer getMaxSpeedMHz();
        InstructionSet getInstructionSet();
        List<InstructionSetExtension> getInstructionSetExtensions();
        Location getLocation();
    }

    public interface Memory extends Localizable {
        BigDecimal getSizeGB();
        Integer getSpeedMHz();
        MemoryType getType();
        Location getLocation();
    }

    public interface RemoteDrive extends Localizable {
        BigDecimal getCapacityGB();
        RemoteStorageBackend getStorageBackend();
        RemoteStorageType getAddressType();
        String getAddress();
        MasterDrive getMaster();
        Integer getBootOrder();
        Location getLocation();

        // Drive used as source upon RemoteDrive creation
        public interface MasterDrive extends Localizable {
            ReplicationMethod getType();
            RemoteStorageType getAddressType();
            Context getAddress();
        }
    }

    public interface LocalDrive extends Localizable {
        BigDecimal getCapacityGB();
        DriveType getType();
        Integer getBootOrder();
        Location getLocation();
    }

    public interface NetworkInterface extends Localizable  {
        Integer getSpeedMbps();
        List<Vlan> getVlans();
        Location getLocation();

        public interface Vlan {
            boolean isTagged();
            Integer getVlanId();
        }
    }
}
