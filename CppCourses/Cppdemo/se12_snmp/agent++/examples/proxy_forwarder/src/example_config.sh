#!/bin/sh
# 
# You will need NET-SNMP 4.2.x installed for runnig this script
# It can be downloaded from http://www.net-snmp.org
#
# This configures a ProxyForwarder from SNMPv2c on port 4700
# to SNMPv1 on port 161 (localhost)
# To test the example run the command below after having
# executed this script.
#
# snmpgetnext -v2c -p 4700 localhost proxy_me .1.3
#

SET="snmpset -O s -p 4700 localhost public "

echo "TargetParamsTable:"

$SET snmpTargetParamsRowStatus.\'remote_params_in\' i 5
$SET snmpTargetParamsRowStatus.\'remote_params_out\' i 5
$SET snmpTargetParamsMPModel.\'remote_params_in\' i 1
$SET snmpTargetParamsMPModel.\'remote_params_out\' i 0
$SET snmpTargetParamsSecurityModel.\'remote_params_in\' i 2
$SET snmpTargetParamsSecurityModel.\'remote_params_out\' i 1
$SET snmpTargetParamsSecurityName.\'remote_params_in\' s "proxy_me"
$SET snmpTargetParamsSecurityName.\'remote_params_out\' s "public"
$SET snmpTargetParamsSecurityLevel.\'remote_params_in\' i 1
$SET snmpTargetParamsSecurityLevel.\'remote_params_out\' i 1
$SET snmpTargetParamsStorageType.\'remote_params_in\' i 3
$SET snmpTargetParamsStorageType.\'remote_params_out\' i 3
$SET snmpTargetParamsRowStatus.\'remote_params_in\' i 1
$SET snmpTargetParamsRowStatus.\'remote_params_out\' i 1

echo "TargetAddrTable:"

$SET snmpTargetAddrRowStatus.\'remote_target_out\' i 5
$SET snmpTargetAddrTDomain.\'remote_target_out\' o .1.3.6.1.6.1.1
$SET snmpTargetAddrTAddress.\'remote_target_out\' x 7F00000100A1 
$SET snmpTargetAddrTimeout.\'remote_target_out\' i 1500
$SET snmpTargetAddrRetryCount.\'remote_target_out\' i 3
$SET snmpTargetAddrTagList.\'remote_target_out\' s "Proxy_Me_p"
$SET snmpTargetAddrParams.\'remote_target_out\' s "remote_params_out"
$SET snmpTargetAddrStorageType.\'remote_target_out\' i 3
$SET snmpTargetAddrRowStatus.\'remote_target_out\' i 1

echo "CommunityTable:"

$SET snmpCommunityStatus.\'Proxy_Me_c\' i 5
$SET snmpCommunityName.\'Proxy_Me_c\' s "proxy_me"
$SET snmpCommunitySecurityName.\'Proxy_Me_c\' s dummy_security_name
$SET snmpCommunityContextEngineID.\'Proxy_Me_c\' s "dummy_context_engine_id"
$SET snmpCommunityContextName.\'Proxy_Me_c\' s "dummy_context_name"
#$SET snmpCommunityTransportTag.\'Proxy_Me_c\' s "" 
$SET snmpCommunityStorageType.\'Proxy_Me_c\' i 3
$SET snmpCommunityStatus.\'Proxy_Me_c\' i 1


echo "ProxyEntry:"

$SET snmpModules.14.1.2.1.9.80.114.111.120.121.95.77.101.95.112 i 5
$SET snmpModules.14.1.2.1.2.80.114.111.120.121.95.77.101.95.112 i 1
$SET snmpModules.14.1.2.1.3.80.114.111.120.121.95.77.101.95.112 s "dummy_context_engine_id"
$SET snmpModules.14.1.2.1.4.80.114.111.120.121.95.77.101.95.112 s "dummy_context_name"
$SET snmpModules.14.1.2.1.5.80.114.111.120.121.95.77.101.95.112 s "remote_params_in"
$SET snmpModules.14.1.2.1.6.80.114.111.120.121.95.77.101.95.112 s "remote_target_out"
#$SET snmpModules.14.1.2.1.7.80.114.111.120.121.95.77.101.95.112 s ""
$SET snmpModules.14.1.2.1.8.80.114.111.120.121.95.77.101.95.112 i 3
$SET snmpModules.14.1.2.1.9.80.114.111.120.121.95.77.101.95.112 i 1

echo "done."
