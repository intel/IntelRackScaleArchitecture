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

package com.intel.rsa.client.api.reader.rss;

import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.VolumeMode;
import com.intel.rsa.common.types.VolumeType;

public interface RssLogicalDrive extends RssResource {
    VolumeType getType();
    VolumeMode getMode();
    boolean isWriteProtected();
    Boolean isSnapshot();
    String getAssetTag();
    int getCapacityGb();
    String getImage();
    Boolean isBootable();
    State getState();
    Health getHealth();
    Health getHealthRollup();
    Iterable<ResourceSupplier<RssLogicalDrive>> getLogicalDrives() throws RsaApiReaderException;
    Iterable<ResourceSupplier<RssDrive>> getPhysicalDrives() throws RsaApiReaderException;
    ResourceSupplier<RssLogicalDrive> getMasterDrive();
    Iterable<ResourceSupplier<RssLogicalDrive>> getUsedBy() throws RsaApiReaderException;
    Iterable<ResourceSupplier<RssRemoteTarget>> getTargets() throws RsaApiReaderException;
    Iterable<ResourceSupplier<RssStorageManager>> getManagers() throws RsaApiReaderException;
}
