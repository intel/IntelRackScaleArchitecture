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

package com.intel.rsa.podm.repositories.orientdb.driver;

import com.intel.rsa.podm.repository.api.GraphSchemaManager;
import com.intel.rsa.podm.repository.api.GraphVertexType;
import com.orientechnologies.orient.core.metadata.schema.OProperty;
import com.orientechnologies.orient.core.metadata.schema.OType;
import com.tinkerpop.blueprints.impls.orient.OrientExtendedGraph;
import com.tinkerpop.blueprints.impls.orient.OrientVertexType;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import static com.google.common.collect.Sets.difference;

@Dependent
public class GraphSchemaManagerImpl implements GraphSchemaManager {

    @Inject
    OrientExtendedGraph orientGraph;

    @Inject
    OrientGraphDataRemover orientGraphDataRemover;

    @Override
    public void registerVertexType(GraphVertexType vertexType) {
        if (vertexType == null) {
            throw new IllegalArgumentException("Argument vertexType should not be null!");
        }

        OrientVertexType orientVertexType = orientGraph.getVertexType(vertexType.getVertexTypeName());

        if (orientVertexType == null) {
            createOrientVertexType(vertexType);
        } else {
            updateOrientVertexType(orientVertexType, vertexType);
            // TODO consider moving directly to schema initialization during application startup
            orientGraphDataRemover.truncateOrientVertexType(vertexType.getVertexTypeName());
        }
    }

    private void updateOrientVertexType(OrientVertexType orientVertexType, GraphVertexType vertexType) {
        Map<String, Class> orientPropertiesMap = orientPropertiesToMap(orientVertexType.declaredProperties());
        Map<String, Class> vertexPropertiesMap = vertexType.getVertexProperties();

        Iterable<Map.Entry<String, Class>> removedOrChangedProperties = difference(orientPropertiesMap.entrySet(), vertexPropertiesMap.entrySet());
        Iterable<Map.Entry<String, Class>> addedProperties = difference(vertexPropertiesMap.entrySet(), orientPropertiesMap.entrySet());

        removeMultipleProperties(orientVertexType, removedOrChangedProperties);
        addMultipleProperties(orientVertexType, addedProperties);
    }

    private void removeMultipleProperties(OrientVertexType orientVertexType, Iterable<Map.Entry<String, Class>> removedProperties) {
        for (Map.Entry<String, Class> removedProperty : removedProperties) {
            orientVertexType.dropProperty(removedProperty.getKey());
        }
    }

    private void addMultipleProperties(OrientVertexType orientVertexType, Iterable<Map.Entry<String, Class>> newProperties) {
        for (Map.Entry<String, Class> newProperty : newProperties) {
            orientVertexType.createProperty(newProperty.getKey(), OType.getTypeByClass(newProperty.getValue()));
        }
    }

    private void createOrientVertexType(GraphVertexType vertexType) {
        OrientVertexType registeredVertexType = orientGraph.createVertexType(vertexType.getVertexTypeName());
        if (vertexType.getVertexProperties() != null) {
            addMultipleProperties(registeredVertexType, vertexType.getVertexProperties().entrySet());
        }
    }

    private Map<String, Class> orientPropertiesToMap(Collection<OProperty> properties) {
        HashMap<String, Class> propertiesMap = new HashMap<>();

        for (OProperty property : properties) {
            propertiesMap.put(property.getName(), property.getType().getDefaultJavaType());
        }

        return propertiesMap;
    }
}
