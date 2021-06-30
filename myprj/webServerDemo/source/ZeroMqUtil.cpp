#include "ZeroMqUtil.h"
#include "zhelpers.hpp"

ZeroMqUtil::ZeroMqUtil()
{
}

ZeroMqUtil::~ZeroMqUtil()
{
}

std::string ZeroMqUtil::sendZeroMqMessage(ZeroMqProtocol protocol, const std::string &targetObj, const std::string &sendContent, int timeOut)
{

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
      connectStr = "tcp://" + targetObj;break;
    case IPC:
      connectStr = "ipc://" + targetObj + ".ipc";break;
    default:
      break;
  }

  requester.connect(connectStr);

  s_send(requester, sendContent);

  std::string recvInfo = s_recv(requester);

  std::cout << "Received reply "<< " [" << recvInfo << "]" << std::endl;
  
  return recvInfo;
}
