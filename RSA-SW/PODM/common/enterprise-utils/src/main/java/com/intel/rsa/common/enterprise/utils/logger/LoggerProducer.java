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

package com.intel.rsa.common.enterprise.utils.logger;

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.common.logger.LoggerFactory;

import javax.enterprise.inject.Produces;
import javax.enterprise.inject.spi.InjectionPoint;

public class LoggerProducer {

    @Produces
    Logger getLogger(InjectionPoint injectionPoint) {
        Class loggedClass = injectionPoint.getMember().getDeclaringClass();
        Logger logger = LoggerFactory.getLogger(loggedClass);

        return logger;
    }

    @Produces @NamedLogger(value = "")
    Logger getLoggerWithName(InjectionPoint injectionPoint) {
        NamedLogger namedLogger = injectionPoint.getAnnotated().getAnnotation(NamedLogger.class);
        String loggerName = namedLogger.value();

        Logger logger = LoggerFactory.getLogger(loggerName);

        return logger;
    }
}
