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

package com.intel.rsa.common.types.locations;

import com.google.common.base.Splitter;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import static com.google.common.collect.Maps.filterEntries;
import static com.google.common.collect.Maps.transformValues;
import static java.lang.String.format;
import static java.util.Collections.unmodifiableMap;
import static java.util.stream.Collectors.toList;

/**
 * Represents topological location of Asset in Data Center.
 */
public final class Location {
    public static final String COORD_SEPARATOR = ",";
    public static final String COORD_VALUE_SEPARATOR = "=";
    private final Map<String, Integer> coords;

    public Location(Map<String, Integer> coords) {
        if (coords == null) {
            throw new IllegalArgumentException("coords must not be null");
        }

        this.coords = unmodifiableMap(new LinkedHashMap<>(coords));
    }

    public Location(String text) {
        if (text == null) {
            throw new IllegalArgumentException("text must not be null");
        }

        Map<String, String> coords = Splitter
                .on(COORD_SEPARATOR)
                .withKeyValueSeparator(COORD_VALUE_SEPARATOR)
                .split(text);
        this.coords = unmodifiableMap(transformValues(coords, Integer::valueOf));
    }

    /**
     * @deprecated it will be removed
     */
    @Deprecated
    Location(Integer pod, Integer rack, Integer drawer, Integer module, Integer blade) {
        LinkedHashMap<String, Integer> coords = new LinkedHashMap<>();

        coords.put("Pod", pod);
        coords.put("Rack", rack);
        coords.put("Drawer", drawer);
        coords.put("Module", module);
        coords.put("Blade", blade);

        this.coords = unmodifiableMap(new LinkedHashMap<>(filterEntries(coords, e -> e.getValue() != null)));
    }

    public Map<String, Integer> toMap() {
        return coords;
    }

    /**
     * @deprecated it will be removed
     */
    @Deprecated
    public Integer getPod() {
        return coords.get("Pod");
    }

    /**
     * @deprecated it will be removed
     */
    @Deprecated
    public Integer getRack() {
        return coords.get("Rack");
    }

    /**
     * @deprecated it will be removed
     */
    @Deprecated
    public Integer getDrawer() {
        return coords.get("Drawer");
    }

    /**
     * @deprecated it will be removed
     */
    @Deprecated
    public Integer getModule() {
        return coords.get("Module");
    }

    /**
     * @deprecated it will be removed
     */
    @Deprecated
    public Integer getBlade() {
        return coords.get("Blade");
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (!(o instanceof Location)) {
            return false;
        }

        Location otherLocation = (Location) o;

        return Objects.equals(coords, otherLocation.coords);
    }

    @Override
    public int hashCode() {
        return coords.hashCode();
    }

    @Override
    public String toString() {
        if (coords.isEmpty()) {
            return "";
        }

        List<String> keyValues = coords
                .entrySet()
                .stream()
                .map(e -> format("%s=%s", e.getKey(), e.getValue()))
                .collect(toList());

        return String.join(",", keyValues);
    }
}
