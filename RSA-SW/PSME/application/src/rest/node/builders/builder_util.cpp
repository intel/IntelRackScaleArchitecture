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

#include "psme/rest/node/builders/builder_util.hpp"

namespace psme {
namespace rest {
namespace node {

using namespace psme::rest::utils;

using psme::rest::resource::Status;
using psme::rest::resource::StateType;
using psme::rest::resource::HealthType;
using psme::core::dto::StatusDTO;

Status
to_resource_status(const StatusDTO::Response& status) {
    Status st{};
    st.set_state(from_string(status.get_state().c_str(), StateType::ABSENT));
    st.set_health(from_string(status.get_health().c_str(), HealthType::WARNING));
    return st;
}

}
}
}
