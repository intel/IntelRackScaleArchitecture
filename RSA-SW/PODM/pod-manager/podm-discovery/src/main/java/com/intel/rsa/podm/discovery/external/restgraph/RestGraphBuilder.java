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

package com.intel.rsa.podm.discovery.external.restgraph;

import com.intel.rsa.client.api.reader.RsaReader;
import com.intel.rsa.client.api.reader.RsaApiReaderException;
import com.intel.rsa.client.api.reader.RsaResource;
import com.intel.rsa.common.logger.Logger;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.net.URI;
import java.util.ArrayDeque;
import java.util.Collection;
import java.util.HashSet;
import java.util.Queue;
import java.util.Set;

import static java.util.stream.Collectors.toList;
import static java.util.stream.StreamSupport.stream;

/**
 * Visits all resources from rssClient and creates graph from them
 */
@Dependent
public class RestGraphBuilder {
    @Inject
    private Logger logger;

    @Inject
    private ResourceLinkExtractor extractor;

    public RestGraph build(RsaReader client) {
        RsaResource serviceRoot;
        try {
            serviceRoot = client.getServiceRoot();
        } catch (RsaApiReaderException e) {
            logger.e("Problem while reading: " + e.getResourceUri(), e);
            return new RestGraph();
        }

        return buildFrom(serviceRoot);
    }

    private RestGraph buildFrom(RsaResource start) {
        RestGraph graph = new RestGraph();

        Queue<RsaResource> queue = new ArrayDeque<>();
        Set<URI> visited = new HashSet<>();

        queue.add(start);

        while (!queue.isEmpty()) {
            RsaResource current = queue.poll();
            visited.add(current.getUri());

            Iterable<ResourceLink> relatedLinks = extractor.extractFrom(current);
            Collection<? extends RsaResource> notVisited = filterNotVisited(relatedLinks, visited);

            graph.addAll(relatedLinks);
            queue.addAll(notVisited);
        }

        return graph;
    }

    private Collection<? extends RsaResource> filterNotVisited(Iterable<ResourceLink> links, Set<URI> visited) {
        return stream(links.spliterator(), false)
                .map(ResourceLink::getTarget)
                .filter(resource -> !visited.contains(resource.getUri()))
                .collect(toList());
    }
}
