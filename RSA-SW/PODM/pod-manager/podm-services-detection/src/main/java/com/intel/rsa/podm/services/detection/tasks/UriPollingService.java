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

package com.intel.rsa.podm.services.detection.tasks;

import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.services.detection.api.DiscoveryService;
import com.intel.rsa.podm.services.detection.api.ServiceDetection;
import com.intel.rsa.podm.services.detection.api.ServiceDetectionException;
import com.intel.rsa.podm.services.detection.api.ServiceEndpoint;
import com.intel.rsa.podm.services.detection.api.ServiceEndpointCandidate;

import javax.ejb.Lock;
import javax.ejb.LockType;
import javax.ejb.Singleton;
import javax.inject.Inject;
import java.net.URI;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import static com.intel.rsa.podm.services.detection.filesystem.ServiceEndpointCandidateSupplier.getEndpointCandidates;

/**
 * Class responsible for checking URIs for service presence and notifying
 * discovery service (providing it with service endpoints) if a valid
 * service was detected at given URI. Notifications about known, discovered
 * services are not repeated.
 *
 * Failed checks are retried, but after failing MAX_FAILED_RETRIES times
 * a polled service URI is not checked again until directly scheduled
 * for re-check.
 *
 * TODO add tests and consider using managed executor to perform parallel URI checking
 */
@Singleton
@Lock(LockType.READ)
public class UriPollingService {

    private static final int MAX_FAILED_RETRIES = 5;

    /**
     * Holds all valid services detected during URI polling
     */
    private final ConcurrentMap<URI, UUID> knownLocations = new ConcurrentHashMap<>();

    /**
     * Holds all failed services with their respective retry counters
     */
    private final ConcurrentMap<ServiceEndpointCandidate, Integer> failedLocations = new ConcurrentHashMap<>();

    @Inject
    private Logger logger;

    @Inject
    private DiscoveryService discoveryService;

    @Inject
    private ServiceDetection serviceDetection;

    /**
     * Method tries to retry all previously checked URIs that have failed and initiates
     * service detection for all URIs obtained from ServiceEndpointCandidateSupplier implementation.
     */
    public void provideEndpoints() {
        retryFailedUris();
        pollEndpointCandidatesForServices(getEndpointCandidates());
    }

    /**
     * Method initiates retry of service detection for previously failed URIs, if retry count
     * does not exceed MAX_FAILED_RETRIES limit.
     */
    private void retryFailedUris() {
        failedLocations.entrySet().stream().filter(entry -> entry.getValue() < MAX_FAILED_RETRIES)
                .forEach(entry -> detectServiceEndpointAtUri(entry.getKey()));
    }

    /**
     * Method polls ServiceEndpointCandidates for service presence if it was not already detected or marked as failed
     *
     * @param candidates set of ServiceEndpointCandidates to be polled for service presence
     */
    private void pollEndpointCandidatesForServices(Set<ServiceEndpointCandidate> candidates) {
        candidates.stream()
            .filter(serviceLocation -> !knownLocations.containsKey(serviceLocation.getEndpointUri())
                    && !failedLocations.containsKey(serviceLocation))
            .forEach(this::detectServiceEndpointAtUri);
    }

    /**
     * Method tries to detect a single service at a given URI and notify discovery service upon success.
     *
     * Detected service is added to knownLocations so it won't be unnecessarily polled later.
     * Failed check puts a URI in failedLocations, or increments its retry counter if it failed before.
     *
     * @param candidate URI to be checked
     */
    private void detectServiceEndpointAtUri(ServiceEndpointCandidate candidate) {
        try {
            ServiceEndpoint serviceEndpoint = serviceDetection.getServiceEndpoint(candidate);
            discoveryService.discover(serviceEndpoint);

            knownLocations.putIfAbsent(serviceEndpoint.getEndpointUri(), serviceEndpoint.getServiceUUID());
            failedLocations.remove(candidate);
        } catch (ServiceDetectionException e) {
            Integer retryCount = failedLocations.putIfAbsent(candidate, 0);
            if (retryCount != null) {
                failedLocations.replace(candidate, retryCount, retryCount + 1);
                logger.e("Valid service could not be detected at {}, retry no {}",  candidate, retryCount + 1);
            } else {
                logger.e("Valid service could not be detected at {}", candidate);
            }
        }
    }

    /**
     * Method iterates through all failed services that were retried MAX_FAILED_RETRIES times
     * and schedules them for re-check. This enables detection of services that take a long time to start.
     */
    public void scheduleReCheckForFailedUris() {
        failedLocations.entrySet().stream().filter(entry -> entry.getValue() >= MAX_FAILED_RETRIES)
            .forEach(entry -> {
                ServiceEndpointCandidate serviceLocation = entry.getKey();
                knownLocations.remove(serviceLocation.getEndpointUri());
                failedLocations.remove(serviceLocation);
                logger.i("Location {} scheduled for re-check", serviceLocation);
            });
    }
}
