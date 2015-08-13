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

package com.intel.rsa.podm.rest.filters;


import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.intel.rsa.common.logger.Logger;
import com.intel.rsa.podm.rest.representation.json.errors.ErrorResponse;
import org.apache.commons.validator.routines.UrlValidator;
import org.jboss.resteasy.plugins.server.servlet.FilterDispatcher;

import javax.inject.Inject;
import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.ws.rs.core.MediaType;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;

/**
 * URL Validator Filter
 * Extends RESTEasy FilterDispatcher and check if provided URL request met basic requirements and is properly built.
 *
 * @see org.jboss.resteasy.plugins.server.servlet.FilterDispatcher
 */
public class UrlValidationFilter extends FilterDispatcher {

    private static final int URL_MAX_LENGTH_IN_BYTES = 8000; //due to RFC 7230: 3.1.1. Request Line - min is 8000 octets
    private static final String DEFAULT_ENCODING = "UTF-8";
    private static final String DEFAULT_ENCODING_HTTP = "utf8";

    @Inject
    private Logger logger;

    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {

        HttpServletRequest httpServletRequest = (HttpServletRequest) servletRequest;
        HttpServletResponse httpServletResponse = (HttpServletResponse) servletResponse;

        if (isUrlValid(httpServletRequest, httpServletResponse)) {
            super.doFilter(servletRequest, servletResponse, filterChain);
        }
    }

    private boolean isUrlValid(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse) {
        if (String.format("%s %s HTTP/1.1\n\r", httpServletRequest.getMethod(), httpServletRequest.getRequestURL())
                .getBytes(StandardCharsets.UTF_8).length > URL_MAX_LENGTH_IN_BYTES) {
            logger.w("Requested URI exceeds maximum URI length which is set to {} octets", URL_MAX_LENGTH_IN_BYTES);
            setErrorResponse(httpServletResponse, ErrorResponse.URI_TOO_LONG);
            return false;
        }

        String url = httpServletRequest.getRequestURL().toString();
        UrlValidator urlValidator = new UrlValidator(UrlValidator.ALLOW_LOCAL_URLS);
        if (urlValidator.isValid(url) && isUrlDecodingPossible(url)) {
            return true;
        } else {
            setErrorResponse(httpServletResponse, ErrorResponse.MALFORMED_URI);
            return false;
        }
    }

    private boolean isUrlDecodingPossible(String url) {
        boolean canDecodeUrl = false;
        try {
            //check if it is possible to decode URL. UTF-8 is a standard coding by W3C
            //URLDecode.decode() may throw exception "IllegalArgumentException" for bad encoded url
            URLDecoder.decode(url, DEFAULT_ENCODING);
            canDecodeUrl = true;
        } catch (UnsupportedEncodingException | IllegalArgumentException e) {
            logger.w("Requested URI {} is malformed", url);
        }

        return canDecodeUrl;
    }

    private void setErrorResponse(HttpServletResponse httpServletResponse, ErrorResponse error) {
        httpServletResponse.reset();
        httpServletResponse.resetBuffer();

        //we have to add since this request is outside our app
        httpServletResponse.addHeader("OData-Version", "4.0");
        httpServletResponse.addHeader("Cache-Control", "max-age=0");
        httpServletResponse.addHeader("Access-Control-Allow-Origin", "*");

        httpServletResponse.setCharacterEncoding(DEFAULT_ENCODING_HTTP);
        httpServletResponse.setContentType(MediaType.APPLICATION_JSON);
        httpServletResponse.setStatus(error.getErrorCode());

        try (PrintWriter responseWriter = httpServletResponse.getWriter()) {
            responseWriter.print(getJsonResponse(error));
            responseWriter.flush();
        } catch (IOException e) {
            logger.e("Unable to write response. Sending empty body response.", e);
        }
    }

    private String getJsonResponse(ErrorResponse error) throws JsonProcessingException {
        ObjectMapper mapper = new ObjectMapper();
        return mapper.writeValueAsString(error);
    }
}
