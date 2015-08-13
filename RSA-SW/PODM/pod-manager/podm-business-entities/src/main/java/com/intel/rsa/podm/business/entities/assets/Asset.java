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
import com.intel.rsa.podm.business.entities.base.DomainObject;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;

import java.net.URI;
import java.time.OffsetDateTime;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.locationProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.offsetDateTimeProperty;
import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.stringProperty;

@AbstractDomainObject
public class Asset extends DomainObject implements Discoverable {
    public static final DomainObjectProperty<String> NAME = stringProperty("name");
    public static final DomainObjectProperty<OffsetDateTime> MODIFIED = offsetDateTimeProperty("modified");
    public static final DomainObjectProperty<Location> LOCATION = locationProperty("location");

    public String getName() {
        return getProperty(NAME);
    }

    public void setName(String name) {
        setProperty(NAME, name);
    }

    public OffsetDateTime getModified() {
        return getProperty(MODIFIED);
    }

    public void updateModified() {
        setProperty(MODIFIED, OffsetDateTime.now());
    }

    public Location getLocation() {
        return getProperty(LOCATION);
    }

    public void setLocation(Location location) {
        setProperty(LOCATION, location);
    }

    public void contain(Asset asset) {
        link(AssetLink.CONTAINS, asset);
    }

    public boolean contains(Asset asset) {
        return DomainObjectHelper.contains(asset, getLinked(AssetLink.CONTAINS));
    }

    @Override
    public URI getSourceUri() {
        return getProperty(SOURCE_URI);
    }

    @Override
    public void setSourceUri(URI sourceUri) {
        setProperty(SOURCE_URI, sourceUri);
    }
}
