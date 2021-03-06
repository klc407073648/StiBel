/**
 * @file ZeroMqUtil.h
 * @brief ZeroMqUtil
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_ZeroMqUtil_H__
#define __STIBEL_ZeroMqUtil_H__

#include<string>
#include "StiBel/Log/LoggerManager.h"

//使用多种协议，inproc（进程内）、ipc（进程间）、tcp、pgm（广播）、epgm；
enum ZeroMqProtocol {
		INPROC,
		IPC,
		TCP,
		PGM,
    	EPGM
	};

class ZeroMqUtil
{
public:
  ZeroMqUtil();
  ~ZeroMqUtil();
  static std::string sendZeroMqMessage(ZeroMqProtocol protocol, const std::string &connectObj, const std::string &sendContent, int timeOut=0);
private:
  DECLARE_STATIC_LOGGERMANAGER();
  
};

#endif //__STIBEL_ZeroMqUtil_H__