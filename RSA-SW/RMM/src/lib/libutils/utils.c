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


#include "libutils/utils.h"
#include "libutils/rmm.h"

bool check_str_len(const char *str, int len)
{
	if ((str == NULL) || (strlen(str) > len)) {
		return FALSE;
	} else
		return TRUE;

	return TRUE;
}

bool check_int_range(int value, int min, int max)
{
	if (((value) < (min)) || ((value) > (max))) {
		return FALSE;
	} else
		return TRUE;

	return TRUE;
}

bool is_str_num(const char *str)
{
	int i = 0;

	if (NULL == str)
		return FALSE;

	for (i = 0; i < strlen(str); i++) {
		if (!((str[i] == '+') || (str[i] == '-') || ((str[i] >= '0') && (str[i] <= '9'))))
			return FALSE;
	}
	return TRUE;
}

bool is_str_uuid(const char *str)
{
	if ((strlen(str) <= ID_STR_MAX_LEN) && (TRUE == is_str_num(str)))
		return FALSE;
	else
		return TRUE;
}


int str2int(const char *str)
{
	int i = 0, tmp = 0;

	if (str == NULL)
		return 0;

	while (str[i] != '\0') {
		if (str[i] >= '0' && str[i] <= '9')
			tmp = tmp*10 + (str[i] - '0');
		i++;
	}
	return tmp;
}

static int uuid_generate_t(char * uuid_str)
{
	int count,label;
	FILE *fp;

	if (!(fp=fopen("/proc/sys/kernel/random/uuid","r"))) {
		printf("Error in open file!\n");
		return -1;
	}

	if (fscanf(fp,"%s", uuid_str)) {
		fclose(fp);

		uuid_str[UUID_MAX_LEN-1] = '\0';

		printf("uuid is %s. str len is %d\n", uuid_str, (int)strlen(uuid_str));
		return 0;
	}

	return -1;
}


int gen_uuid(char *uuid)
{
	char uuid_str[UUID_MAX_LEN] = {0};

	if (NULL == uuid)
		return -1;

	uuid_generate_t(uuid_str);

	memcpy(uuid, uuid_str, UUID_MAX_LEN);

	return 0;
}
