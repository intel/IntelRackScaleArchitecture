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

package com.intel.rsa.podm.rest.representation.json.providers;

import com.fasterxml.jackson.databind.JsonSerializer;
import com.fasterxml.jackson.databind.module.SimpleModule;
import com.intel.rsa.common.types.ConvertableEnum;
import com.intel.rsa.common.types.Id;
import com.intel.rsa.common.types.locations.Location;
import com.intel.rsa.podm.rest.representation.json.deserializers.ConvertableEnumDeserializer;
import com.intel.rsa.podm.rest.representation.json.deserializers.LocationDeserializer;
import com.intel.rsa.podm.rest.representation.json.serializers.ContextAwareSerializer;
import com.intel.rsa.podm.rest.representation.json.serializers.ConvertableEnumSerializer;
import com.intel.rsa.podm.rest.representation.json.serializers.IdSerializer;
import com.intel.rsa.podm.rest.representation.json.serializers.OffsetDateTimeSerializer;
import org.reflections.Reflections;

import javax.enterprise.inject.Instance;
import java.time.OffsetDateTime;
import java.util.Set;

public class RsaObjectMapperModuleProvider {

    private static final String TYPES_PACKAGE_NAME = "com.intel.rsa.common.types";

    public SimpleModule getSerializerModule(JsonProvider.SerializerContext context, Instance<ContextAwareSerializer> serializers) {
        SimpleModule module = new SimpleModule();

        for (ContextAwareSerializer serializer : serializers) {
            serializer.setContext(context);
            module.addSerializer((JsonSerializer<?>) serializer);
        }

        //add serializers for all ConvertableEnums automatically
        for (Class subType : getRsaEnumTypes()) {
            module.addSerializer(subType, new ConvertableEnumSerializer<>());
        }

        module.addSerializer(OffsetDateTime.class, OffsetDateTimeSerializer.INSTANCE);
        module.addSerializer(Id.class, IdSerializer.INSTANCE);

        return module;
    }

    public SimpleModule getDeserializerModule() {
        SimpleModule module = new SimpleModule();

        for (Class subType : getRsaEnumTypes()) {
            module.addDeserializer(subType, new ConvertableEnumDeserializer<>(subType));
        }
        module.addDeserializer(Location.class, new LocationDeserializer());

        return module;
    }

    private static Set<Class<? extends ConvertableEnum>> getRsaEnumTypes() {
        Reflections reflections = new Reflections(TYPES_PACKAGE_NAME);
        return reflections.getSubTypesOf(ConvertableEnum.class);
    }
}
