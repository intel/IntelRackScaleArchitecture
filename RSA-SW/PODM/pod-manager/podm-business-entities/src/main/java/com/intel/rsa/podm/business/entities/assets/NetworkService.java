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

import com.intel.rsa.common.types.Health;
import com.intel.rsa.common.types.State;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;
import com.intel.rsa.podm.business.entities.base.Link;
import com.tinkerpop.blueprints.Direction;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.net.URI;
import java.util.List;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.enumProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

/**
 * Network Service resource of Manager.
 */
@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class NetworkService extends Asset implements Discoverable {
    public static final DomainObjectProperty<Health> HEALTH = enumProperty("health", Health.class);
    public static final DomainObjectProperty<State> STATE = enumProperty("state", State.class);
    public static final DomainObjectProperty<String> DESCRIPTION = stringProperty("description");
    public static final DomainObjectProperty<String> HOST_NAME = stringProperty("hostName");
    public static final DomainObjectProperty<String> FQDN = stringProperty("fqdn");

    public Health getHealth() {
        return getProperty(HEALTH);
    }

    public void setHealth(Health health) {
        setProperty(HEALTH, health);
    }

    public State getState() {
        return getProperty(STATE);
    }

    public void setState(State state) {
        setProperty(STATE, state);
    }

    public String getDescription() {
        return getProperty(DESCRIPTION);
    }

    public void setDescription(String description) {
        setProperty(DESCRIPTION, description);
    }

    public String getHostName() {
        return getProperty(HOST_NAME);
    }

    public void setHostName(String hostName) {
        setProperty(HOST_NAME, hostName);
    }

    public String getFqdn() {
        return getProperty(FQDN);
    }

    public void setFqdn(String fqdn) {
        setProperty(FQDN, fqdn);
    }

    public NetworkServiceProtocol getHttp() {
        return getProtocol(Protocols.HTTP);
    }

    public void setHttp(Boolean enabled, Integer port) {
        setProtocol(Protocols.HTTP, enabled, port);
    }

    public NetworkServiceProtocol getHttps() {
        return getProtocol(Protocols.HTTPS);
    }

    public void setHttps(Boolean enabled, Integer port) {
        setProtocol(Protocols.HTTPS, enabled, port);
    }

    public NetworkServiceProtocol getIpmi() {
        return getProtocol(Protocols.IPMI);
    }

    public void setIpmi(Boolean enabled, Integer port) {
        setProtocol(Protocols.IPMI, enabled, port);
    }

    public NetworkServiceProtocol getSsh() {
        return getProtocol(Protocols.SSH);
    }

    public void setSsh(Boolean enabled, Integer port) {
        setProtocol(Protocols.SSH, enabled, port);
    }

    public NetworkServiceProtocol getSnmp() {
        return getProtocol(Protocols.SNMP);
    }

    public void setSnmp(Boolean enabled, Integer port) {
        setProtocol(Protocols.SNMP, enabled, port);
    }

    public NetworkServiceProtocol getVirtualMedia() {
        return getProtocol(Protocols.VIRTUAL_MEDIA);
    }

    public void setVirtualMedia(Boolean enabled, Integer port) {
        setProtocol(Protocols.VIRTUAL_MEDIA, enabled, port);
    }

    public NetworkServiceProtocol getSsdp() {
        return getProtocol(Protocols.SSDP);
    }

    public void setSsdp(Boolean enabled, Integer port) {
        setProtocol(Protocols.SSDP, enabled, port);
    }

    public NetworkServiceProtocol getTelnet() {
        return getProtocol(Protocols.TELNET);
    }

    public void setTelnet(Boolean enabled, Integer port) {
        setProtocol(Protocols.TELNET, enabled, port);
    }

    public NetworkServiceProtocol getKvmip() {
        return getProtocol(Protocols.KVMIP);
    }

    public void setKvmip(Boolean enabled, Integer port) {
        setProtocol(Protocols.KVMIP, enabled, port);
    }

    private void setProtocol(Protocols protocolLink, Boolean enabled, Integer port) {
        NetworkServiceProtocol protocol = getProtocol(protocolLink);
        protocol.setEnabled(enabled);
        protocol.setPort(port);
    }

    private NetworkServiceProtocol getProtocol(Protocols protocolLink) {
        List<NetworkServiceProtocol> linked = getLinked(protocolLink);
        if (linked.isEmpty()) {
            return addDomainObject(protocolLink, NetworkServiceProtocol.class);
        }
        return linked.get(0);
    }

    @Override
    public URI getSourceUri() {
        return getProperty(SOURCE_URI);
    }

    @Override
    public void setSourceUri(URI sourceUri) {
        setProperty(SOURCE_URI, sourceUri);
    }

    private enum Protocols implements Link {
        HTTP,
        HTTPS,
        IPMI,
        SSH,
        SNMP,
        VIRTUAL_MEDIA,
        SSDP,
        TELNET,
        KVMIP;

        @Override
        public String getLabel() {
            return name();
        }

        @Override
        public Direction getDirection() {
            return Direction.OUT;
        }
    }
}
