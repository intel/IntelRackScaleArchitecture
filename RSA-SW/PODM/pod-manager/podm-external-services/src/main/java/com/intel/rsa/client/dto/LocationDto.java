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

package com.intel.rsa.client.dto;

import com.fasterxml.jackson.annotation.JsonAnySetter;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

import java.util.LinkedHashMap;

@JsonIgnoreProperties(ignoreUnknown = true)
public class LocationDto {
    private final LinkedHashMap<String, Integer> coords = new LinkedHashMap<>();

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

    @JsonAnySetter
    public void set(String name, Integer value) {
        coords.put(name, value);
    }

    public LinkedHashMap<String, Integer> toMap() {
        return coords;
    }
}
