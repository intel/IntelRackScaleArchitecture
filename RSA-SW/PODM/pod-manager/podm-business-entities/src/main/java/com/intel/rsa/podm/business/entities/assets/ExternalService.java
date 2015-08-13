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
import java.net.URI;
import java.util.Objects;
import java.util.UUID;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.uriProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.uuidProperty;
import static javax.transaction.Transactional.TxType.REQUIRED;

@Dependent
@Transactional(REQUIRED)
public class ExternalService extends DomainObject {
    public static final DomainObjectProperty<UUID> UUID = uuidProperty("uuid");
    public static final DomainObjectProperty<URI> BASE_URI = uriProperty("baseUri");
    public static final DomainObjectProperty<Type> SERVICE_TYPE = enumProperty("type", Type.class);

    public UUID getUuid() {
        return getProperty(UUID);
    }

    public void setUuid(UUID uuid) {
        setProperty(UUID, uuid);
    }

    public URI getBaseUri() {
        return getProperty(BASE_URI);
    }

    public void setBaseUri(URI baseUri) {
        setProperty(BASE_URI, baseUri);
    }

    public Type getServiceType() {
        return getProperty(SERVICE_TYPE);
    }

    public void setServiceType(Type type) {
        setProperty(SERVICE_TYPE, type);
    }

    public <T extends DomainObject & Discoverable> T findOrCreate(URI uri, Class<T> clazz) {
        for (T domainObject : getLinked(() -> "owns", clazz)) {
            URI sourceUri = domainObject.getSourceUri();

            if (Objects.equals(uri, sourceUri)) {
                return domainObject;
            }
        }

        T domainObject = addDomainObject(() -> "owns", clazz);
        domainObject.setSourceUri(uri);

        return domainObject;
    }

    public void add() {

    }

    public enum Type {
        PSME,
        RSS
    }
}
