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
import com.intel.rsa.podm.configuration.ConfigurationReader;
import com.intel.rsa.podm.configuration.ConfigurationIOException;
import com.intel.rsa.podm.configuration.ConfigurationWriter;
import com.intel.rsa.podm.configuration.dto.UuidDto;
import org.testng.annotations.Test;

import java.util.UUID;

import static com.intel.rsa.podm.configuration.PersistentConfigurations.SERVICE_ROOT_UUID;
import static java.util.UUID.randomUUID;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.matches;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertEquals;
import static org.testng.Assert.assertNotNull;

public class ServiceRootUuidProviderTest {
    @Test
    public void whenGettingUuidAndPrevoiusUuidNotExists_ShouldGenerateNewUuid() throws ConfigurationIOException {
        ServiceRootUuidProvider handler = createHandler();
        when(handler.configurationReader.readConfiguration(SERVICE_ROOT_UUID.getName(), UuidDto.class)).thenReturn(new UuidDto());

        UUID uuid = handler.getUuid();

        verify(handler.configurationWriter, times(1)).saveConfiguration(matches(SERVICE_ROOT_UUID.getName()), any());
        assertNotNull(uuid);
    }

    @Test
    public void whenGettingUuidAndPreviousUuidExists_ShouldReturnUnchangedPreviousUuid() throws ConfigurationIOException {
        ServiceRootUuidProvider handler = createHandler();
        UuidDto previousUuidDto = new UuidDto(randomUUID());
        when(handler.configurationReader.readConfiguration(SERVICE_ROOT_UUID.getName(), UuidDto.class)).thenReturn(previousUuidDto);

        UUID actualUuid = handler.getUuid();

        verify(handler.configurationWriter, never()).saveConfiguration(matches(SERVICE_ROOT_UUID.getName()), any());
        assertEquals(actualUuid, previousUuidDto.getUuid());
    }

    @Test
    public void whenErrorReadingConfiguration_ShouldGenerateNewUuid() throws ConfigurationIOException {
        ServiceRootUuidProvider handler = createHandler();
        when(handler.configurationReader.readConfiguration(SERVICE_ROOT_UUID.getName(), UuidDto.class)).thenThrow(ConfigurationIOException.class);

        UUID uuid = handler.getUuid();

        verify(handler.configurationWriter, times(1)).saveConfiguration(matches(SERVICE_ROOT_UUID.getName()), any());
        assertNotNull(uuid);
    }

    @Test
    public void whenErrorWritingConfiguration_ShouldLogErrorAndReturnDefaultUuid() throws ConfigurationIOException {
        ServiceRootUuidProvider handler = createHandler();
        when(handler.configurationReader.readConfiguration(SERVICE_ROOT_UUID.getName(), UuidDto.class)).thenReturn(new UuidDto());
        doThrow(ConfigurationIOException.class).when(handler.configurationWriter).saveConfiguration(any(), any());

        UUID uuid = handler.getUuid();

        verify(handler.logger, times(1)).e(any(), any(ConfigurationIOException.class));
        assertEquals(uuid, ServiceRootUuidProvider.DEFAULT_UUID);
    }

    private ServiceRootUuidProvider createHandler() {
        ServiceRootUuidProvider handler = new ServiceRootUuidProvider();
        handler.logger = mock(Logger.class);
        handler.configurationWriter = mock(ConfigurationWriter.class);
        handler.configurationReader = mock(ConfigurationReader.class);
        return handler;
    }
}
