#!/bin/bash
curPath=/home/project/StiBel

cd $curPath/deploy
./webServer_main &
./rrbroker_main &
./rrserver_main &

tail -f /dev/null