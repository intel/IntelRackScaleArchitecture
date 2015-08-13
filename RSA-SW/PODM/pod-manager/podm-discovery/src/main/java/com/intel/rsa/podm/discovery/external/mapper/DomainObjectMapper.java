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

package com.intel.rsa.podm.discovery.external.mapper;

import com.intel.rsa.client.api.reader.RsaResource;
import com.intel.rsa.common.mapper.BrilliantMapper;
import com.intel.rsa.podm.business.entities.base.DomainObject;

public abstract class DomainObjectMapper<S extends RsaResource, T extends DomainObject> extends BrilliantMapper<S, T> {
    protected S source;
    protected T target;
    private final Class<S> sourceClass;
    private final Class<T> targetClass;

    protected DomainObjectMapper(Class<S> sourceClass, Class<T> targetClass) {
        this.sourceClass = sourceClass;
        this.targetClass = targetClass;
    }

    public Class<S> getSourceClass() {
        return sourceClass;
    }

    public Class<T> getTargetClass() {
        return targetClass;
    }

    @Override
    public void map(S source, T target) {
        this.source = source;
        this.target = target;
        super.map(source, target);
    }
}
