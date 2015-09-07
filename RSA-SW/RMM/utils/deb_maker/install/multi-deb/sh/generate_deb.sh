#!/bin/bash
CUR_PATH=$(cd "$(dirname "$0")"; pwd)

function generate_deb()
{
    chmod +x $1/DEBIAN/post*
    dpkg -b $1 $1.deb
    mv $1.deb $1/../bin/
}

declare -a arr=(
    "rsa-rmm-api"
    "rsa-rmm-base"
    "rsa-rmm-consolecontrol"
    "rsa-rmm-all"
    "rsa-net-snmp")
    
for i in "${arr[@]}"
do
   generate_deb $CUR_PATH/../$i
done  
