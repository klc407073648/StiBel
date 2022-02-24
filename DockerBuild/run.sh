#!/bin/bash
source ./common.sh

function runMyprj() 
{
    logDebug "runMyprj begin"

    cd $work_path/deploy

    ./webServer_main &
    ./rrbroker_main &
    ./rrserver_main &

    logDebug "runMyprj end"
}

function runSwd() {

    logDebug "runSwd begin"

    cd $work_path/DockerBuild

    ./swd.sh &

    logDebug "runSwd end"
}

function MAIN() 
{
    logDebug "run.sh MAIN begin"

    runMyprj
    runSwd

    logDebug "run.sh MAIN end"
}

MAIN
