#include "ZeroMqUtil.h"
#include "zhelpers.hpp"

CREATE_STATIC_LOGGERMANAGER(ZeroMqUtil);

ZeroMqUtil::ZeroMqUtil()
{
}

ZeroMqUtil::~ZeroMqUtil()
{
}

std::string ZeroMqUtil::sendZeroMqMessage(ZeroMqProtocol protocol, const std::string &connectObj, const std::string &sendContent, int timeOut)
{
  LogDebug("ZeroMqUtil::sendZeroMqMessage protocol:[%d],connectObj:[%s],sendContent:[%s],timeOut:[%d]",protocol,connectObj.c_str(),sendContent.c_str(),timeOut);
  
  zmq::context_t context(1);
  zmq::socket_t requester(context, ZMQ_REQ);

  if (timeOut != 0)
  {
    zmq_setsockopt(requester, ZMQ_RCVTIMEO, &timeOut, sizeof(timeOut));
    zmq_setsockopt(requester, ZMQ_SNDTIMEO, &timeOut, sizeof(timeOut));
  }

  std::string connectStr = "";

  switch (protocol)
  {
    case TCP:
      connectStr = "tcp://" + connectObj;break;
    case IPC:
      connectStr = "ipc://" + connectObj + ".ipc";break;
    default:
      break;
  }

  LogDebug("ZeroMqUtil::sendZeroMqMessage connectStr:[%s]",connectStr.c_str());
  
  requester.connect(connectStr);

  s_send(requester, sendContent);

  std::string recvInfo = s_recv(requester);

  LogDebug("ZeroMqUtil::sendZeroMqMessage Received reply:[%s]",recvInfo.c_str());
  
  return recvInfo;
}
