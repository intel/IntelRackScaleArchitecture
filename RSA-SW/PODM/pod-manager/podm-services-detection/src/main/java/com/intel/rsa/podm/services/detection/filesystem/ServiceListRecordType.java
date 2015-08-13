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

package com.intel.rsa.podm.services.detection.filesystem;

import com.intel.rsa.podm.services.detection.api.ServiceType;

public enum ServiceListRecordType {

    PSME(ServiceType.PSME_SERVICE, 8888),
    RSS(ServiceType.STORAGE_SERVICE, 7778),
    UNKNOWN_RECORD(ServiceType.STORAGE_SERVICE, -1);

    private final ServiceType apiType;
    private final int defaultPort;

    ServiceListRecordType(ServiceType apiType, int defaultPort) {
        this.apiType = apiType;
        this.defaultPort = defaultPort;
    }

    /**
     * Creates enum instance from input string, assumes lowercase input.
     * UNKNOWN_RECORD is returned as default value for unrecognized inputs.
     *
     * @param typeId typeId string
     * @return Enum value,
     */
    public static ServiceListRecordType fromString(String typeId) {
        switch (typeId) {
            case "psme": return PSME;
            case "rss": return RSS;
            default: return UNKNOWN_RECORD;
        }
    }

    public ServiceType getApiType() {
        return apiType;
    }

    public int getDefaultPort() {
        return defaultPort;
    }
}
