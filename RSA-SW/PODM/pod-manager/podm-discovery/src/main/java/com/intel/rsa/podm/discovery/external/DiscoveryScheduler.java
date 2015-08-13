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

package com.intel.rsa.podm.discovery.external;

import javax.annotation.Resource;
import javax.enterprise.concurrent.ManagedScheduledExecutorService;
import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.UUID;

import static java.util.concurrent.TimeUnit.MINUTES;

@Dependent
public class DiscoveryScheduler {
    @Resource
    ManagedScheduledExecutorService managedExecutorService;

    @Inject
    private DiscoveryRunner runner;

    public void schedule(UUID serviceUuid) {
        final int fixedDelay = 1;
        managedExecutorService.scheduleWithFixedDelay(() -> runner.run(serviceUuid), 0, fixedDelay, MINUTES);
    }
}
