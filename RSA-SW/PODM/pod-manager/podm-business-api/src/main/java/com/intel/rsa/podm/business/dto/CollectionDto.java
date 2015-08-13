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

package com.intel.rsa.podm.business.dto;

import com.intel.rsa.common.types.Id;

import java.time.OffsetDateTime;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

public class CollectionDto {

    private final CollectionDtoType type;
    private final OffsetDateTime modified;
    private final List<Id> members;

    public CollectionDto(CollectionDtoType type, OffsetDateTime modified, Collection<Id> members) {
        this.type = type;
        this.modified = modified;
        this.members = Collections.unmodifiableList(new ArrayList<>(members));
    }

    public CollectionDtoType getType() {
        return  type;
    }

    public OffsetDateTime getModified() {
        return modified;
    }

    public List<Id> getMembers() {
        return members;
    }
}
