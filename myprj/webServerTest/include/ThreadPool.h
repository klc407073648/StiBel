/**
 * @file ThreadPool.h
 * @brief ThreadPool类
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_THREADPOOL_H__
#define __STIBEL_THREADPOOL_H__

#include <pthread.h>
#include <functional>
#include <memory>
#include <vector>

#include "StiBel/Log/LoggerManager.h"

const int THREADPOOL_INVALID = -1;
const int THREADPOOL_LOCK_FAILURE = -2;
const int THREADPOOL_QUEUE_FULL = -3;
const int THREADPOOL_SHUTDOWN = -4;
const int THREADPOOL_THREAD_FAILURE = -5;
const int THREADPOOL_GRACEFUL = 1;

const int MAX_THREADS = 1024;
const int MAX_QUEUE = 65535;

typedef enum
{
    immediate_shutdown = 1,
    graceful_shutdown = 2
} ShutDownOption;

struct ThreadPoolTask
{
    std::function<void(std::shared_ptr<void>)> fun;
    std::shared_ptr<void> args;
};

void myHandler(std::shared_ptr<void> req);

class ThreadPool
{
private:
    static pthread_mutex_t lock;
    static pthread_cond_t notify;

    static std::vector<pthread_t> threads;
    static std::vector<ThreadPoolTask> queue;
    static int thread_count;
    static int queue_size;

    static int head;
    static int tail;// tail 指向尾节点的下一节点
    static int count;
    static int shutdown;
    static int started;

public:
    DECLARE_STATIC_LOGGERMANAGER();
    static int threadpool_create(int threadCount, int queueSize);
    static int threadpool_add(std::shared_ptr<void> args, std::function<void(std::shared_ptr<void>)> fun = myHandler);
    static int threadpool_destroy(ShutDownOption shutdownOption = graceful_shutdown);
    static int threadpool_free();
    static void *threadpool_thread(void *args);
};

#endif //__STIBEL_THREADPOOL_H__
