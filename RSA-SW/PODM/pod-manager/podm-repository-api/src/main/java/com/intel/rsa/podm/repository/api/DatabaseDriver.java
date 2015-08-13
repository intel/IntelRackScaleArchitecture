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

package com.intel.rsa.podm.repository.api;

import com.tinkerpop.blueprints.Vertex;

import java.util.List;


/**
 * Abstraction for graph database driver used by Pod data layer.
 */
public interface DatabaseDriver {
    /**
     * Creates vertex of specified class.
     * @param className
     * @return created vertex
     */
    Vertex addVertex(String className);


    /**
     * Finds vertex of specified class by its business id.
     * @param className
     * @param id business id
     * @return
     */
    Vertex getVertex(String className, Long id);

    /**
     * Resolves vertex class.
     * @param vertex
     * @return
     */
    String getVertexClass(Vertex vertex);

    /**
     * Converts vertex technical id to business id.
     * @param recordId
     * @return
     */
    Long getId(Object recordId);

    /**
     * Finds all vertices of specified class.
     * @param className
     * @return
     */
    List<Vertex> getVerticesOfClass(String className);

    /**
     * Searches for vertices having some provided property value, traversing, both direction, by provided link label only.
     * @param startVertex from where to begin traverse
     * @param subGraphLinkLabel link to traverse by
     * @param propertyName searched property name
     * @param propertyValue searched property value
     * @return matching vertices
     */
    List<Vertex> findInSubGraphByProperty(Vertex startVertex, String subGraphLinkLabel, String propertyName,
                                                Object propertyValue);

    /**
     * Finds vertices of specified class by specified property.
     *
     * @param className vertex class name
     * @param propertyName name of property to search by
     * @param propertyValue value of property to search by
     * @return matching vertices
     */
    List<Vertex> findByProperty(String className, String propertyName, Object propertyValue);

    /**
     * Finds vertices of specified class by specified flag - passing false value will return both
     * Vertices without this flag set and Vertices with flag set to false
     *
     * @param className
     * @param propertyName
     * @param value
     * @return
     */
    List<Vertex> findByFlag(String className, String propertyName, boolean value);

    /**
     * Deletes specified vertex
     *
     * @param vertex
     */
    void deleteVertex(Vertex vertex);
}
