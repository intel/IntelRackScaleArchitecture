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

import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.entities.assets.links.AssetLink;
import com.intel.rsa.podm.business.entities.base.AbstractDomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;
import com.intel.rsa.podm.business.entities.base.SingletonDomainObject;

import java.time.OffsetDateTime;
import java.util.Collection;
import java.util.Objects;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.offsetDateTimeProperty;

/**
 * Base class for global/singleton collections containing domain objects from Asset hierarchy.
 */
@AbstractDomainObject
public abstract class AssetCollection<T extends Asset> extends SingletonDomainObject {
    public static final DomainObjectProperty<OffsetDateTime> MODIFIED = offsetDateTimeProperty("modified");

    public void setModified(OffsetDateTime modified) {
        setProperty(MODIFIED, modified);
    }

    public OffsetDateTime getModified() {
        OffsetDateTime modified = getProperty(MODIFIED);

        if (modified == null) {
            modified = OffsetDateTime.now();
            setModified(modified);
        }

        return modified;
    }

    public Collection<T> getMembers() {
        return getLinked(AssetLink.MEMBER, getMembersClass());
    }

    public void link(T member) {
        updateModified();
        link(AssetLink.MEMBER, member);
    }

    protected abstract Class<T> getMembersClass();

    public T getByLocation(Location location) {
        for (T member : getMembers()) {
            if (Objects.equals(member.getLocation(), location)) {
                return member;
            }
        }
        return null;
    }

    private void updateModified() {
        setModified(OffsetDateTime.now());
    }
}
