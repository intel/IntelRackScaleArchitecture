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

package com.intel.rsa.podm.business.entities.base;

import com.intel.rsa.common.types.ConvertableEnum;
import com.intel.rsa.common.types.helpers.ConvertableEnumListHolder;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.business.entities.base.converters.ConvertableEnumListConverter;
import com.intel.rsa.podm.business.entities.base.converters.DateConverter;
import com.intel.rsa.podm.business.entities.base.converters.EnumConverter;
import com.intel.rsa.podm.business.entities.base.converters.LocationConverter;
import com.intel.rsa.podm.business.entities.base.converters.UriConverter;
import com.intel.rsa.podm.business.entities.base.converters.UuidConverter;

import java.math.BigDecimal;
import java.net.URI;
import java.time.OffsetDateTime;
import java.util.Date;
import java.util.List;
import java.util.UUID;

public final class DomainObjectProperties {
    private DomainObjectProperties() {
    }

    public static DomainObjectProperty<String> stringProperty(String name) {
        return DomainObjectProperty.property(name, String.class);
    }

    public static DomainObjectProperty<Integer> integerProperty(String name) {
        return DomainObjectProperty.property(name, Integer.class);
    }

    public static DomainObjectProperty<Boolean> booleanProperty(String name) {
        return DomainObjectProperty.property(name, Boolean.class);
    }

    public static DomainObjectProperty<Long> longProperty(String name) {
        return DomainObjectProperty.property(name, Long.class);
    }

    public static DomainObjectProperty<BigDecimal> decimalProperty(String name) {
        return DomainObjectProperty.property(name, BigDecimal.class);
    }

    public static <T> DomainObjectProperty<List<T>> listProperty(String name) {
        return DomainObjectProperty.property(name, (Class) List.class);
    }

    public static DomainObjectProperty<OffsetDateTime> offsetDateTimeProperty(String name) {
        return DomainObjectProperty.property(name, Date.class, new DateConverter());
    }

    public static <T extends Enum> DomainObjectProperty<T> enumProperty(String name, Class<T> aClass) {
        return DomainObjectProperty.property(name, String.class, new EnumConverter<>(aClass));
    }

    public static DomainObjectProperty<Location> locationProperty(String name) {
        return DomainObjectProperty.property(name, String.class, new LocationConverter());
    }

    public static <T extends ConvertableEnum> DomainObjectProperty<ConvertableEnumListHolder<T>> convertableEnumListProperty(String name, Class<T> enumType) {
        return DomainObjectProperty.property(name, String.class, new ConvertableEnumListConverter<>(enumType));
    }

    public static DomainObjectProperty<UUID> uuidProperty(String name) {
        return DomainObjectProperty.property(name, String.class, new UuidConverter());
    }

    public static DomainObjectProperty<URI> uriProperty(String name) {
        return DomainObjectProperty.property(name, String.class, new UriConverter());
    }
}
