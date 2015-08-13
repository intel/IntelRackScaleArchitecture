/*!
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
 *
 * @file utils.hpp
 *
 * @brief Declaration of validate_request_body function
 * */

#ifndef PSME_REST_NODE_CRUD_UTILS_HPP
#define PSME_REST_NODE_CRUD_UTILS_HPP

#include "json/json.hpp"

namespace psme {
namespace rest {

/*! Forward declarations */
namespace http {
    class Request;
    class Response;
}

namespace node {

/*!
 * @brief Validates request body (JSON) against schema.
 *
 * @param request Request to be validated.
 * @param schema JSON schema against which validation is done.
 *
 * @return Valid JSON object
 * @throw ServerException if JSON is not valid.
 * */
json::Value
validate_request_body(const psme::rest::http::Request& request,
                      const json::Value& schema);

}
}
}

#endif // PSME_REST_NODE_CRUD_UTILS_HPP
