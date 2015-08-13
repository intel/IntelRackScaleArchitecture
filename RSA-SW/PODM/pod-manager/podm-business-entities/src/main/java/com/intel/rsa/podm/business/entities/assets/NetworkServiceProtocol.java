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

package com.intel.rsa.podm.business.entities.assets;

import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.booleanProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;

/**
 * Protocol supported by NetworkService.
 */
@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class NetworkServiceProtocol extends DomainObject {
    public static final DomainObjectProperty<Boolean> ENABLED = booleanProperty("enabled");
    public static final DomainObjectProperty<Integer> PORT = integerProperty("port");

    public Boolean isEnabled() {
        return getProperty(ENABLED);
    }

    public void setEnabled(Boolean enabled) {
        setProperty(ENABLED, enabled);
    }

    public Integer getPort() {
        return getProperty(PORT);
    }

    public void setPort(Integer port) {
        setProperty(PORT, port);
    }
}
