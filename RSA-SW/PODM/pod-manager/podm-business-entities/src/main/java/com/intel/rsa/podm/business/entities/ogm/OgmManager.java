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

package com.intel.rsa.podm.business.entities.ogm;

import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectInspector;
import com.intel.rsa.podm.repository.api.GraphSchemaManager;
import com.intel.rsa.podm.repository.api.GraphVertexType;

import javax.enterprise.context.ApplicationScoped;
import javax.inject.Inject;
import java.util.HashMap;
import java.util.Map;

import static com.google.common.base.Preconditions.checkNotNull;
import static com.intel.rsa.podm.business.entities.base.DomainObjectInspector.inspect;

/**
 * OgmManager allows to manage mapping of Object Model (provided with DomainObject classes) to Graph Model.
 *  Be aware that DomainObject class proxied by container will not be handled in any special way.
*/
@ApplicationScoped
public class OgmManager {

    @Inject
    GraphSchemaManager graphSchemaManager;

    private final HashMap<String, Class<? extends DomainObject>> mappedDomainObjectClasses = new HashMap<>();

    public void addToMapping(Class<? extends DomainObject> domainObjectClass) {
        checkNotNull(domainObjectClass);

        DomainObjectInspector inspector = inspect(domainObjectClass);
        Map<String, Class> properties = inspector.getProperties();
        String name = domainObjectClass.getSimpleName();

        GraphVertexType vertexType = new GraphVertexType(name, properties);
        graphSchemaManager.registerVertexType(vertexType);

        mappedDomainObjectClasses.put(name, domainObjectClass);
    }

    public Class<? extends DomainObject> getDomainObjectClass(String vertexTypeName) {
        checkNotNull(vertexTypeName);

        if (!mappedDomainObjectClasses.containsKey(vertexTypeName)) {
            throw new IllegalArgumentException("There is no mapping for vertex type name: " + vertexTypeName);
        }

        return mappedDomainObjectClasses.get(vertexTypeName);
    }
}
