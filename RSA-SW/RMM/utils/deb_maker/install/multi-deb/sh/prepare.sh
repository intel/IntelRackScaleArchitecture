#!/bin/bash
CUR_PATH=$(cd "$(dirname "$0")"; pwd)
PROJ_PATH=$CUR_PATH/../../../..

function del_file()
{
    for del in $1*
    do
        if [ -f $del ]
        then
            rm -rf $del
        fi
    done
}

#1
ITEM_PATH=$CUR_PATH/../rsa-rmm-base

mkdir -p $ITEM_PATH/usr/local/bin
mkdir -p $ITEM_PATH/usr/local/sbin
mkdir -p $ITEM_PATH/usr/local/lib
mkdir -p $ITEM_PATH/etc
mkdir -p $ITEM_PATH/etc/rmm/
mkdir -p $ITEM_PATH/etc/udev/rules.d
mkdir -p $ITEM_PATH/var/rmm/
mkdir -p $ITEM_PATH/var/rmm/redfish
del_file $ITEM_PATH/usr/local/bin/
del_file $ITEM_PATH/usr/local/lib/
del_file $ITEM_PATH/etc/rmm
del_file $ITEM_PATH/etc/udev/rules.d/
del_file $ITEM_PATH/etc/
del_file $ITEM_PATH/var/rmm/redfish
del_file $ITEM_PATH/var/rmm/

#cp $PROJ_PATH/build/lib/*.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libwrap.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libjson.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/librmmcfg.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libencrypter.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libredfish.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libutils.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/liblog.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libcurl.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libinit.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libjsonrpc.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libjsonrpcapi.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libssl.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/lib/libcrypto.so* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/utils/redfish/MR.json $ITEM_PATH/var/rmm/redfish
cp $PROJ_PATH/utils/cfg/rmm.cfg $ITEM_PATH/etc/rmm
cp $PROJ_PATH/utils/encrypt_text/keyfile $ITEM_PATH/etc/rmm

cp $PROJ_PATH/build/bin/memdbd $ITEM_PATH/usr/local/bin/
cp $PROJ_PATH/build/bin/ipmi_module $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/assetd $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/asset_module $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/registerd $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/rmmlogd $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/upgraded $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/snmp_subagentd $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/cm_reset $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/snmpd $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/rmm_post_install $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/build/bin/rmm_super $ITEM_PATH/usr/local/bin
cp $PROJ_PATH/utils/cfg/80-persistent-tty.rules $ITEM_PATH/etc/udev/rules.d/
cp -r $PROJ_PATH/src/core/restd/web $ITEM_PATH/var/rmm/

#cp $PROJ_PATH/build/bin/in.tftpd $ITEM_PATH/usr/local/sbin/
$CUR_PATH/modify_ver.sh $ITEM_PATH

#2
ITEM_PATH=$CUR_PATH/../rsa-rmm-api

mkdir -p $ITEM_PATH/usr/local/bin
del_file $ITEM_PATH/usr/local/bin/
cp $PROJ_PATH/build/bin/restd $ITEM_PATH/usr/local/bin/
cp $PROJ_PATH/build/bin/redfishd $ITEM_PATH/usr/local/bin/
$CUR_PATH/modify_ver.sh $ITEM_PATH


#3
ITEM_PATH=$CUR_PATH/../rsa-rmm-consolecontrol

mkdir -p $ITEM_PATH/usr/local/bin
del_file $ITEM_PATH/usr/local/bin/

cp $PROJ_PATH/build/bin/dumpmemdb $ITEM_PATH/usr/local/bin
$CUR_PATH/modify_ver.sh $ITEM_PATH

#4
ITEM_PATH=$CUR_PATH/../rsa-rmm-all
$CUR_PATH/modify_ver.sh $ITEM_PATH

#5
ITEM_PATH=$CUR_PATH/../rsa-net-snmp

mkdir -p $ITEM_PATH/usr/local/bin
del_file $ITEM_PATH/usr/local/bin/

mkdir -p $ITEM_PATH/etc/snmp
del_file $ITEM_PATH/etc/snmp/

mkdir -p $ITEM_PATH/usr/local/lib
del_file $ITEM_PATH/usr/local/lib/

#mkdir -p $ITEM_PATH/usr/local/share/snmp/mibs
#del_file $ITEM_PATH/usr/local/share/snmp/mibs

cp $PROJ_PATH/build/lib/libnet* $ITEM_PATH/usr/local/lib
cp $PROJ_PATH/build/bin/snmp* $ITEM_PATH/usr/local/bin/
#copy snmp config file
cp $PROJ_PATH/src/core/snmp_subagentd/config/*.conf $ITEM_PATH/etc/snmp/
#copy mibs file
#cp $PROJ_PATH/src/core/snmp_subagentd/*.txt $ITEM_PATH/usr/local/share/snmp/mibs/
#cp $PROJ_PATH/src/dep/snmp/net-snmp-5.7.3/mibs/*.txt $ITEM_PATH/usr/local/share/snmp/mibs/
#add env 
#export MIBDIRS="/usr/local/share/snmp/mibs"
$CUR_PATH/modify_ver.sh $ITEM_PATH
