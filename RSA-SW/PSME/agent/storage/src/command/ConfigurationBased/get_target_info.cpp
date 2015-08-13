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

#include "agent-framework/command/storage/get_target_info.hpp"
#include "agent-framework/module/module_manager.hpp"

using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! Dummy GetTargetInfo implementation */
class GetTargetInfo : public storage::GetTargetInfo {
public:
    GetTargetInfo() {}

    using storage::GetTargetInfo::execute;

    void execute(const Request& request, Response& response) {
       const auto target = ModuleManager::find_target(
                                                request.get_target()).lock();
        if (!target) {
            log_warning(GET_LOGGER("rpc"),
                        "Target = " << request.get_target() << " not found.");
            throw exception::NotFound();
        }

        response.set_target_data(*target);
    }

    ~GetTargetInfo();
};

GetTargetInfo::~GetTargetInfo() {}


static Command::Register<GetTargetInfo> g("ConfigurationBased");
