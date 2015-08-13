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

package com.intel.rsa.podm.configuration;

import java.util.HashMap;
import java.util.Map;

public final class ConfigurationPaths {
    private static final String DEFAULT_PATH_TO_CONFIGURATION_FILES = "/tmp/rsa/config";
    private static final Map<String, String> CONFIGURATION_PATHS = new HashMap<>();

    private ConfigurationPaths() {
    }

    static {
        CONFIGURATION_PATHS.put(PersistentConfigurations.SERVICE_ROOT_UUID.getName(), "/var/lib/rsa");
    }

    public static String getJsonConfigurationPath(String configurationName) {
        return getConfigurationPath(configurationName) + "/" + configurationName + ".json";
    }

    private static String getConfigurationPath(String configurationName) {
        return CONFIGURATION_PATHS.containsKey(configurationName)
                ? CONFIGURATION_PATHS.get(configurationName)
                : DEFAULT_PATH_TO_CONFIGURATION_FILES;
    }

}
