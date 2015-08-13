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

import com.intel.rsa.common.types.FormFactor;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.MemoryType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.math.BigDecimal;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.decimalProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class Memory extends Asset {
    public static final DomainObjectProperty<String> SOCKET = stringProperty("socket");
    public static final DomainObjectProperty<MemoryType> TYPE = enumProperty("type", MemoryType.class);
    public static final DomainObjectProperty<Integer> SIZE_GB = integerProperty("sizeGB");
    public static final DomainObjectProperty<Integer> SPEED_MHZ = integerProperty("speedMHz");
    public static final DomainObjectProperty<BigDecimal> VOLTAGE_VOLT = decimalProperty("voltageVolt");
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<String> MANUFACTURER = stringProperty("manufacturer");
    public static final DomainObjectProperty<String> BANK = stringProperty("bank");
    public static final DomainObjectProperty<Integer> DATA_WIDTH_BITS = integerProperty("dataWidthBits");
    public static final DomainObjectProperty<Integer> TOTAL_WIDTH_BITS = integerProperty("totalWidthBits");
    public static final DomainObjectProperty<FormFactor> FORM_FACTOR = enumProperty("formFactor", FormFactor.class);
    public static final DomainObjectProperty<String> SERIAL_NUMBER = stringProperty("serialNumber");
    public static final DomainObjectProperty<String> ASSET_TAG = stringProperty("assetTag");
    public static final DomainObjectProperty<String> PART_NUMBER = stringProperty("partNumber");
    public static final DomainObjectProperty<String> RANK = stringProperty("rank");
    public static final DomainObjectProperty<Integer> CONFIGURED_SPEED_MHZ = integerProperty("configuredSpeedMHz");
    public static final DomainObjectProperty<BigDecimal> MINIMUM_VOLTAGE_VOLT = decimalProperty("minimumVoltageVolt");
    public static final DomainObjectProperty<BigDecimal> MAXIMUM_VOLTAGE_VOLT = decimalProperty("maximumVoltageVolt");

    public Blade getBlade() {
        try {
            return single(getLinked(AssetLink.CONTAINED_BY, Blade.class));
        } catch (IllegalStateException e) {
            throw new IllegalStateException("Memory has to be contained by exactly one Blade", e);
        }
    }

    @Override
    public Location getLocation() {
        return getBlade().getLocation();
    }

    public String getSocket() {
        return getProperty(SOCKET);
    }

    public void setSocket(String socket) {
        setProperty(SOCKET, socket);
    }

    public MemoryType getType() {
        return getProperty(TYPE);
    }

    public void setType(MemoryType type) {
        setProperty(TYPE, type);
    }

    public Integer getSizeGB() {
        return getProperty(SIZE_GB);
    }

    public void setSizeGB(Integer sizeGB) {
        setProperty(SIZE_GB, sizeGB);
    }

    public Integer getSpeedMHz() {
        return getProperty(SPEED_MHZ);
    }

    public void setSpeedMHz(Integer speedMHz) {
        setProperty(SPEED_MHZ, speedMHz);
    }

    public BigDecimal getVoltageVolt() {
        return getProperty(VOLTAGE_VOLT);
    }

    public void setVoltageVolt(BigDecimal voltageVolt) {
        setProperty(VOLTAGE_VOLT, voltageVolt);
    }

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public Health getHealth() {
        return getProperty(HEALTH);
    }

    public void setHealth(Health health) {
        setProperty(HEALTH, health);
    }

    public String getManufacturer() {
        return getProperty(MANUFACTURER);
    }

    public void setManufacturer(String manufacturer) {
        setProperty(MANUFACTURER, manufacturer);
    }

    public String getBank() {
        return getProperty(BANK);
    }

    public void setBank(String bank) {
        setProperty(BANK, bank);
    }

    public Integer getDataWidthBits() {
        return getProperty(DATA_WIDTH_BITS);
    }

    public void setDataWidthBits(Integer dataWidthBits) {
        setProperty(DATA_WIDTH_BITS, dataWidthBits);
    }

    public Integer getTotalWidthBits() {
        return getProperty(TOTAL_WIDTH_BITS);
    }

    public void setTotalWidthBits(Integer totalWidthBits) {
        setProperty(TOTAL_WIDTH_BITS, totalWidthBits);
    }

    public FormFactor getFormFactor() {
        return getProperty(FORM_FACTOR);
    }

    public void setFormFactor(FormFactor formFactor) {
        setProperty(FORM_FACTOR, formFactor);
    }

    public String getSerialNumber() {
        return getProperty(SERIAL_NUMBER);
    }

    public void setSerialNumber(String serialNumber) {
        setProperty(SERIAL_NUMBER, serialNumber);
    }

    public String getAssetTag() {
        return getProperty(ASSET_TAG);
    }

    public void setAssetTag(String assetTag) {
        setProperty(ASSET_TAG, assetTag);
    }

    public String getPartNumber() {
        return getProperty(PART_NUMBER);
    }

    public void setPartNumber(String partNumber) {
        setProperty(PART_NUMBER, partNumber);
    }

    public String getRank() {
        return getProperty(RANK);
    }

    public void setRank(String rank) {
        setProperty(RANK, rank);
    }

    public Integer getConfiguredSpeedMhz() {
        return getProperty(CONFIGURED_SPEED_MHZ);
    }

    public void setConfiguredSpeedMhz(Integer configuredSpeedMhz) {
        setProperty(CONFIGURED_SPEED_MHZ, configuredSpeedMhz);
    }

    public BigDecimal getMinimumVoltageVolt() {
        return getProperty(MINIMUM_VOLTAGE_VOLT);
    }

    public void setMinimumVoltageVolt(BigDecimal minimumVoltageVolt) {
        setProperty(MINIMUM_VOLTAGE_VOLT, minimumVoltageVolt);
    }

    public BigDecimal getMaximumVoltageVolt() {
        return getProperty(MAXIMUM_VOLTAGE_VOLT);
    }

    public void setMaximumVoltageVolt(BigDecimal maximumVoltageVolt) {
        setProperty(MAXIMUM_VOLTAGE_VOLT, maximumVoltageVolt);
    }
}
