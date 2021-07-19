#!/bin/bash
#日志路径和文件
LOG_DIR=/home/project/StiBel/logs/logpath
LOG_FILE=$LOG_DIR/my.log

#当前日志级别
logLevel=1

#日志级别
logDebugNum=1
logInfoNum=2
logWarnNum=3
logErrorNum=4

#日志打印函数
function write_log()
{
  if [ ! -d ${LOG_DIR} ];then
	 mkdir -p ${LOG_DIR} 
  fi
  
  if [ ! -d ${LOG_FILE} ];then
	 touch  ${LOG_FILE} 
  fi
  
  fileCount=`ls ${LOG_DIR}/*.log |wc -l`
  
  if [ ${fileCount} -gt 10 ];then
     find ${LOG_DIR} -type f |xargs ls -tr | head -5 | xargs rm
  fi
  
  echo -n `date "+%Y-%m-%d %T"` >>${LOG_FILE}
  echo " $1" >>${LOG_FILE}
  
  return 0
}

#打印各类级别日志函数
function logDebug()
{
  if [ $logDebugNum -ge $logLevel ] ; then
      echo -e "\033[32m--------------- $1 ---------------\033[0m" && write_log $1
  fi
}

function logInfo()
{
  if [ $logInfoNum -ge $logLevel ] ; then
      echo -e "\033[36m--------------- $1 ---------------\033[0m" && write_log $1
  fi
}

function logWarn()
{
  if [ $logInfoNum -ge $logLevel ] ; then
      echo -e "\033[33m--------------- $1 ---------------\033[0m" && write_log $1
  fi
}

function logError()
{
  if [ $logInfoNum -ge $logLevel ] ; then
      echo -e "\033[31m--------------- $1 ---------------\033[0m" && write_log $1
  fi
}

function MAIN()
{
  logDebug "logDebug"
  logInfo "logInfo"
  logWarn "logWarn"
  logError "logError"
}

MAIN