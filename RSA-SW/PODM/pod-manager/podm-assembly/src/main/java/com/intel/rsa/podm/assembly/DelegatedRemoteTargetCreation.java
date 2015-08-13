/*
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.intel.rsa.podm.assembly;

import com.intel.rsa.common.types.Id;

import java.net.URI;
import java.util.concurrent.Future;

public class DelegatedRemoteTargetCreation {

    private final Id nodeId;

    private final Future<URI> futureTargetUri;

    private long creationStartMillis;

    public DelegatedRemoteTargetCreation(Id nodeId, Future<URI> futureTargetUri) {
        this.nodeId = nodeId;
        this.futureTargetUri = futureTargetUri;
        creationStartMillis = System.currentTimeMillis();
    }

    public Id getNodeId() {
        return nodeId;
    }

    public Future<URI> getFutureTargetUri() {
        return futureTargetUri;
    }

    public boolean isTimedOut(long timeoutMillis) {
        long creationTimeElapsed = System.currentTimeMillis() - creationStartMillis;
        return creationTimeElapsed > timeoutMillis;
    }
}
