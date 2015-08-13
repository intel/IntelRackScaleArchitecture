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

package com.intel.rsa.client.webclient;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;
import com.fasterxml.jackson.datatype.jsr310.JSR310Module;
import com.intel.rsa.client.deserializers.ConvertableEnumDeserializer;
import com.intel.rsa.client.deserializers.ConvertableEnumSerializer;
import com.intel.rsa.common.types.ConvertableEnum;
import org.jboss.resteasy.client.jaxrs.ResteasyClient;
import org.jboss.resteasy.client.jaxrs.ResteasyClientBuilder;
import org.jboss.resteasy.plugins.providers.jackson.ResteasyJackson2Provider;
import org.reflections.Reflections;

import java.util.Set;


/**
 * Provides configured Resteasy JAX-RS clients
 */
public final class ResteasyClientProvider {
    private static final String TYPES_PACKAGE_NAME = "com.intel.rsa.common.types";

    private ResteasyClientProvider() {
    }

    /**
     * Provides ResteasyClient with configured Jackson
     *
     * @return configured client with jackson provider
     */
    public static ResteasyClient getClientWithJacksonProvider() {
        ResteasyJackson2Provider resteasyJacksonProvider = new ResteasyJackson2Provider();

        ObjectMapper mapper = new ObjectMapper()
                .registerModule(new JSR310Module())
                .registerModule(createEnumModule())
                .enable(DeserializationFeature.FAIL_ON_NULL_FOR_PRIMITIVES);


        resteasyJacksonProvider.setMapper(mapper);

        return new ResteasyClientBuilder()
                .register(resteasyJacksonProvider)
                .build();
    }

    private static SimpleModule createEnumModule() {
        SimpleModule enumDeserializersModule = new SimpleModule();

        //add deserializers for all ConvertableEnums automatically
        Reflections reflections = new Reflections(TYPES_PACKAGE_NAME);
        Set<Class<? extends ConvertableEnum>> subTypes = reflections.getSubTypesOf(ConvertableEnum.class);
        for (Class subType : subTypes) {
            enumDeserializersModule.addSerializer(subType, new ConvertableEnumSerializer<>(subType));
            enumDeserializersModule.addDeserializer(subType, new ConvertableEnumDeserializer<>(subType));
        }

        return enumDeserializersModule;
    }
}
