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

#include "network_hop_dto.hpp"

using namespace psme::core::dto::network;

NetworkHopDTO::~NetworkHopDTO(){}

NetworkHopDTO::Response::~Response(){}

void NetworkHopDTO::Response::to_object(const Json::Value& response) {
    if (response.isObject()) {
        set_metric(response["metric"].asInt());
        set_port_identifier(response["portIdentifier"].asString());
        set_ipv4_address(response["ipv4Address"].asString());
        set_ipv6_address(response["ipv6Address"].asString());
    }
}
