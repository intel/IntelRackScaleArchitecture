# Copyright (c) 2015 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# iPXE specific settings
option space ipxe;
option iscsi-initiator-iqn code 203 = string;

ddns-update-style none;

default-lease-time 100;
max-lease-time 720;

log-facility local7;

#
# RSA Host network
#
subnet 10.1.0.0 netmask 255.255.255.0 {
	range 10.1.0.150 10.1.0.253;
	option routers 10.1.0.1;
	next-server 10.1.0.1;
}

#
# RSA Data Management network
#
subnet 10.2.0.0 netmask 255.255.255.0 {
    range 10.2.0.2 10.2.0.254;
	option routers 10.2.0.1;
	option ntp-servers 10.2.0.1;

	# For the iPXE boot
	if exists user-class and option user-class = "iPXE" {
        filename "";
    } else {
        filename "rsaipxe.0";
    }

	next-server 10.2.0.1;

	if ((substring((option host-name),0,10) = "ceph-admin") or (substring((option host-name),0,11) = "rsa-storage")) {
        on commit {
                set mac_addr = concat(
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,1,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,2,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,3,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,4,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,5,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,6,1))),2)
                        );
                set ip_addr  = binary-to-ascii(10, 8, ".", leased-address);
                execute("/usr/bin/python", "/usr/bin/parse.leases.py", "-a", mac_addr, "-i", ip_addr, "-e", "commit", "-o", "iSCSI");
        }
        on release {
                set mac_addr = concat(
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,1,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,2,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,3,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,4,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,5,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,6,1))),2)
                        );
                set ip_addr  = binary-to-ascii(10, 8, ".", leased-address);
                execute("/usr/bin/python", "/usr/bin/parse.leases.py", "-a", mac_addr, "-i", ip_addr, "-e", "release", "-o", "iSCSI");
        }
        on expiry {
                set mac_addr = concat(
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,1,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,2,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,3,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,4,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,5,1))),2),":",
                            suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,6,1))),2)
                        );
                set ip_addr  = binary-to-ascii(10, 8, ".", leased-address);
                execute("/usr/bin/python", "/usr/bin/parse.leases.py", "-a", mac_addr, "-i", ip_addr, "-e", "expiry", "-o", "iSCSI");
        }
	}
}

#
# RSA Management network
#
subnet 10.3.0.0 netmask 255.255.255.0 {
	range 10.3.0.220 10.3.0.253;
	option routers 10.3.0.1;
	option ntp-servers 10.3.0.1;

	set lease_hostname = option host-name;
	if (substring((option host-name),0,6) = "rsa-tc") {
		set lease_hostname = "rsa-tc";
	}

	on commit {
		set mac_addr = concat(
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,1,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,2,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,3,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,4,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,5,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,6,1))),2)
        		);
		set ip_addr  = binary-to-ascii(10, 8, ".", leased-address);
		execute("/usr/bin/python", "/usr/bin/parse.leases.py", "-a", mac_addr, "-i", ip_addr, "-e", "commit", "-o", lease_hostname);
	}

	on release {
		set mac_addr = concat(
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,1,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,2,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,3,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,4,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,5,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,6,1))),2)
        		);
		set ip_addr  = binary-to-ascii(10, 8, ".", leased-address);
		execute("/usr/bin/python", "/usr/bin/parse.leases.py", "-a", mac_addr, "-i", ip_addr, "-e", "release", "-o", lease_hostname);
	}
	on expiry {
		set mac_addr = concat(
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,1,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,2,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,3,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,4,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,5,1))),2),":",
        			suffix(concat("0",binary-to-ascii(16,8,"",substring(hardware,6,1))),2)
        		);
		set ip_addr  = binary-to-ascii(10, 8, ".", leased-address);
		execute("/usr/bin/python", "/usr/bin/parse.leases.py", "-a", mac_addr, "-i", ip_addr, "-e", "expiry", "-o", lease_hostname);
	}
}
