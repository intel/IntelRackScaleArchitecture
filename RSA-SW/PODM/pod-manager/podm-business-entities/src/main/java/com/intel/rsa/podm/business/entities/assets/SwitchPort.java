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

import com.intel.rsa.common.types.AdministrativeState;
import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.OperationalState;
import com.intel.rsa.common.types.LinkType;
import com.intel.rsa.common.types.State;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.util.Collection;

import static com.intel.rsa.common.utils.IterableHelper.single;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.integerProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;
import static javax.transaction.Transactional.TxType.MANDATORY;

@Dependent
@Transactional(MANDATORY)
public class SwitchPort extends ManagedAsset {
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<Health> HEALTH_ROLLUP = enumProperty("healthRollup", Health.class);

    public static final DomainObjectProperty<LinkType> LINK_TYPE = enumProperty("linkType", LinkType.class);
    public static final DomainObjectProperty<OperationalState> OPERATIONAL_STATE = enumProperty("operationalState", OperationalState.class);
    public static final DomainObjectProperty<AdministrativeState> ADMINISTRATIVE_STATE = enumProperty("administrativeState", AdministrativeState.class);
    public static final DomainObjectProperty<Integer> LINK_SPEED_GBPS = integerProperty("linkSpeedGbps");
    public static final DomainObjectProperty<String> NEIGHBOUR_PORT = stringProperty("neighbourPort");

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

    public Health getHealthRollup() {
        return getProperty(HEALTH_ROLLUP);
    }

    public void setHealthRollup(Health healthRollup) {
        setProperty(HEALTH_ROLLUP, healthRollup);
    }

    public Integer getLinkSpeedGbps() {
        return getProperty(LINK_SPEED_GBPS);
    }

    public OperationalState getOperationalState() {
        return getProperty(OPERATIONAL_STATE);
    }

    public AdministrativeState getAdministrativeState() {
        return getProperty(ADMINISTRATIVE_STATE);
    }

    public LinkType getLinkType() {
        return getProperty(LINK_TYPE);
    }

    public String getNeighbourPort() {
        return getProperty(NEIGHBOUR_PORT);
    }

    public void setLinkSpeedGbps(Integer value) {
        setProperty(LINK_SPEED_GBPS, value);
    }

    public void setOperationalState(OperationalState value) {
        setProperty(OPERATIONAL_STATE, value);
    }

    public void setAdministrativeState(AdministrativeState value) {
        setProperty(ADMINISTRATIVE_STATE, value);
    }

    public void setLinkType(LinkType value) {
        setProperty(LINK_TYPE, value);
    }

    public void setNeighbourPort(String neighbourPort) {
        setProperty(NEIGHBOUR_PORT, neighbourPort);
    }

    public Collection<VlanNetworkInterface> getVlans() {
        return getLinked(AssetLink.CONTAINS, VlanNetworkInterface.class);
    }

    @Override
    public Location getLocation() {
        return getSwitch().getLocation();
    }

    public Switch getSwitch() {
        try {
            return single(getLinked(AssetLink.CONTAINED_BY, Switch.class));
        } catch (IllegalStateException e) {
            throw new IllegalStateException("SwitchPort has to be contained by exactly one Switch", e);
        }
    }
}
