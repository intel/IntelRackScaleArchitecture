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
 * @file builder_util.hpp
 *
 * @brief Declaration of to_resource_status function
 * */

#ifndef PSME_REST_NODE_BUILDER_UTIL_HPP
#define PSME_REST_NODE_BUILDER_UTIL_HPP

#include "psme/rest/resource/status.hpp"
#include "../src/core/dto/status_dto.hpp"

namespace psme {
namespace rest {
namespace node {

/*!
 * @brief Converts JSONRPC status to REST status.
 *
 * @param[in] status JSONRPC status
 * */
psme::rest::resource::Status
to_resource_status(const psme::core::dto::StatusDTO::Response& status);

}
}
}

#endif // PSME_REST_NODE_BUILDER_UTIL_HPP
