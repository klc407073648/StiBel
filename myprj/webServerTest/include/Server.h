/**
 * @file Server.h
 * @brief Server
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_SERVER_H__
#define __STIBEL_SERVER_H__

#include "StiBel/Log/LoggerManager.h"

class Server
{
public:
  Server(int threadNum, int port, int queueSize);
  ~Server() {}
  void start();
  void stop() { _started = false; }

private:
  DECLARE_STATIC_LOGGERMANAGER();
  int _threadNum;
  int _port;
  int _queueSize;
  int _listenFd;
  bool _epollInited;
  bool _threadPollInited;
  bool _started;
};

#endif //__STIBEL_SERVER_H__