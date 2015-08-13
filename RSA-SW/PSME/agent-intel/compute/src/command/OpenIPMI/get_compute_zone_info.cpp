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

#include "agent-framework/command/compute/get_compute_zone_info.hpp"

using namespace agent_framework::generic;
using namespace agent_framework::command;


class GetComputeZoneInfo : public agent_framework::command::compute::GetComputeZoneInfo {
public:
    GetComputeZoneInfo() { }

    using agent_framework::command::compute::GetComputeZoneInfo::execute;

    void execute(const Request&, Response& response) {
        response.set_status({"Enabled", "OK"});

        Collection collection;
        Collection::CollectionEntry compute_entry("Compute", "Compute");
        collection.add_collection_entry(compute_entry);
        response.set_collection(collection);

        response.set_oem({});
    }

    ~GetComputeZoneInfo();
};

GetComputeZoneInfo::~GetComputeZoneInfo() { }

static Command::Register<GetComputeZoneInfo> g("Intel");
