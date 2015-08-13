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
 * */

#include "agent-framework/registration/registration_client.hpp"
#include "agent-framework/registration/registration_request.hpp"

using namespace agent_framework::generic;

Json::Value RegistrationClient::attach() noexcept(false) {
    RegistrationRequest request;

    Json::Value result = this->CallMethod("attach", request.to_json());
    if (result.isObject()) {
        log_debug(GET_LOGGER("registration"),
                "Registartion data received: " << result);
        return result;
    }
    else {
        throw jsonrpc::JsonRpcException(
                jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE,
                result.toStyledString());
    }
}

Json::Value RegistrationClient::heart_beat() noexcept(false) {
    RegistrationRequest request;

    Json::Value result = CallMethod("heartBeat", {});
    if (result.isObject() && result.isMember("timestamp")
            && result.isMember("minDelay")) {
        log_debug(GET_LOGGER("registration"),
                "Heart beat received: " << result);
        return result;
    }
    else {
        throw jsonrpc::JsonRpcException(
                jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE,
                result.toStyledString());
    }
}


RegistrationClient::~RegistrationClient() {}
