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

package com.intel.rsa.podm.repositories.orientdb.query;

import com.tinkerpop.blueprints.Direction;

/**
 * Utilities class for building graph queries.
 */
public final class GraphQueryHelper {
    private GraphQueryHelper() {
    }

    /**
     * Converts link to fields required to traverse those links.
     * @param linkLabel
     * @param linkDirection
     * @return
     */
    public static String[] convertToTraverseFields(String linkLabel, Direction linkDirection) {
        String[] traverseFields = new String[2];

        if (linkDirection == Direction.IN) {
            traverseFields[0] = "out";
            traverseFields[1] = "in_" + linkLabel;
        } else {
            traverseFields[0] = "in";
            traverseFields[1] = "out_" + linkLabel;
        }
        return traverseFields;
    }

    /**
     * Converts link to fields required to traverse those links, both direction.
     * @param linkLabel
     * @return
     */
    public static String[] convertToTraverseBothDirectionFields(String linkLabel) {
        return new String[] {
                "in",
                "in_" + linkLabel,
                "out",
                "out_" + linkLabel
        };
    }
}
