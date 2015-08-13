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
 * */

#include "psme/rest/node/crud/utils.hpp"
#include "psme/rest/http/server.hpp"
#include "psme/rest/error/error_factory.hpp"
#include "psme/rest/error/server_exception.hpp"
#include "json/value.hpp"
#include "configuration/schema_validator.hpp"
#include "configuration/schema_errors.hpp"
#include "configuration/schema_reader.hpp"
#include "logger_ext.hpp"

using namespace psme::rest::error;
using configuration::SchemaErrors;
using configuration::SchemaValidator;
using configuration::SchemaReader;

namespace psme {
namespace rest {
namespace node {

json::Value
validate_request_body(const psme::rest::http::Request& request, const json::Value& schema) {
    json::Deserializer deserializer(request.get_body());

    if (deserializer.is_invalid()) {
        log_error(GET_LOGGER("rest"), " Malformed JSON: " << request.get_body());
        ServerError error = ErrorFactory::create_invalid_payload_error();
        throw ServerException(error);
    }

    json::Value json;
    deserializer >> json;

    SchemaErrors errors;
    SchemaValidator validator;
    SchemaReader reader;
    reader.load_schema(schema, validator);
    validator.validate(json, errors);

    if (errors.count()) {
        log_error(GET_LOGGER("rest"), " Bad request: " << errors.to_string());
        ServerError error = ErrorFactory::create_invalid_payload_error();
        throw ServerException(error);
    }

    return json;
}

}
}
}
