/*!
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file command/network/get_neighbor_switches_id.cpp
 * @brief Generic network command get neighbor switches id information implementation
 * */

#include "agent-framework/command/network/get_neighbor_switches_id.hpp"

using namespace agent_framework::command::network;

constexpr const char GetNeighborSwitchesId::AGENT[];

constexpr const char GetNeighborSwitchesId::TAG[];

GetNeighborSwitchesId::~GetNeighborSwitchesId() { }

GetNeighborSwitchesId::Request::~Request() { }

GetNeighborSwitchesId::Response::~Response() { }
