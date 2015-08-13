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

package com.intel.rsa.podm.services.detection;

import com.intel.rsa.podm.services.detection.api.ServiceDetectionException;
import org.testng.annotations.Test;

import javax.enterprise.concurrent.ManagedScheduledExecutorService;
import java.util.concurrent.RejectedExecutionException;

import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;


public class ServicesDetectorTest {

    @Test(expectedExceptions = UnsupportedOperationException.class)
    public void subsequentCallsOfPollForServicesMethod_ShouldThrow() throws ServiceDetectionException {

        ServicesDetector servicesDetector = new ServicesDetector();
        servicesDetector.managedExecutorService = mock(ManagedScheduledExecutorService.class);

        servicesDetector.pollForServices();
        servicesDetector.pollForServices();
    }

    @Test(expectedExceptions = ServiceDetectionException.class)
    public void whenStartingPollingTaskViaManagedExecutorServiceFails_ShouldThrow() throws ServiceDetectionException {
        ServicesDetector servicesDetector = new ServicesDetector();
        servicesDetector.managedExecutorService = mock(ManagedScheduledExecutorService.class);

        doThrow(new RejectedExecutionException("Test throw"))
                .when(servicesDetector.managedExecutorService)
                .scheduleWithFixedDelay(any(), anyLong(), anyLong(), any());

        servicesDetector.pollForServices();
    }
}
