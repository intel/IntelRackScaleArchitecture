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


#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
#include "libutils/string.h"


static int get_dhcp(char *dhcp)
{
	char dhcp_str[16] = {0};
	FILE *fp = NULL;
	char line[256] = {0};
	int i = 0;

	if (NULL == dhcp) {
		return -1;
	}

	fp = fopen("/etc/dhcp/dhcpd.conf", "r");
	if (NULL == fp) {
		perror("dhcpd.conf");
		return -1;
	}

	while (NULL != fgets(line, 256, fp)) {
		while ((line[i] != '\0')
			   && (line[i] != '\n')
			   && (line[i] != '\r')) {
			if (line[i] == ' ') {
				i++;
				continue;
			} else if (line[i] == '#') {
				break;
			} else {
				if (strncmp("subnet", line + i, 6) == 0) {
					if (sscanf(line + i, "subnet%s%*s", dhcp_str) != 1)
						return -1;

					memcpy(dhcp, dhcp_str, 16);
					return 0;
				}

				break;
			}
		}
	}

	return -1;
}

static int get_local_ip_by_dhcp(char *dhcp, char *ip)
{
	struct ifaddrs *ifas = NULL;
	struct ifaddrs *ifa = NULL;
	int dhcp_pos[4] = {0};
	char dhcp_head[64] = {0};
	int rc = -1;
	struct sockaddr_in *addr = NULL;
	char *p = NULL;

	if (sscanf(dhcp, "%d.%d.%d.%d", &dhcp_pos[0], &dhcp_pos[1],
			   &dhcp_pos[2], &dhcp_pos[3]) != 4) {
		return -1;
	}

	rc = snprintf(dhcp_head, sizeof(dhcp_head), "%d.%d.%d.", dhcp_pos[0], dhcp_pos[1], dhcp_pos[2]);
	if (rc <= 0)
		return -1;

	if (getifaddrs(&ifas) != 0) {
		return -1;
	}

	for (ifa = ifas; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr != NULL) {
			addr = (struct sockaddr_in *)ifa->ifa_addr;
			p = inet_ntoa(addr->sin_addr);
			if (NULL == p)
				return -1;

			if (strncmp(p, dhcp_head, strlen(dhcp_head)) == 0) {
				memcpy(ip, p, 16);
				return 0;
			}
		}
	}
	return 0;
}

static int get_local_ip_by_route(char *ip)
{
	int sock_fd = -1;
	struct ifreq ifr;
	struct sockaddr_in *addr = NULL;
	char *p = NULL;
	char iface[16] = {0};
	char dst[16] = {0};
	FILE *fp = NULL;
	char buf[256] = {0};

	fp = fopen("/proc/net/route", "r");
	if (NULL == fp)
		return -1;

	while (NULL != fgets(buf, 256, fp)) {
		if (sscanf(buf, "%s%s%*s", iface, dst) != 2)
			continue;

		if (strcmp(dst, "00000000") == 0) {
			break;
		}
	}

	fclose(fp);

	strncpy_safe(ifr.ifr_name, iface, IFNAMSIZ, IFNAMSIZ - 1);

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock_fd < 0) {
		return -1;
	}

	if (ioctl(sock_fd, SIOCGIFADDR, &ifr) < 0) {
		goto err;
	}

	addr = (struct sockaddr_in *)&(ifr.ifr_addr);
	p = inet_ntoa(addr->sin_addr);

	memcpy(ip, p, 16);

	close(sock_fd);

	return 0;

err:
	close(sock_fd);

	return -1;
}

/**
 * @brief: the length for the buffer "ip" should >= 16
 */
int libutils_get_ip(char *ip)
{
	char dhcp[16] = {0};

	if (NULL == ip)
		return -1;

	if (get_dhcp(dhcp) == 0) {
		if (get_local_ip_by_dhcp(dhcp, ip) == 0)
			return 0;
	} else {
		if (get_local_ip_by_route(ip) == 0)
			return 0;
	}

	return -1;
}

