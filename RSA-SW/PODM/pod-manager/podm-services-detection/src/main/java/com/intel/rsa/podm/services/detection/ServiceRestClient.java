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

package com.intel.rsa.podm.services.detection;

import com.intel.rsa.podm.services.detection.api.ServiceDetectionException;
import com.intel.rsa.podm.services.detection.api.ServiceEndpointCandidate;
import org.jboss.resteasy.client.jaxrs.ResteasyClient;
import org.jboss.resteasy.client.jaxrs.ResteasyClientBuilder;

import javax.enterprise.context.RequestScoped;
import javax.ws.rs.ProcessingException;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Response;
import java.util.HashMap;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

@RequestScoped
public class ServiceRestClient {

    private static final String UUID_JSON_PROPERTY = "UUID";
    private static final long CLIENT_TIMEOUT_SECONDS = 2L;

    /**
     * Method tries to obtain UUID from body of HTTP response
     * returned from GET request on given URI of a service.
     *
     * Method throws if service is not available, e.g.:
     * - returns HTTP status other than SUCCESSFUL (status code 2xx)
     * - service URI is unreachable (within 2 seconds timeout)
     * - an error occurred during examining response body
     *
     * @param candidate Candidate service containing URI to be examined if it qualifies for discovery
     * @return UUID of examined service
     * @throws ServiceDetectionException
     */
    public UUID obtainServiceUuidFromCandidate(ServiceEndpointCandidate candidate) throws ServiceDetectionException {

        ResteasyClient client = getResteasyClient();
        try {
            WebTarget target = client.target(candidate.getEndpointUri());
            Response response = target.request().get();

            if (response.getStatusInfo().getFamily() == Response.Status.Family.SUCCESSFUL) {
                @SuppressWarnings("unchecked")
                HashMap<String, String> value = response.readEntity(HashMap.class);

                response.close();
                return UUID.fromString(value.get(UUID_JSON_PROPERTY));
            } else {
                throw new WebApplicationException(response);
            }
        } catch (ProcessingException | WebApplicationException e) {
            throw new ServiceDetectionException("URI unavailable: " + candidate.getEndpointUri().toString());
        } finally {
            client.close();
        }
    }

    private ResteasyClient getResteasyClient() {
        ResteasyClientBuilder clientBuilder = new ResteasyClientBuilder()
            .connectionPoolSize(1)
            .connectionTTL(CLIENT_TIMEOUT_SECONDS, TimeUnit.SECONDS)
            .establishConnectionTimeout(CLIENT_TIMEOUT_SECONDS, TimeUnit.SECONDS)
            .socketTimeout(CLIENT_TIMEOUT_SECONDS, TimeUnit.SECONDS);

        return clientBuilder.build();
    }
}
