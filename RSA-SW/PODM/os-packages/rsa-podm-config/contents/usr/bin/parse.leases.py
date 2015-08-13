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

#!/usr/bin/env python


from optparse import OptionParser
import sys
import datetime
import re

# iSCSI machine parameters
ISCSI_HOSTNAME = 'iSCSI'
ISCSI_LID = '.0.0.0'

# DHCP events
DHCP_EVENT_COMMIT = "commit"
DHCP_EVENT_RELEASE = "release"
DHCP_EVENT_EXPIRE = "expiry"

# OPTIONS
OP_STRING = "op"
IP_STRING = "ip"
MAC_ADDR_STRING = "mac_addr"
DHCP_EVENT_STRING = "dhcp_event"

def get_options():
    """ Parses user input. """

    parser = OptionParser()

    parser.add_option("-o", "--option", dest=OP_STRING,
                      help="IP host-name option value", metavar="OP")

    parser.add_option("-i", "--ip", dest=IP_STRING,
                      help="IP address.", metavar="IP")

    parser.add_option("-a", "--mac-addr", dest=MAC_ADDR_STRING,
                      help="MAC address.", metavar="MAC_ADDR")

    parser.add_option("-e", "--dhcp-event", dest=DHCP_EVENT_STRING,
                      help="DHCP event type. Please use one of the following:\n commit, release, expiry.", metavar="DHCP_EVENT")

    (options, args) = parser.parse_args()


    if (not options.ip):
        print("You have to specify IP address!!!")
        sys.exit(127)

    if (not options.mac_addr):
        print("You have to specify MAC address!!!")
        sys.exit(127)

    if (not options.dhcp_event):
        print("You have to specify DHCP event type!!!")
        sys.exit(127)
    else:
        if not (options.dhcp_event == DHCP_EVENT_COMMIT or \
                options.dhcp_event == DHCP_EVENT_RELEASE or \
                options.dhcp_event == DHCP_EVENT_EXPIRE):
            print("DHCP event " + options.dhcp_event + " does not exist.")
            print("Please use one of the following: commit, release, expiry.")
            sys.exit(127)
    if (not options.op):
        options.op = "??"
    return options


def main():

    datafile_path = '/tmp/leases'
    logfile_path = '/tmp/parse.lesases.txt'

    # Get input variables
    options = get_options()
    # Create logfile
    try:
        logfile = open(logfile_path, 'a')
    except IOError:
        print("Cannot create the log file!!!")
        sys.exit(127)
    # Open or create data file
    try:
        datafile = open(datafile_path, 'r')
        leases = datafile.readlines()

    except IOError:
        datafile = open(datafile_path, 'w')
        leases = ''

    datafile.close()

    # Find current interface in the data file

    found_mac = 0
    new_lease = 1
    hostname = ''
    location = ''
    # Check what to do
    if (options.dhcp_event == DHCP_EVENT_COMMIT):
        print "Commit\n"

	if (options.op == ISCSI_HOSTNAME):
            hostname = ISCSI_HOSTNAME
            location = ISCSI_LID
        else:
            token = options.op.split("_")
            hostname = token[0]
            if (len(token) > 1):
                location = ".1" + token[1][1:-2]

        if leases != '':
            for linia in leases:
                token = linia.split(' ')
                if token[0] == options.mac_addr:
                        found_mac = 1
                        if (token[1] == options.ip):
                            new_lease = 0
                            break
                        else:
                            logfile.write(str(datetime.datetime.now()) + '  Changed lease to ' + str(options.ip) + ' on ' + str(options.mac_addr) + '\n')
            if new_lease == 1:
                # Open datafile for writing
                datafile = open(datafile_path, 'w')

                for linia in leases:
                    token = linia.split(' ')
                    if token[0] == options.mac_addr:
                        found_mac = 1
                        datafile.write(str(options.mac_addr) + ' ' + str(options.ip) + ' ' + str(hostname) + ' ' + str(location) + '\n')
                    else:
                        datafile.write(linia)
                datafile.flush()
                datafile.close()

        if found_mac == 0:
            # Open datafile for writing
            datafile = open(datafile_path, 'a')

            datafile.write(str(options.mac_addr) + ' ' + str(options.ip) + ' ' + str(hostname) + ' ' + str(location) + '\n')

            datafile.flush()
            datafile.close()


        logfile.write(str(datetime.datetime.now()) + '  Commited ' + str(options.ip) + ' on ' + str(options.mac_addr) + '\n')

    elif (options.dhcp_event == DHCP_EVENT_RELEASE):
        if leases != '':
            # Open datafile for writing
            datafile = open(datafile_path, 'w');
            for linia in leases:
                mac = linia.split(' ')
                if mac[0] != options.mac_addr:
                    datafile.write(linia)
            datafile.flush()
            datafile.close()
        logfile.write(str(datetime.datetime.now()) + '  Released ' + str(options.ip) + ' on ' + str(options.mac_addr) + '\n')

    elif (options.dhcp_event == DHCP_EVENT_EXPIRE):
        if leases != '':
            for linia in leases:
                mac = linia.split(' ')
                if mac[0] != options.mac_addr:
                    datafile.write(linia)
        logfile.write(str(datetime.datetime.now()) + '  Expired ' + str(options.ip) + ' on ' + str(options.mac_addr) + '\n')

    logfile.flush()
    logfile.close()
    sys.exit(0)
if __name__ == '__main__':
    main()
