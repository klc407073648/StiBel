/**
 * @file ZeroMqUtil.h
 * @brief ZeroMqUtil
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_ZeroMqUtil_H__
#define __STIBEL_ZeroMqUtil_H__
#include <string>

//使用多种协议，inproc（进程内）、ipc（进程间）、tcp、pgm（广播）、epgm；
enum ZeroMqProtocol
{
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
	/**
     * @brief  通过zeromq发送消息.
     *
     * @return 接收targetObj返回的消息字符串
     */
	static std::string sendZeroMqMessage(ZeroMqProtocol protocol, const std::string &targetObj, const std::string &sendContent, int timeOut = 0);

private:
};

#endif //__STIBEL_ZeroMqUtil_H__