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

package com.intel.rsa.podm.rest.odataid;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.intel.rsa.common.types.Id;

import java.net.URI;

import static com.google.common.base.Preconditions.checkNotNull;

public final class ODataId {

    @JsonProperty("@odata.id")
    private final URI value;

    private ODataId(URI value) {
        this.value = checkNotNull(value);
    }

    public static ODataId oDataId(URI uri) {
        return new ODataId(uri);
    }

    public static ODataId oDataId(ODataId oDataId, Id id) {
        String uri = oDataId.toString();
        return oDataId(uri.endsWith("/") ? (uri + id) : (uri + "/" + id));
    }

    static ODataId oDataId(String uri) {
        return oDataId(URI.create(uri));
    }

    @Override
    public String toString() {
        return this.value.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        ODataId oDataId = (ODataId) o;

        if (!value.equals(oDataId.value)) {
            return false;
        }

        return true;
    }

    @Override
    public int hashCode() {
        return value.hashCode();
    }
}
