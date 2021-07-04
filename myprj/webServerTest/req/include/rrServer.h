/**
 * @file rrServer.h
 * @brief rrServer
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_rrServer_H__
#define __STIBEL_rrServer_H__

#include <string>
#include "zhelpers.hpp"
#include "StiBel/Data/MySQL/MySQL.h"
#include "StiBel/Util.h"
#include "StiBel/JSON/JsonUtil.h"
#include "StiBel/Log/LoggerManager.h"
#include <vector>

using namespace StiBel::Data::MySQL;
using namespace StiBel::JSON;
using StiBel::StringUtil;

class rrServer
{
public:
	rrServer();
	~rrServer();
	void init();
	void start();
private:
	void connMySql();

private:
	DECLARE_STATIC_LOGGERMANAGER();
	zmq::context_t *context;
	zmq::socket_t *responder;
	MySQL::ptr _mySql;
};

#endif //__STIBEL_rrServer_H__