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

package com.intel.rsa.podm.business.entities.base;


import javax.enterprise.inject.Instance;
import javax.enterprise.inject.New;
import javax.inject.Inject;

public class SingletonDomainObjectProvider<T extends SingletonDomainObject> {

    @Inject
    @New
    private Instance<T> domainObjects;

    private T cachedDomainObject;

    public T getInstance() {
        if (cachedDomainObject == null) {
            T domainObject = domainObjects.get();
            domainObject.initializeOrCreateAsSingleton();
            cachedDomainObject = domainObject;
        }

        return cachedDomainObject;
    }
}
