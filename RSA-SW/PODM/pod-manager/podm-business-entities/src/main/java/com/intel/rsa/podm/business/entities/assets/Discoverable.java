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

package com.intel.rsa.podm.business.entities.assets;

import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;
import com.intel.rsa.podm.business.entities.base.Link;
import com.tinkerpop.blueprints.Direction;

import java.net.URI;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.uriProperty;

public interface Discoverable {
    Link OWNED_BY = new OwnedBy();

    DomainObjectProperty<URI> SOURCE_URI = uriProperty("sourceUri");

    URI getSourceUri();
    void setSourceUri(URI sourceUri);

    class OwnedBy implements Link {
        @Override
        public String getLabel() {
            return "owns";
        }

        @Override
        public Direction getDirection() {
            return Direction.IN;
        }
    }
}
