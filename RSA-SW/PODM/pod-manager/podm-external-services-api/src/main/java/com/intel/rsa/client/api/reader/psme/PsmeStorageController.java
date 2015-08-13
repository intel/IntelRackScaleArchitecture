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

package com.intel.rsa.client.api.reader.psme;

import com.intel.rsa.client.api.reader.ResourceSupplier;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.StorageControllerInterface;
import com.intel.rsa.common.types.locations.Location;

/**
 * StorageController resource details provided by PSME.
 */
public interface PsmeStorageController extends PsmeResource {
    StorageControllerInterface getControllerInterface();
    int getDriveCount();
    State getState();
    Health getHealth();
    Health getHealthRollup();
    Location getLocation();

    Iterable<ResourceSupplier<PsmeDrive>> getDrives() throws RsaApiReaderException;
}
