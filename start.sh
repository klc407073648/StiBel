#!/bin/bash
curPath=/home/project/StiBel
cd $curPath
chmod 777 *.sh

source ./common.sh

logInfo "1.begin to build StiBel project"

logDebug "curPath:$curPath"

rm -rf $curPath/logs/log4cpp/*

cd $curPath/build
rm -rf ./*

#cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_BUILD_PROJECT_OPTION=all ..
cmake ..
make -j8

logInfo "2.end to build StiBel project"

logInfo "3.begin to run service"

cd $curPath/deploy
./webServerTest &
./rrbroker_main &
./rrserver_main &

logInfo "4.end to run service"

tail -f /dev/null