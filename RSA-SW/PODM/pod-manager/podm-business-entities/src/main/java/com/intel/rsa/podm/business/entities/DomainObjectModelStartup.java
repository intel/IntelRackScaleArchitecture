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

package com.intel.rsa.podm.business.entities;

import com.intel.rsa.common.enterprise.utils.proxy.Unproxier;
import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.business.entities.base.AbstractDomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.SingletonDomainObject;
import com.intel.rsa.podm.business.entities.base.SingletonDomainObjectProvider;
import com.intel.rsa.podm.business.entities.ogm.OgmManager;

import javax.annotation.PostConstruct;
import javax.ejb.Singleton;
import javax.ejb.Startup;
import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;
import javax.enterprise.inject.Instance;
import javax.inject.Inject;

@Singleton
@Startup
@TransactionAttribute(TransactionAttributeType.NOT_SUPPORTED)
public class DomainObjectModelStartup {
    @Inject
    private OgmManager ogmManager;

    @Inject
    private Instance<DomainObject> domainObjects;

    @Inject
    private Instance<SingletonDomainObjectProvider<? extends SingletonDomainObject>> singletonProviders;

    @Inject
    private Unproxier unproxier;

    @Inject
    private Logger logger;

    @PostConstruct
    public void initializeDomainObjectModel() {
        createSchema();
        initializeSingletons();
    }

    private void createSchema() {

        domainObjects.forEach(domainObject -> {
            if (!domainObject.getClass().isAnnotationPresent(AbstractDomainObject.class)) {
                Class<DomainObject> domainObjectClass = unproxier.unproxy(domainObject.getClass());
                logger.d("Creating schema for " + domainObjectClass);
                ogmManager.addToMapping(domainObjectClass);
            }
        });
    }

    private void initializeSingletons() {
        for (SingletonDomainObjectProvider<? extends SingletonDomainObject> singletonProvider : singletonProviders) {
            singletonProvider.getInstance();
        }
    }
}
