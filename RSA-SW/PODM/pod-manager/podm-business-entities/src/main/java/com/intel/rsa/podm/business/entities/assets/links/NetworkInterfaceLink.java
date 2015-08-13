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

package com.intel.rsa.podm.business.entities.assets.links;

import com.intel.rsa.podm.business.entities.base.Link;
import com.tinkerpop.blueprints.Direction;

public enum NetworkInterfaceLink implements Link {
    IP4_ADDRESS("ipV4Address", Direction.OUT),
    IP6_ADDRESS("ipV6Address", Direction.OUT),
    IP6_STATIC_ADDRESS("ipV6StaticAddress", Direction.OUT),
    IP6_ADDRESS_POLICY("ipV6AddressPolicy", Direction.OUT),
    NAME_SERVER("nameServerLink", Direction.OUT);

    private final String label;
    private final Direction direction;

    NetworkInterfaceLink(String label, Direction direction) {
        this.label = label;
        this.direction = direction;
    }

    @Override
    public String getLabel() {
        return label;
    }

    @Override
    public Direction getDirection() {
        return direction;
    }
}
