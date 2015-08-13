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

import com.google.common.collect.HashBasedTable;
import com.intel.rsa.common.enterprise.utils.beans.BeanFactory;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.utils.IterableHelper;
import com.intel.rsa.podm.business.entities.DomainObjectNotFoundException;
import com.intel.rsa.podm.repository.api.DatabaseDriver;
import com.intel.rsa.podm.repository.api.VertexNotFoundException;
import com.tinkerpop.blueprints.Vertex;

import javax.inject.Inject;
import javax.transaction.TransactionScoped;
import javax.transaction.Transactional;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import static com.google.common.base.Preconditions.checkNotNull;
import static javax.transaction.Transactional.TxType.MANDATORY;

/**
 * Allows to obtain or create instance of domain objects in state associated with current transaction
 */
@TransactionScoped
@Transactional(MANDATORY)
public class DomainObjectRepository implements Serializable {

    @Inject
    DatabaseDriver dbDriver;

    @Inject
    BeanFactory factory;

    private final HashBasedTable<Id, Class<? extends DomainObject>, DomainObject> cachedObjects = HashBasedTable.create();

    public <T extends DomainObject> T create(Class<T> domainObjectClass) {
        checkNotNull(domainObjectClass, "domainObjectClass should not be null");

        T domainObject = factory.create(domainObjectClass);
        domainObject.initializeAsNewInstance();
        return domainObject;
    }

    /**
     *  gets domain object instance by its id.
     *  For given domain object class and id it always returns the same reference
     */
    public <T extends DomainObject> T get(Class<T> domainObjectClass, Id id) {
        checkNotNull(domainObjectClass, "domainObjectClass should not be null");
        checkNotNull(id, "id should not be null");

        if (cachedObjects.contains(id, domainObjectClass)) {
            return (T) cachedObjects.get(id, domainObjectClass);
        }

        T domainObject = factory.create(domainObjectClass);
        try {
            domainObject.initializeById(id);
        } catch (VertexNotFoundException e) {
            throw new DomainObjectNotFoundException(e);
        }

        cachedObjects.put(id, domainObjectClass, domainObject);

        return domainObject;
    }

    /**
     * Method gets all domain objects of given class
     *
     * @param domainObjectClass class of Domain Object
     * @return List of initialized Domain Objects
     */
    public <T extends DomainObject> List<T> getAll(Class<T> domainObjectClass) {
        checkNotNull(domainObjectClass, "domainObjectClass should not be null");

        List<Vertex> vertices = dbDriver.getVerticesOfClass(domainObjectClass.getSimpleName());
        List<T> domainObjectList = new ArrayList<>();
        for (Vertex vertex : vertices) {
            Long id = dbDriver.getId(vertex.getId());
            T domainObject = get(domainObjectClass, Id.id(id));
            domainObjectList.add(domainObject);
        }

        return domainObjectList;
    }

    /**
     * Finds single domain object by given property.
     *
     * @param domainObjectClass type of domain object
     * @param domainObjectProperty property to search by
     * @param propertyValue property value to search by
     * @return matching domain object or null if not found
     * @throws IllegalStateException if more then one match found
     */
    public <T extends DomainObject, U> T getSingleByProperty(Class<T> domainObjectClass, DomainObjectProperty<U> domainObjectProperty, U propertyValue) {
        List<Vertex> vertices = getVerticesOfClassByProperty(domainObjectClass, domainObjectProperty, propertyValue);
        Vertex vertex = IterableHelper.singleOrNull(vertices);
        if (vertex == null) {
            return null;
        }
        Long id = dbDriver.getId(vertex.getId());
        return get(domainObjectClass, Id.id(id));
    }

    private <T extends DomainObject, U> List<Vertex> getVerticesOfClassByProperty(Class<T> domainObjectClass,
                                                                                  DomainObjectProperty<U> domainObjectProperty,
                                                                                  U propertyValue) {
        checkNotNull(domainObjectClass, "domainObjectClass should not be null");
        checkNotNull(domainObjectProperty, "domainObjectProperty should not be null");
        checkNotNull(propertyValue, "propertyValue should not be null");

        Object value = domainObjectProperty.convertFromMappedType(propertyValue);
        return dbDriver.findByProperty(domainObjectClass.getSimpleName(), domainObjectProperty.getName(), value);
    }

    /**
     * Finds all domain objects by given property
     *
     * @param domainObjectClass type of domain object
     * @param domainObjectProperty property to search by
     * @param propertyValue property value to search by
     * @return matching domain objects or empty list if not found
     */
    public <T extends DomainObject, U> List<T> getAllByProperty(Class<T> domainObjectClass, DomainObjectProperty<U> domainObjectProperty, U propertyValue) {
        List<Vertex> vertices = getVerticesOfClassByProperty(domainObjectClass, domainObjectProperty, propertyValue);

        List<T> domainObjectList = new ArrayList<>();
        for (Vertex vertex : vertices) {
            Long id = dbDriver.getId(vertex.getId());
            T domainObject = get(domainObjectClass, Id.id(id));
            domainObjectList.add(domainObject);
        }

        return domainObjectList;
    }


    /**
     * Finds all domain objects using boolean flag. DomainObject without this flag set are treated as having false flag value.
     *
     * @param domainObjectClass type of domain object
     * @param domainObjectFlag boolean property to search by
     * @param value boolean property value to search by
     * @return List of initialized Domain Objects
     */
    public <T extends DomainObject, U> List<T> getAllByFlag(Class<T> domainObjectClass, DomainObjectProperty<Boolean> domainObjectFlag, boolean value) {
        checkNotNull(domainObjectClass, "domainObjectClass should not be null");
        checkNotNull(domainObjectFlag, "domainObjectProperty should not be null");
        checkNotNull(value, "propertyValue should not be null");

        List<Vertex> vertices = dbDriver.findByFlag(domainObjectClass.getSimpleName(), domainObjectFlag.getName(), value);
        List<T> domainObjectList = new ArrayList<>();
        for (Vertex vertex : vertices) {
            Long id = dbDriver.getId(vertex.getId());
            T domainObject = get(domainObjectClass, Id.id(id));
            domainObjectList.add(domainObject);
        }

        return domainObjectList;
    }

    /**
     * Checks whether domain object exists
     *
     * @param domainObjectClass Domain Object class
     * @param id Domain Object ID
     * @return true if Domain Objects exists; false otherwise
     */
    public boolean exists(Class<? extends DomainObject> domainObjectClass, Id id) {
        try {
            get(domainObjectClass, id);
            return true;
        } catch (DomainObjectNotFoundException e) {
            return false;
        }
    }

    /**
     * Removes object of given class and Id from repository (both from database and domain object cache)
     *
     * @param domainObjectClass class of Domain Object to be deleted
     * @param id Id of domain object to be deleted
     */
    public <T extends DomainObject> void delete(Class<T> domainObjectClass, Id id) {
        T domainObject = get(domainObjectClass, id);
        cachedObjects.remove(id, domainObjectClass);

        Vertex vertex = domainObject.getVertex();
        dbDriver.deleteVertex(vertex);
    }
}
