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

package com.intel.rsa.podm.business.services;

import com.intel.rsa.podm.business.dto.CollectionDto;
import com.intel.rsa.podm.business.dto.DriveDto;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;

public interface DriveService {

    /**
     * Returns collection of context elements which identify drives available under specific storage controller.
     * @param storageControllerContext
     * @return
     * @throws com.intel.rsa.podm.business.exceptions.EntityNotFoundException if collection is not found in the context specified by the argument
     */
    CollectionDto getDriveCollection(Context storageControllerContext) throws EntityNotFoundException;

    /**
     * Returns representation of a specific Drive.
     * @param driveContext
     * @return
     * @throws com.intel.rsa.podm.business.exceptions.EntityNotFoundException if drive is not found in the context specified by the argument
     */
    DriveDto getDrive(Context driveContext) throws EntityNotFoundException;
}
