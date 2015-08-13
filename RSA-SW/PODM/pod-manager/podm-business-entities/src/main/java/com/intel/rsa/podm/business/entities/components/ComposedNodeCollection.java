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

package com.intel.rsa.podm.business.entities.components;


import com.intel.rsa.podm.business.entities.assets.links.CollectionLink;
import com.intel.rsa.podm.business.entities.base.DomainObjectProperty;
import com.intel.rsa.podm.business.entities.base.SingletonDomainObject;

import javax.enterprise.context.Dependent;
import javax.transaction.Transactional;
import java.time.OffsetDateTime;
import java.util.Collection;

import static com.intel.rsa.podm.business.entities.base.DomainObjectProperties.offsetDateTimeProperty;

@Dependent
@Transactional(Transactional.TxType.REQUIRED)
public class ComposedNodeCollection extends SingletonDomainObject {

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

    public Collection<ComposedNode> getMembers() {
        return getLinked(CollectionLink.COLLECTS, ComposedNode.class);
    }

    public void link(ComposedNode node) {
        updateModified();
        link(CollectionLink.COLLECTS, node);
    }

    private void updateModified() {
        setModified(OffsetDateTime.now());
    }
}
