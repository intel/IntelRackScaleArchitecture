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

package com.intel.rsa.podm.rest.representation.json.serializers;

import java.net.URI;

final class ODataForCollection {
    private final String name;
    private final String oDataContext;
    private final String oDataType;

    private ODataForCollection(Builder builder) {
        name = builder.name;
        oDataContext = builder.oDataContext;
        oDataType = builder.oDataType;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public String getName() {
        return name;
    }

    public String getODataType() {
        return oDataType;
    }

    public URI getODataContext() {
        return URI.create(oDataContext);
    }

    public static final class Builder {
        private String name;
        private String oDataContext;
        private String oDataType;

        private Builder() {
        }

        public Builder name(String name) {
            this.name = name;
            return this;
        }

        public Builder oDataContext(String oDataContext) {
            this.oDataContext = oDataContext;
            return this;
        }

        public Builder oDataType(String oDataType) {
            this.oDataType = oDataType;
            return this;
        }

        public ODataForCollection build() {
            return new ODataForCollection(this);
        }
    }
}
