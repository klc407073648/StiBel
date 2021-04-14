/**
 * @file Timer.h
 * @brief Timer类
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_TIMER_H__
#define __STIBEL_TIMER_H__


#include "StiBel/MutexLock.h"
#include <unistd.h>
#include <memory>
#include <queue>
#include <deque>

using StiBel::MutexLock;
using StiBel::MutexLockGuard;

class RequestData;

class TimerNode
{
public:
    typedef std::shared_ptr<RequestData> SP_ReqData;
    TimerNode(SP_ReqData requestData, int timeout);
    ~TimerNode();
    void update(int timeout);
    bool isValid();
    void clearReq();
    void setDeleted() { _deleted = true; }
    bool isDeleted() const { return _deleted; }
    size_t getExpTime() const { return _expiredTime; }
private:
    size_t getCurTime();
private:
    bool _deleted;
    size_t _expiredTime;
    SP_ReqData _requestData;
};

struct TimerCmp
{
    bool operator()(std::shared_ptr<TimerNode> &a, std::shared_ptr<TimerNode> &b) const
    {
        return a->getExpTime() > b->getExpTime();
    }
};

class TimerManager
{
public:
    typedef std::shared_ptr<RequestData> SP_ReqData;
    typedef std::shared_ptr<TimerNode> SP_TimerNode;
    TimerManager();
    ~TimerManager();
    void addTimer(SP_ReqData requestData, int timeout);
    void handleExpiredEvent();
private:
    std::priority_queue<SP_TimerNode, std::deque<SP_TimerNode>, TimerCmp> _timerNodeQueue;
    MutexLock lock;
};

#endif //__STIBEL_TIMER_H__