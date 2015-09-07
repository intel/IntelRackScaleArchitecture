/**
 * Copyright (c)  2015, Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <errno.h>


#define TIMESTAMP_LENGTH	80


/**
 * @Ex: "2013-01-10T01:09:57"
 */
static int get_local_time(char *output)
{
	struct tm *tm;
	time_t timep;
	char timestamp[TIMESTAMP_LENGTH] = {0};

	if (NULL == output)
		return -1;

	time(&timep);
	tm = localtime(&timep);

	snprintf(timestamp, sizeof(timestamp), "%d-%02d-%02dT%02d:%02d:%02d",
				(tm->tm_year + 1900), (tm->tm_mon + 1), tm->tm_mday,
				tm->tm_hour, tm->tm_min, tm->tm_sec);

	memcpy(output, timestamp, TIMESTAMP_LENGTH);

	return 0;
}


/**
 * @Ex: "2013-01-10T01:09:57+08:00"
 */
static int get_iso8601_time(char *output)
{
	struct timeval tv;
	struct timezone tz;
	char buff[TIMESTAMP_LENGTH] = {0};
	char timestamp[TIMESTAMP_LENGTH + 4] = {0};
	int tz_min = 0;

	if (NULL == output)
		return -1;

	get_local_time(buff);

	gettimeofday(&tv, &tz);

	tz_min = abs(tz.tz_minuteswest);
	if (0 == tz.tz_minuteswest) {
		snprintf(timestamp, sizeof(timestamp), "%sZ", buff);
	} else {
		snprintf(timestamp, sizeof(timestamp), "%s%s%02d:%02d", buff,
				(tz.tz_minuteswest < 0) ? "+" : "-",
				tz_min / 60, tz_min % 60);
	}

	memcpy(output, timestamp, TIMESTAMP_LENGTH + 4);

	return 0;
}


/**
 * @Ex: "2013-01-10T01:09:57+08:00"
 */

int libutils_get_time(char* name, char* value)
{
	return get_iso8601_time(value);
}
