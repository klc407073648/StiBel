#include "Server.h"
#include "RequestData.h"
#include "Epoll.h"
#include "ThreadPool.h"
#include "MyUtil.h"
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <memory>

CREATE_STATIC_LOGGERMANAGER(Server);

Server::Server(int threadNum, int port,int queueSize)
    : _threadNum(threadNum),
      _port(port),
      _queueSize(queueSize),
      _started(false),
      _epollInited((Epoll::epoll_init(MAXEVENTS, LISTENQ)) ==0),
      _threadPollInited((ThreadPool::threadpool_create(_threadNum, _queueSize)) ==0),
      _listenFd(socket_bind_listen(_port)) {
  LogDebug("Server::Server() threadNum:[%d],port:[%d],queueSize:[%d]", threadNum, port, queueSize);
  
  handle_for_sigpipe();

  LogDebug("Server::Server()");
  if (!_epollInited)
  {
      LogError("epoll init failed");
      abort();
  }

  if (!_threadPollInited)
  {
      LogError("Threadpool create failed\n");
      abort();
  }

  if (_listenFd < 0) 
  {
    LogError("socket bind failed");
    abort();
  }

  if (setSocketNonBlocking(_listenFd) < 0) 
  {
    LogError("set socket non block failed");
    abort();
  }
}

void Server::start() {
  LogDebug("Server::start()");
    _started = true;
    std::shared_ptr<RequestData> request(new RequestData());
    request->setFd(_listenFd);
    if (Epoll::epoll_add(_listenFd, request, EPOLLIN | EPOLLET) < 0)
    {
        perror("epoll add error");
        abort();
    }

    while (_started)
    {
        //sleep(10);
        LogDebug("Server::start() Epoll::my_epoll_wait");
        Epoll::my_epoll_wait(_listenFd, MAXEVENTS, -1);

        //ThreadPool::threadpool_destroy();
        //break;
    }
}