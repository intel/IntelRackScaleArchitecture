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

import com.intel.rsa.client.api.reader.RsaReader;
import com.intel.rsa.common.logger.Logger;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.UUID;

@Dependent
public class DiscoveryRunner {
    @Inject
    private Logger logger;

    @Inject
    private Discovery discovery;

    @Inject
    private ReaderFactory readerFactory;

    public void run(UUID serviceUuid) {
        logger.i("Polling data from '{}' started", serviceUuid);

        try (RsaReader reader = readerFactory.create(serviceUuid)) {
            discovery.discover(reader);
            logger.i("Polling data from '{}' finished", serviceUuid);
        } catch (RuntimeException e) {
            logger.e("Error while polling data from '" + serviceUuid + "'", e);
        }
    }
}
