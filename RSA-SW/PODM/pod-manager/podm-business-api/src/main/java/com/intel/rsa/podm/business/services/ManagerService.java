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
import com.intel.rsa.podm.business.dto.ManagerDto;
import com.intel.rsa.podm.business.exceptions.EntityNotFoundException;
import com.intel.rsa.podm.business.services.context.Context;

/**
 * Service for Manager assets.
 */
public interface ManagerService {
    /**
     * @return Global collection of managers
     */
    CollectionDto getManagerCollection();

    /**
     * Finds Manager with specified business id or throws exception if none matches.
     * @param context context of Manager
     * @return found Manager
     * @throws EntityNotFoundException
     */
    ManagerDto getManager(Context context) throws EntityNotFoundException;

    /**
     * Checks if context points to storage Manager.
     * @param context context of Manager
     * @return true if storage Manager in context, false if manager of other type
     * @throws EntityNotFoundException if manager not found
     */
    boolean isStorageManager(Context context) throws EntityNotFoundException;
}
