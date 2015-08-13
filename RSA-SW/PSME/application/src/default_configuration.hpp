/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 * @file default_configuration.hpp
 *
 * @brief Application default configuration
 * */

#ifndef APPLICATION_DEFAULT_CONFIGURATION_HPP
#define APPLICATION_DEFAULT_CONFIGURATION_HPP

namespace psme {
namespace app {

/*!
    Logger configuration can have defined streams example
    App main logger in PSME Application
    "logger" : {
        "app" : {},
        "rest" : {
            "level" : "DEBUG",
            streams : [
                {
                    "type" : "FILE",
                    "file" : "path_to_file"
                },
                {
                "type" : "STDOUT"
                }
            ]
        }
    }
}
*/
/*! Default configuration for the application */
static constexpr const char DEFAULT_CONFIGURATION[] = R"({
"server": { "url": "http://localhost:8888", "network-interface-name" : "enp0s20f0.4094"},
"registration": {"port": 8383, "minDelay": 3},
"commands": { "generic": "Registration" },
"logger" : { "app" : {} },
"eventing" : {"enabled": false, "address" : "localhost", "port" : 5667, "poll-interval-sec" : 10},
"rest-server" : {"storage-service-mode" : false},
"service-uuid-file" : "service_uuid.json"
})";

/*!
 * @brief Configuration validators
 * Supported validators:
 * min - minimal accepted value
 * max - maximum accepted value
 * type - check value type eg. "int", "string"
 * address - check value as IPv4 or IPv6 address
 * url - check value as URL
 * anyof - check if value is one of array value eg. "anyof" : [ 1, 2, 3 ]
 * anyof validator required "type" property
 *
 * json validator property should contain property:
 * "validator": true
 *
 * Eg. Module port validator
 * "module": {
 *      "port": {
 *          "validator": true,
 *          "min": 0,
 *          "max": 65535
 *      }
 * }
 */
static constexpr char DEFAULT_VALIDATOR_JSON[] = R"({
"logger" : {
    "rest" : {
        "level" : {
            "validator" : true,
            "type" : "string",
            "anyof": ["DEBUG", "ERROR", "INFO"]
        }
    }
},
"eventing" : {
    "enabled" : {
        "validator" : true,
        "type" : "bool"
    },
    "poll-interval-sec" : {
        "validator" : true,
        "type" : "uint",
        "min" : 5
    }
},
"rest-server" : {
    "storage-service-mode" : {
        "validator" : true,
        "type" : "bool"
    }
},
"server" : {
    "network-interface-name" : {
        "validator" : true,
        "type" : "string"
    }
},
"service-uuid-file" : {
    "validator" : true,
    "type" : "string"
},
"port" : {
    "validator": true,
    "max": 65535,
    "min": 0
},
"level": {
    "validator": true,
    "type": "string",
    "anyof": ["DEBUG", "ERROR", "INFO"]
},
"url": {
    "validator": true,
    "type": "string",
    "url": ""
},
"address": {
    "validator": true,
    "address": ""
}
})";

/* Default environment variable for configuration file */
static constexpr const char DEFAULT_ENV_FILE[] = "CORE_CONFIGURATION_FILE";

/* Default configuration file to load when env isn't present/valid */
static constexpr const char DEFAULT_FILE[] = "configuration.json";

}
}

#endif /* APPLICATION_DEFAULT_CONFIGURATION_HPP */
