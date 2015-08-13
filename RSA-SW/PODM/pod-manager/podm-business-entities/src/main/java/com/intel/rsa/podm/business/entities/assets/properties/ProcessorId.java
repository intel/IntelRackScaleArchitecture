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

import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.longProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class ProcessorId extends DomainObject {

    public static final DomainObjectProperty<String> VENDOR_ID = stringProperty("vendorId");
    public static final DomainObjectProperty<Integer> NUMERIC_ID = integerProperty("numericId");
    public static final DomainObjectProperty<Integer> FAMILY = integerProperty("family");
    public static final DomainObjectProperty<Integer> MODEL = integerProperty("model");
    public static final DomainObjectProperty<Integer> STEP = integerProperty("step");
    public static final DomainObjectProperty<Long> MICROCODE_INFO = longProperty("microcodeInfo");

    public String getVendorId() {
        return getProperty(VENDOR_ID);
    }

    public void setVendorId(String vendorId) {
        setProperty(VENDOR_ID, vendorId);
    }

    public Integer getNumericId() {
        return getProperty(NUMERIC_ID);
    }

    public void setNumericId(Integer numericId) {
        setProperty(NUMERIC_ID, numericId);
    }

    public Integer getFamily() {
        return getProperty(FAMILY);
    }

    public void setFamily(Integer family) {
        setProperty(FAMILY, family);
    }

    public Integer getModel() {
        return getProperty(MODEL);
    }

    public void setModel(Integer model) {
        setProperty(MODEL, model);
    }

    public Integer getStep() {
        return getProperty(STEP);
    }

    public void setStep(Integer step) {
        setProperty(STEP, step);
    }

    public Long getMicrocodeInfo() {
        return getProperty(MICROCODE_INFO);
    }

    public void setMicrocodeInfo(Long microcodeInfo) {
        setProperty(MICROCODE_INFO, microcodeInfo);
    }
}
