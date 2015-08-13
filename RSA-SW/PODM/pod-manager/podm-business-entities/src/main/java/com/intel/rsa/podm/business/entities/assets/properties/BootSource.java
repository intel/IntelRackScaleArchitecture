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

import com.intel.rsa.common.types.BootSourceType;
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class BootSource extends DomainObject {
    public static final DomainObjectProperty<BootSourceType> BOOT_SOURCE_TYPE = enumProperty("bootSourceType", BootSourceType.class);

    public BootSourceType getBootSourceType() {
        return getProperty(BOOT_SOURCE_TYPE);
    }

    public void setBootSourceType(BootSourceType bootSourceType) {
        setProperty(BOOT_SOURCE_TYPE, bootSourceType);
    }
}
