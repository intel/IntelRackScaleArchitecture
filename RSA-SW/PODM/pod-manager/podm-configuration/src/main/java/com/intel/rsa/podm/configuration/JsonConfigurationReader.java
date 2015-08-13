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

package com.intel.rsa.podm.configuration;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.Module;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;
import com.intel.rsa.common.types.ConvertableEnum;
import com.intel.rsa.common.types.helpers.ConvertableEnumHelper;
import org.reflections.Reflections;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Set;

/**
 * Reads preconfigured content for the given type from config json file
 */
public class JsonConfigurationReader implements ConfigurationReader {

    private static final String TYPES_PACKAGE_NAME = "com.intel.rsa.common.types";

    @Override
    public <T> T readConfiguration(String configurationName, Class<T> type) throws ConfigurationIOException {
        String path = ConfigurationPaths.getJsonConfigurationPath(configurationName);
        try (InputStream is = new FileInputStream(path)) {
            ObjectMapper mapper = new ObjectMapper();
            mapper.registerModule(getConvertableEnumModule());

            return mapper.readValue(is, type);
        } catch (IOException e) {
            throw new ConfigurationIOException("Could not read configuration : " + path + ". ", e);
        }
    }

    private Module getConvertableEnumModule() {
        SimpleModule enumDeserializersModule = new SimpleModule();

        //add deserializers for all ConvertableEnums automatically
        Reflections reflections = new Reflections(TYPES_PACKAGE_NAME);
        Set<Class<? extends ConvertableEnum>> subTypes = reflections.getSubTypesOf(ConvertableEnum.class);
        for (Class subType : subTypes) {
            enumDeserializersModule.addDeserializer(subType, new ConvertableEnumDeserializer<>(subType));
        }

        return enumDeserializersModule;
    }

    // TODO find the best way to move it to web-client cross-cutting component (merge with external services implementation)
    static class ConvertableEnumDeserializer<T extends ConvertableEnum> extends JsonDeserializer<T> {
        private final Class<T> enumType;

        public ConvertableEnumDeserializer(Class<T> enumType) {
            this.enumType = enumType;
        }

        @Override
        public T deserialize(JsonParser parser, DeserializationContext context) throws IOException, JsonProcessingException {
            String text = parser.getValueAsString();
            //Since is optional value it can be null
            if (text == null) {
                return null;
            }
            return ConvertableEnumHelper.fromString(enumType, text);
        }
    }
}
