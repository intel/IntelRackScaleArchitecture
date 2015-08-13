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
#include "agent-framework/command/compute/get_collection.hpp"

using namespace agent_framework::command;

/*! GetCollection implementation */
class GetCollection : public compute::GetCollection {
public:
    GetCollection() {}

    using compute::GetCollection::execute;

    void execute(const Request&, Response& response) {
        response.add_subcomponent({"123e4567-e89b-12d3-a456-43665544043"});
        response.add_subcomponent({"321e4567-e89b-12d3-a456-43665544043"});
        response.add_subcomponent({"443e4567-e89b-12d3-a456-43665544043"});
        response.add_subcomponent({"323e4567-e89b-12d3-a456-43665544043"});
    }

    ~GetCollection();
};

GetCollection::~GetCollection() {}

static Command::Register<GetCollection> g("Stubs");
