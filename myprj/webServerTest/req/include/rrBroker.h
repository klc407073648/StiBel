/**
 * @file rrBroker.h
 * @brief rrBroker
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_rrBroker_H__
#define __STIBEL_rrBroker_H__

#include <string>
#include "StiBel/Log/LoggerManager.h"
#include <map>
#include <vector>
#include "zhelpers.hpp"

class rrBroker
{
public:
  rrBroker();
  ~rrBroker();
  void init();
  void start();

private:
  DECLARE_STATIC_LOGGERMANAGER();
  //map<std::string, std::string> front2back;
  std::vector<std::string> front2back;
  zmq::context_t *context;
  zmq::socket_t *frontend;
  zmq::socket_t *backend;
};

#endif //__STIBEL_rrBroker_H__