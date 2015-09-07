#!/bin/bash
CUR_PATH=$(cd "$(dirname "$0")"; pwd)
function changemod()
{
    chmod +x $1/DEBIAN/post*
}
declare -a arr=(
    "rsa-rmm-api"
    "rsa-rmm-base"
    "rsa-rmm-consolecontrol"
    "rsa-rmm-all"
    "rsa-net-snmp")
    
for i in "${arr[@]}"
do
   changemod $CUR_PATH/../$i
done
chmod 777 $CUR_PATH/*.sh
