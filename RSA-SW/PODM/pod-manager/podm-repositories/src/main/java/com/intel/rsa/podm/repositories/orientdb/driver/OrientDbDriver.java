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

import com.intel.rsa.podm.repositories.orientdb.graph.annotation.PodGraph;
import com.intel.rsa.podm.repositories.orientdb.query.GraphQueryHelper;
import com.intel.rsa.podm.repository.api.DatabaseDriver;
import com.intel.rsa.podm.repository.api.VertexNotFoundException;
import com.orientechnologies.orient.core.id.ORecordId;
import com.orientechnologies.orient.core.record.impl.ODocument;
import com.tinkerpop.blueprints.Vertex;
import com.tinkerpop.blueprints.impls.orient.OrientGraph;
import com.tinkerpop.blueprints.impls.orient.OrientVertex;
import org.ops4j.orient.adapter.api.OrientDatabaseConnection;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * Driver for OrientDb database
 */
@Dependent
public class OrientDbDriver implements DatabaseDriver, Serializable {

    private static final String CLASS_PREFIX = "class:";
    private static final String CLASS_FIELD_NAME = "@class";
    private static final String DEPTH = "depth";
    private static final String PROPERTY_SELECTOR = ".";

    @Inject
    @PodGraph(transactional = true)
    private OrientDatabaseConnection conn;

    @Override
    public Vertex addVertex(String className) {
        OrientGraph graph = conn.ograph();
        return graph.addVertex(CLASS_PREFIX + className);
    }

    private Vertex getVertex(Object recordId) {
        return conn.ograph().getVertex(recordId);
    }

    @Override
    public Vertex getVertex(String className, Long id) {
        long clusterPosition = id - 1;
        Vertex vertex = getVertex(new ORecordId(getClusterId(className, clusterPosition), clusterPosition));
        if (vertex == null) {
            throw new VertexNotFoundException(className, id);
        }
        return vertex;
    }

    private int getClusterId(String className, long clusterPosition) {
        if (clusterPosition < 0) {
            return -1;
        }

        OrientGraph graph = conn.ograph();
        int[] clusterIds = graph.getVertexType(className).getClusterIds();
        if (clusterIds.length > 1) {
            throw new RuntimeException("Ambiguous clusters for " + className);
        }

        return clusterIds[0];
    }

    @Override
    public Long getId(Object recordId) {
        return ((ORecordId) recordId).getClusterPosition() + 1;
    }

    @Override
    public String getVertexClass(Vertex vertex) {
        return vertex.getProperty(CLASS_FIELD_NAME);
    }

    @Override
    public List<Vertex> getVerticesOfClass(String className) {
        OrientGraph graph = conn.ograph();
        final List<Vertex> vertices = new ArrayList<>();
        graph.getVerticesOfClass(className).forEach(v -> vertices.add(v));
        return vertices;
    }

    @Override
    public List<Vertex> findInSubGraphByProperty(Vertex startVertex, String searchLinkLabel, String propertyName,
                                                       Object propertyValue) {
        List<Vertex> result = new ArrayList<>();
                ((OrientVertex) startVertex)
                .traverse()
                .fields(GraphQueryHelper.convertToTraverseBothDirectionFields(searchLinkLabel))
                        .predicate((record, currentResult, context) -> {
                            if (propertyValue.equals(((ODocument) record).field(propertyName))) {
                                result.add(getVertex(record.getIdentity()));
                            }
                            return true;
                        })
                .execute();
        return result;
    }

    // TODO create separate class for simple searching functionality and move "find" methods there
    @Override
    public List<Vertex> findByProperty(String className, String propertyName, Object propertyValue) {
        OrientGraph graph = conn.ograph();
        List<Vertex> vertices = new ArrayList<>();
        graph.getVertices(className + PROPERTY_SELECTOR + propertyName, propertyValue).forEach(v -> vertices.add(v));
        return vertices;
    }

    @Override
    public List<Vertex> findByFlag(String className, String propertyName, boolean value) {
        List<Vertex> vertices = getVerticesOfClass(className);
        List<Vertex> flagged = new ArrayList<>();
        for (Vertex vertex : vertices) {
            Boolean flag = vertex.getProperty(propertyName);
            boolean flagValue = (flag != null) ? flag : false;
            if (flagValue == value) {
                flagged.add(vertex);
            }
        }
        return flagged;
    }

    @Override
    public void deleteVertex(Vertex vertex) {
        // TODO this removes also edges related to this vertex
        conn.ograph().removeVertex(vertex);
    }
}
