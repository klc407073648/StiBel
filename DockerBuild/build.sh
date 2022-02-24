#!/bin/bash
source ./common.sh

function preDeal() 
{
    logDebug "preDeal begin"

    cp -r /usr/lib64/mysql/* /usr/lib/
    rm -rf $work_path/logs/log4cpp/*

    cd $work_path/DockerBuild
    chmod 777 *.sh
    dos2unix *.sh

    logDebug "preDeal end"
}

function buildMyprj() 
{
    logDebug "buildMyprj begin"

    cd $work_path
    buildTime=`date +"%Y%m%d"`
    tar zxf StiBel_${buildTime}.tar.gz

    logInfo "work_path:$work_path"

    cd $work_path/build

    rm -rf ./*

    cmake -DSTIBEL_BUILD_RELEASE_TYPE=ON -DSTIBEL_BUILD_EXAMPLES=OFF ..
    make -j4

    checkBuildResult buildMyprj

    logDebug "buildMyprj end"
}

function MAIN() 
{
    logDebug "build.sh MAIN begin"

    preDeal
    buildMyprj

    logDebug "build.sh MAIN end"
}

MAIN
