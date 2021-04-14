/**
 * @file Epoll.h
 * @brief Epoll类
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_EPOLL_H__
#define __STIBEL_EPOLL_H__

#include "Timer.h"
#include <vector>
#include <sys/epoll.h>
#include <memory>

class RequestData;

const int MAXFDS = 100000;
const int MAXEVENTS = 5000;
const int LISTENQ = 1024;
const int TIMER_TIME_OUT = 500;

class Epoll
{
public:
    typedef std::shared_ptr<RequestData> SP_ReqData;
    static int epoll_init(int maxevents, int listen_num);
    static int epoll_add(int fd, SP_ReqData request, __uint32_t events);
    static int epoll_mod(int fd, SP_ReqData request, __uint32_t events);
    static int epoll_del(int fd, __uint32_t events = (EPOLLIN | EPOLLET | EPOLLONESHOT));
    static void my_epoll_wait(int listen_fd, int max_events, int timeout);
    static void acceptConnection(int listen_fd, int epoll_fd, const std::string path);
    static std::vector<SP_ReqData> getEventsRequest(int listen_fd, int events_num, const std::string path);

    static void add_timer(SP_ReqData request_data, int timeout);

private:
    static epoll_event *events;
    static SP_ReqData fd2req[MAXFDS];
    static int epoll_fd;
    static const std::string PATH;
    static TimerManager _timerManager;
};

#endif //__STIBEL_EPOLL_H__