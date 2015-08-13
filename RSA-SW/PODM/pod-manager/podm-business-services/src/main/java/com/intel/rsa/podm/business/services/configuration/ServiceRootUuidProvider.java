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

package com.intel.rsa.podm.business.services.configuration;

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.configuration.ConfigurationIOException;
import com.intel.rsa.podm.configuration.ConfigurationReader;
import com.intel.rsa.podm.configuration.ConfigurationWriter;
import com.intel.rsa.podm.configuration.dto.UuidDto;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.UUID;

import static com.intel.rsa.podm.configuration.PersistentConfigurations.SERVICE_ROOT_UUID;
import static java.util.UUID.fromString;

/**
 * Handles providing of service root uuid. Creates new uuid if not present in configuration.
 */
@Dependent
public class ServiceRootUuidProvider {
    static final UUID DEFAULT_UUID = fromString("00000000-0000-0000-0000-000000000000");

    @Inject
    ConfigurationReader configurationReader;

    @Inject
    ConfigurationWriter configurationWriter;

    @Inject
    Logger logger;

    public UUID getUuid() {
        UUID uuid = readUuidFromConfiguration();

        if (uuid == null) {
            uuid = createUuid();
        }

        return uuid;
    }

    private UUID readUuidFromConfiguration() {
        UUID uuid = null;
        try {
            UuidDto uuidDto = configurationReader.readConfiguration(SERVICE_ROOT_UUID.getName(), UuidDto.class);
            uuid = uuidDto.getUuid();
        } catch (ConfigurationIOException e) {
            logger.w("ServiceRoot UUID configuration is unavailable");
        }
        return uuid;
    }

    private UUID createUuid() {
        try {
            UUID uuid = UUID.randomUUID();
            configurationWriter.saveConfiguration(SERVICE_ROOT_UUID.getName(), new UuidDto(uuid));
            logger.i("Service Root UUID is set to {}", uuid);
            return uuid;
        } catch (ConfigurationIOException e) {
            logger.e("Cannot create ServiceRoot UUID configuration. Using default value.", e);
        }
        return DEFAULT_UUID;
    }
}
