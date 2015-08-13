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

#include "agent-framework/command/compute/get_storage_controller_info.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! Stubs GetStorageControllerInfo implementation */
class GetStorageControllerInfo : public compute::GetStorageControllerInfo {
public:
    GetStorageControllerInfo() {}

    using compute::GetStorageControllerInfo::execute;

    void execute(const Request&, Response& response) {
        response.set_drive_count(1);
        response.set_interface("SAS");
        response.set_status({"Enabled", "OK"});
        response.set_fru_info({ "123fed3029c-b23394-12",
                                "Intel Corporation",
                                "E323",
                                "29ee2220939"});
    }

    ~GetStorageControllerInfo();
};

GetStorageControllerInfo::~GetStorageControllerInfo() {}

static Command::Register<GetStorageControllerInfo> g("Stubs");
