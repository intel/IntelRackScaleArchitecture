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

package com.intel.rsa.common.enterprise.utils.proxy;

import javax.enterprise.context.Dependent;

import static com.google.common.base.Preconditions.checkNotNull;

/**
 * Contains method allowing to determine what is underlying class of class proxied by JEE container.
 */
@Dependent
public class Unproxier {
    public <T> Class<T> unproxy(Class<? extends T> proxiedClass) {
        checkNotNull(proxiedClass, "Proxied class should not be null");

        String name = proxiedClass.getSimpleName();

        if (name.contains("$")) {
            return (Class<T>) proxiedClass.getSuperclass();
        }

        return (Class<T>) proxiedClass;
    }
}
