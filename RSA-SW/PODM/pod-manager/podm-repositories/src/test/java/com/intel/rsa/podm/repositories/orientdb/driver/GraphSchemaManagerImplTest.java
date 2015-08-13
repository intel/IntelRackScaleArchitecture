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

import com.intel.rsa.podm.repository.api.GraphVertexType;
import com.orientechnologies.orient.core.metadata.schema.OProperty;
import com.orientechnologies.orient.core.metadata.schema.OType;
import com.tinkerpop.blueprints.impls.orient.OrientExtendedGraph;
import com.tinkerpop.blueprints.impls.orient.OrientVertexType;
import org.testng.annotations.Test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

public class GraphSchemaManagerImplTest {

    @Test(expectedExceptions = IllegalArgumentException.class, expectedExceptionsMessageRegExp = ".*vertexType.*")
    public void whenRegisteringVertexTypeAndPassingNull_ShouldThrow() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();
        graphSchemaManager.registerVertexType(null);
    }

    @Test
    public void whenRegisteringVertexType_ShouldCallCreateVertexTypeFromOrientGraph() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", Collections.emptyMap());

        graphSchemaManager.registerVertexType(vertexType);

        verify(graphSchemaManager.orientGraph).createVertexType(eq(vertexType.getVertexTypeName()));
    }

    @Test
    public void whenRegisteringTheSameVertexTypeTwice_ShouldCreateVertexTypeOnlyOnce() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", Collections.emptyMap());

        graphSchemaManager.registerVertexType(vertexType);

        when(graphSchemaManager.orientGraph.getVertexType(vertexType.getVertexTypeName())).thenReturn(mock(OrientVertexType.class));

        graphSchemaManager.registerVertexType(vertexType);

        verify(graphSchemaManager.orientGraph, times(1)).createVertexType(eq(vertexType.getVertexTypeName()));
    }

    @Test
    public void whenRegisteringTwoDifferentVertexTypesTwice_ShouldCallCreateVertexTypeFromOrientGraphOnceForEach() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", Collections.emptyMap());
        GraphVertexType anotherVertexType = new GraphVertexType("anotherVertexTypeName", Collections.emptyMap());

        graphSchemaManager.registerVertexType(vertexType);
        graphSchemaManager.registerVertexType(anotherVertexType);

        verify(graphSchemaManager.orientGraph, times(1)).createVertexType(eq(vertexType.getVertexTypeName()));
        verify(graphSchemaManager.orientGraph, times(1)).createVertexType(eq(anotherVertexType.getVertexTypeName()));
    }

    @Test
    public void whenRegisteringVertexWithoutProperties_ShouldNotCallCreatePropertyFromOrientGraphVertexType() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();

        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", Collections.emptyMap());
        OrientVertexType mockedVertex = mockOrientVertexType(graphSchemaManager, vertexType.getVertexTypeName());

        graphSchemaManager.registerVertexType(vertexType);

        verify(mockedVertex, never()).createProperty(any(), any());
    }

    @Test
    public void whenRegisteringVertexWithSingleProperty_ShouldCallCreatePropertyFromOrientGraphVertexTypeOnce() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();

        Map<String, Class> properties = new HashMap<>();
        properties.put("test1", String.class);
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", properties);

        OrientVertexType mockedVertex = mockOrientVertexType(graphSchemaManager, vertexType.getVertexTypeName());
        graphSchemaManager.registerVertexType(vertexType);

        verify(mockedVertex, times(1)).createProperty("test1", OType.STRING);
    }

    @Test
    public void whenRegisteringVertexWithMoreThanOneProperty_ShouldCallCreatePropertyFromOrientGraphVertexTypeForEachProperty() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();

        Map<String, Class> properties = new HashMap<>();
        properties.put("test1", String.class);
        properties.put("test2", String.class);
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", properties);

        OrientVertexType mockedVertex = mockOrientVertexType(graphSchemaManager, vertexType.getVertexTypeName());

        graphSchemaManager.registerVertexType(vertexType);

        verify(mockedVertex, times(2)).createProperty(any(), any());
    }

    @Test
    public void whenRegisteringVertexWithMultipleProperties_ShouldCreateAllPropertiesWithProperNameAndType() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();

        Map<String, Class> properties = new HashMap<String, Class>() { {
            put("test1", String.class);
            put("40", Long.class);
            put("1", Integer.class);
        } };
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", properties);

        OrientVertexType mockedVertex = mockOrientVertexType(graphSchemaManager, vertexType.getVertexTypeName());

        graphSchemaManager.registerVertexType(vertexType);

        verify(mockedVertex).createProperty("test1", OType.STRING);
        verify(mockedVertex).createProperty("40", OType.LONG);
        verify(mockedVertex).createProperty("1", OType.INTEGER);
    }

    @Test
    public void whenRegisteringVertexType_ShouldCheckIfVertexTypeIsDefinedInOrientGraph() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", Collections.emptyMap());

        graphSchemaManager.registerVertexType(vertexType);
        verify(graphSchemaManager.orientGraph, times(1)).getVertexType(vertexType.getVertexTypeName());
    }

    @Test
    public void whenRegisteringVertexTypeWithoutProperties_ShouldNotCreateOrientVertexTypeIfTypeAlreadyExists() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", Collections.emptyMap());

        OrientVertexType orientVertexType = mockOrientVertexType(graphSchemaManager, vertexType.getVertexTypeName());
        when(graphSchemaManager.orientGraph.getVertexType(vertexType.getVertexTypeName())).thenReturn(orientVertexType);

        graphSchemaManager.registerVertexType(vertexType);

        verify(graphSchemaManager.orientGraph, never()).createVertexType(vertexType.getVertexTypeName());
    }

    @Test
    public void whenRegisteringExistingVertexTypeWithProperties_ShouldCreateAllMissingProperties() {
        OrientVertexType mockedVertexTypeLessProperties = mock(OrientVertexType.class);

        List<OProperty> lessOProperties = new ArrayList<>();
        lessOProperties.add(createOProperty("test1", String.class));
        lessOProperties.add(createOProperty("1", Integer.class));

        when(mockedVertexTypeLessProperties.declaredProperties()).thenReturn(lessOProperties);

        Map<String, Class> properties = new HashMap<String, Class>() { {
            put("test1", String.class);
            put("40", Long.class);
            put("1", Integer.class);
        } };
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", properties);

        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();

        when(graphSchemaManager.orientGraph.getVertexType(vertexType.getVertexTypeName())).thenReturn(mockedVertexTypeLessProperties);

        graphSchemaManager.registerVertexType(vertexType);

        verify(mockedVertexTypeLessProperties).createProperty("40", OType.LONG);
    }

    @Test
    public void whenRegisteringExistingVertexTypeWithProperties_ShouldDropAllObsoleteProperties() {
        List<OProperty> moreOProperties = new ArrayList<>();
        moreOProperties.add(createOProperty("test1", String.class));
        moreOProperties.add(createOProperty("40", Long.class));
        moreOProperties.add(createOProperty("1", Integer.class));

        OrientVertexType mockedVertexTypeMoreProperties = mock(OrientVertexType.class);
        when(mockedVertexTypeMoreProperties.declaredProperties()).thenReturn(moreOProperties);

        Map<String, Class> properties = new HashMap<String, Class>() { {
            put("test1", String.class);
            put("1", Integer.class);
        } };
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", properties);

        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();
        when(graphSchemaManager.orientGraph.getVertexType(vertexType.getVertexTypeName())).thenReturn(mockedVertexTypeMoreProperties);

        graphSchemaManager.registerVertexType(vertexType);

        verify(mockedVertexTypeMoreProperties).dropProperty("40");
    }

    @Test
    public void whenRegisteringExistingVertexTypeWithPropertyWithExistingNameAndDifferentType_ShouldDropAndCreateProperty() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();

        Map<String, Class> properties = new HashMap<String, Class>() { {
            put("test1", String.class);
        } };
        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", properties);

        OrientVertexType vertexTypeWithDifferentPropertyType = mock(OrientVertexType.class);

        List<OProperty> oPropertyWithDifferentPropertyType = new ArrayList<>();
        oPropertyWithDifferentPropertyType.add(createOProperty("test1", Integer.class));

        when(vertexTypeWithDifferentPropertyType.declaredProperties()).thenReturn(oPropertyWithDifferentPropertyType);

        when(graphSchemaManager.orientGraph.getVertexType(vertexType.getVertexTypeName())).thenReturn(vertexTypeWithDifferentPropertyType);

        graphSchemaManager.registerVertexType(vertexType);

        verify(vertexTypeWithDifferentPropertyType).createProperty("test1", OType.STRING);
        verify(vertexTypeWithDifferentPropertyType).dropProperty("test1");
    }

    @Test
    public void whenVertexTypeIsUpdated_ShouldTruncateVertexData() {
        GraphSchemaManagerImpl graphSchemaManager = createGraphSchemaManager();

        GraphVertexType vertexType = new GraphVertexType("vertexTypeName", Collections.emptyMap());

        graphSchemaManager.registerVertexType(vertexType);

        OrientVertexType mockedVertexType = mock(OrientVertexType.class);
        List<OProperty> oProperties = new ArrayList<>();
        oProperties.add(createOProperty("test1", String.class));

        when(mockedVertexType.declaredProperties()).thenReturn(oProperties);
        when(graphSchemaManager.orientGraph.getVertexType(vertexType.getVertexTypeName())).thenReturn(mockedVertexType);

        graphSchemaManager.registerVertexType(vertexType);
        verify(graphSchemaManager.orientGraphDataRemover).truncateOrientVertexType("vertexTypeName");
    }

    private GraphSchemaManagerImpl createGraphSchemaManager() {
        GraphSchemaManagerImpl graphSchemaManager = new GraphSchemaManagerImpl();
        graphSchemaManager.orientGraph = mock(OrientExtendedGraph.class);
        graphSchemaManager.orientGraphDataRemover = mock(OrientGraphDataRemover.class);

        return graphSchemaManager;
    }

    private OrientVertexType mockOrientVertexType(GraphSchemaManagerImpl graphSchemaManager, String vertexTypeName) {
        OrientVertexType mockedVertex = mock(OrientVertexType.class);
        when(graphSchemaManager.orientGraph.createVertexType(vertexTypeName))
                .then(a -> {
                when(graphSchemaManager.orientGraph.getVertexType(vertexTypeName)).thenReturn(mockedVertex);
                return mockedVertex;
            });
        return mockedVertex;
    }

    private OProperty createOProperty(String name, Class clazz) {
        OProperty oProperty = mock(OProperty.class);
        OType oType = OType.getTypeByClass(clazz);

        when(oProperty.getName()).thenReturn(name);
        when(oProperty.getType()).thenReturn(oType);

        return oProperty;
    }
}
