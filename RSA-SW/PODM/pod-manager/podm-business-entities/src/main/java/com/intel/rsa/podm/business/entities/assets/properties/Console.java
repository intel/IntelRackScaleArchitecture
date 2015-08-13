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

package com.intel.rsa.podm.business.entities.assets.properties;

import com.intel.rsa.common.types.ConnectionType;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.booleanProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class Console extends DomainObject {
    public static final DomainObjectProperty<Boolean> ENABLED = booleanProperty("enabled");
    public static final DomainObjectProperty<Integer> MAX_CONCURRENT_SESSIONS = integerProperty("maxConcurrentSessions");
    public static final DomainObjectProperty<ConnectionType> CONNECT_TYPES_SUPPORTED = enumProperty("ConnectTypesSupported", ConnectionType.class);

    public Boolean isEnabled() {
        return getProperty(ENABLED);
    }

    public void setEnabled(boolean enabled) {
        setProperty(ENABLED, enabled);
    }

    public Integer getMaxConcurrentSessions() {
        return getProperty(MAX_CONCURRENT_SESSIONS);
    }

    public void setMaxConcurrentSessions(Integer maxConcurrentSessions) {
        setProperty(MAX_CONCURRENT_SESSIONS, maxConcurrentSessions);
    }

    public ConnectionType getConnectTypesSupported() {
        return getProperty(CONNECT_TYPES_SUPPORTED);
    }

    public void setConnectTypesSupported(ConnectionType connectTypesSupported) {
        setProperty(CONNECT_TYPES_SUPPORTED, connectTypesSupported);
    }

}
