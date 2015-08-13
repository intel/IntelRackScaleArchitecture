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

package com.intel.rsa.podm.rest;


import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.configuration.VersionLoader;
import com.intel.rsa.podm.services.detection.ServicesDetector;
import com.intel.rsa.podm.services.detection.api.ServiceDetectionException;
import org.testng.annotations.Test;

import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

public class ApplicationStartupTest {

    @Test
    public void whenStartingApplication_ShouldInitializePollingForServicesOnce() throws ServiceDetectionException {
        ApplicationStartup sut = createApplicationStartup();

        sut.applicationStartup();

        verify(sut.servicesDetector, times(1)).pollForServices();
    }

    @Test(expectedExceptions = Error.class, expectedExceptionsMessageRegExp = "Application failed to start.*")
    public void whenInitializationOfPollingForServicesFails_ShouldThrowAnError() throws ServiceDetectionException {
        ApplicationStartup sut = createApplicationStartup();

        doThrow(ServiceDetectionException.class).when(sut.servicesDetector).pollForServices();

        sut.applicationStartup();
    }

    private ApplicationStartup createApplicationStartup() {
        ApplicationStartup sut = new ApplicationStartup();
        sut.logger = mock(Logger.class);
        sut.servicesDetector = mock(ServicesDetector.class);
        sut.versionLoader = mock(VersionLoader.class);

        return sut;
    }
}
