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

import com.intel.rsa.common.types.EnumStatus;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.util.Collection;
import java.util.Objects;
import java.util.Optional;

import static com.google.common.base.Preconditions.checkNotNull;
import static com.intel.rsa.common.types.locations.Locations.rackLocation;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class Pod extends ManagedAsset {
    public static final DomainObjectProperty<String> MANUFACTURER = stringProperty("manufacturer");
    public static final DomainObjectProperty<String> MODEL = stringProperty("model");
    public static final DomainObjectProperty<String> SERIAL_NUMBER = stringProperty("serialNumber");
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<EnumStatus> ENUM_STATUS = enumProperty("enumStatus", EnumStatus.class);

    private static final String RSA_RACK_DEFAULT_NAME = "RSA Rack";

    public Collection<Rack> getRacks() {
        return getLinked(AssetLink.CONTAINS, Rack.class);
    }

    public Rack getRack(Id id) {
        return DomainObjectHelper.findById(id, getRacks());
    }

    public Rack getRack(Integer locationCoord) {
        checkNotNull(locationCoord);
        Optional<Rack> rack = getRacks().stream().filter(r -> Objects.equals(r.getLocation().getRack(), locationCoord)).findFirst();
        return rack.isPresent() ? rack.get() : null;
    }

    public Rack createRack(Integer locationCoord) {
        checkNotNull(locationCoord);
        if (getRack(locationCoord) != null) {
            throw new RuntimeException("Rack already exists under location:" + locationCoord);
        }
        Rack rack = addDomainObject(AssetLink.CONTAINS, Rack.class);
        rack.setName(RSA_RACK_DEFAULT_NAME);
        rack.updateModified();
        rack.setLocation(rackLocation(getLocation().getPod(), locationCoord));

        //TODO: Provide real data when available
        rack.setState(State.ENABLED);
        rack.setHealth(Health.OK);
        return rack;
    }

    public String getManufacturer() {
        return getProperty(MANUFACTURER);
    }

    public void setManufacturer(String manufacturer) {
        setProperty(MANUFACTURER, manufacturer);
    }

    public Health getHealth() {
        return getProperty(HEALTH);
    }

    public void setHealth(Health health) {
        setProperty(HEALTH, health);
    }

    public String getModel() {
        return getProperty(MODEL);
    }

    public void setModel(String model) {
        setProperty(MODEL, model);
    }

    public String getSerialNumber() {
        return getProperty(SERIAL_NUMBER);
    }

    public void setSerialNumber(String serialNumber) {
        setProperty(SERIAL_NUMBER, serialNumber);
    }

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public EnumStatus getEnumStatus() {
        return getProperty(ENUM_STATUS);
    }

    public void setEnumStatus(EnumStatus enumStatus) {
        setProperty(ENUM_STATUS, enumStatus);
    }
}
