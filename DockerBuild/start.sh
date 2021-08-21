#!/bin/bash
curPath=/home/project/StiBel

cd $curPath/deploy
./webServerTest &
./rrbroker_main &
./rrserver_main &

tail -f /dev/null