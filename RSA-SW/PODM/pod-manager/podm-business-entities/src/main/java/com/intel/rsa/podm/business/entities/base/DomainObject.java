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

import com.intel.rsa.common.enterprise.utils.proxy.Unproxier;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.podm.business.entities.ogm.OgmManager;
import com.intel.rsa.podm.repository.api.DatabaseDriver;
import com.tinkerpop.blueprints.Direction;
import com.tinkerpop.blueprints.Vertex;

import javax.inject.Inject;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

import static com.intel.rsa.common.types.Id.id;

/**
 * Domain object base class. Not to be instantiated (not abstract because of JEE limitations)
 */
@AbstractDomainObject
public class DomainObject {
    protected Vertex vertex;

    @Inject
    protected DatabaseDriver dbDriver;

    @Inject
    protected DomainObjectRepository repository;

    @Inject
    protected OgmManager ogmManager;

    /**
     * @deprecated it will be removed in the next iteration of refactoring
     */
    @Deprecated
    public List<Link> getLinks() {
        return Collections.EMPTY_LIST;
    }

    public Id getId() {
        return id(dbDriver.getId(vertex.getId()));
    }

    void initializeAsNewInstance() {
        vertex = dbDriver.addVertex(getVertexClassName());
    }

    void initializeById(Id id) {
        vertex = dbDriver.getVertex(getVertexClassName(), id.getValue());
    }

    protected  <T extends DomainObject> T addDomainObject(Link link, Class<T> domainObjectClass) {
        T domainObject = repository.create(domainObjectClass);

        link(link, domainObject);
        return domainObject;
    }

    protected void link(Link link, DomainObject target) {
        Vertex targetVertex = target.getVertex();
        String label = link.getLabel();

        if (link.getDirection() == Direction.IN) {
            linkVertices(targetVertex, this.vertex, label);
        } else {
            linkVertices(vertex, targetVertex, label);
        }
    }

    protected <T extends DomainObject> List<T> getLinked(Link link) {
        List<T> linkedDos = new ArrayList<>();
        vertex.getVertices(link.getDirection(), link.getLabel()).forEach(v -> appendDomainObject(linkedDos, v));
        return Collections.unmodifiableList(linkedDos);
    }

    protected <T extends DomainObject> List<T> getLinked(Link link, Class<T> domainObjectClass) {
        List<T> linkedDos = new ArrayList<>();

        //TODO it should be moved to OgmManager
        String vertexClassName = domainObjectClass.getSimpleName();

        vertex.getVertices(link.getDirection(), link.getLabel()).forEach(v -> {
            if (dbDriver.getVertexClass(v).equals(vertexClassName)) {
                appendDomainObject(linkedDos, v);
            }
        });

        return Collections.unmodifiableList(linkedDos);
    }

    private <T extends DomainObject> void appendDomainObject(List<T> domainObjects, Vertex vertex) {
        Id domainObjectId = id(dbDriver.getId(vertex.getId()));
        Class<T> domainObjectClass = (Class<T>) ogmManager.getDomainObjectClass(dbDriver.getVertexClass(vertex));
        domainObjects.add(repository.get(domainObjectClass, domainObjectId));
    }

    /**
     * Searches for linked DomainObjects having provided property. Only objects linked with traverseLink are considered.
     * @param traverseLink link to traverse between objects
     * @param property property to search for
     * @param propertyValue property value to search for
     * @return matching domain objects
     */
    protected <T extends DomainObject, U> List<T> findByProperty(Link traverseLink, DomainObjectProperty<U> property, U propertyValue) {
        List<T> domainObjects = new ArrayList<>();
        dbDriver.findInSubGraphByProperty(vertex, traverseLink.getLabel(), property.getName(), propertyValue).forEach(
                v -> appendDomainObject(domainObjects, v));
        return domainObjects;
    }

    protected <T> T getProperty(DomainObjectProperty<T> property) {
        Object value = vertex.getProperty(property.getName());

        return  (T) property.convertToMappedType(value);
    }

    /**
     * Method stores a single domain object property.
     * If the value passed is null, the property is removed.
     *
     * @param property domain object property definition
     * @param value value to be set for the property
     * @param <T> type of value passed
     */
    protected <T> void setProperty(DomainObjectProperty<T> property, T value) {
        if (value != null) {
            Object convertedValue = property.convertFromMappedType(value);
            vertex.setProperty(property.getName(), convertedValue);
        } else {
            removeProperty(property);
        }
    }

    protected <T> T removeProperty(DomainObjectProperty<T> property) {
        return vertex.removeProperty(property.getName());
    }

    protected Vertex getVertex() {
        return vertex;
    }

    protected String getVertexClassName() {
        return new Unproxier().unproxy(this.getClass()).getSimpleName();
    }

    @Override
    public String toString() {
        return "DomainObject{" + "vertex=" + vertex + '}';
    }

    private static void linkVertices(Vertex vertex, Vertex targetVertex, String label) {
        for (Vertex outVertex : vertex.getVertices(Direction.OUT, label)) {
            if (Objects.equals(outVertex.getId(), targetVertex.getId())) {
                return;
            }
        }

        vertex.addEdge(label, targetVertex);
    }
}
