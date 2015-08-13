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

#include "psme/rest/node/crud/root.hpp"
#include "psme/rest/resource/resource.hpp"
#include "psme/rest/http/http_status_code.hpp"
#include "psme/rest/http/server.hpp"

static constexpr char WELCOME[] = R"(<!DOCTYPE html>
<html>
<body>
<a href="/rest/v1" style="text-decoration:none"><pre>
      __     ____  ____  __  __ _____   ____  _____ ____ _____      _    ____ ___     __
  ____\ \   |  _ \/ ___||  \/  | ____| |  _ \| ____/ ___|_   _|    / \  |  _ \_ _|   / /____
 |_____\ \  | |_) \___ \| |\/| |  _|   | |_) |  _| \___ \ | |     / _ \ | |_) | |   / /_____|
 |_____/ /  |  __/ ___) | |  | | |___  |  _ <| |___ ___) || |    / ___ \|  __/| |   \ \_____|
      /_/   |_|   |____/|_|  |_|_____| |_| \_\_____|____/ |_|   /_/   \_\_|  |___|   \_\
</pre></a>
</body>
</html>)";

using namespace psme::rest::node;
using psme::rest::resource::ResourceUPtr;

constexpr const char Root::TYPE[];

Root::Root(const string& uuid, const string& gami_id, const string& id)
    : Node(uuid, gami_id, id, ResourceUPtr(new Resource(TYPE)))
{ }

Root::~Root() { }

void Root::get(const Request&, Response& response) {
    response.set_reply(http::HttpStatusCode::OK, WELCOME, "text/html");
}
