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

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.business.dto.ServiceRootDto;
import com.intel.rsa.podm.business.services.configuration.ServiceRootUuidProvider;
import com.intel.rsa.podm.configuration.ConfigurationIOException;
import org.testng.annotations.Test;

import java.util.UUID;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.Assert.assertEquals;
import static org.testng.Assert.assertNotNull;


public class ServiceRootServiceImplTest {
    @Test
    public void whenInitialized_ShouldBeAbleToReturnServiceRootWithValidUuid() throws ConfigurationIOException {
        ServiceRootServiceImpl service = createService();
        UUID expectedUuid = UUID.randomUUID();
        when(service.serviceRootUuidProvider.getUuid()).thenReturn(expectedUuid);

        service.initialize();
        ServiceRootDto serviceRoot = service.getServiceRoot();

        assertEquals(serviceRoot.getUuid(), expectedUuid);
    }

    @Test
    public void whenInitialized_ShouldBeAbleToReturnServiceRootWithValidModified() throws ConfigurationIOException {
        ServiceRootServiceImpl service = createService();

        service.initialize();
        ServiceRootDto serviceRoot = service.getServiceRoot();

        assertNotNull(serviceRoot.getModified());
    }

    @Test
    public void whenGettingServiceRoot_ShouldReturnSameUuidAndModifiedOnSubsequentInvocations() {
        ServiceRootServiceImpl service = createService();
        when(service.serviceRootUuidProvider.getUuid()).thenReturn(UUID.randomUUID());

        service.initialize();
        ServiceRootDto serviceRootFirstInvocation = service.getServiceRoot();
        ServiceRootDto serviceRootSecondInvocation = service.getServiceRoot();

        assertEquals(serviceRootFirstInvocation.getUuid(), serviceRootSecondInvocation.getUuid());
        assertEquals(serviceRootFirstInvocation.getModified(), serviceRootSecondInvocation.getModified());
    }

    private ServiceRootServiceImpl createService() {
        ServiceRootServiceImpl service = new ServiceRootServiceImpl();
        service.logger = mock(Logger.class);
        service.serviceRootUuidProvider = mock(ServiceRootUuidProvider.class);
        return service;
    }
}
