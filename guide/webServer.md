# 版本历史

模型结构如下：

* 使用了epoll边沿触发+EPOLLONESHOT+非阻塞IO。
* 使用了一个固定线程数的线程池，且在其成员变量中维护了一个任务队列，由条件变量触发通知新任务的到来。
* 实现了一个小根堆的定时器及时剔除超时请求，使用了STL的优先队列来管理定时器。
* 解析了HTTP的get、post请求，支持长短连接（其他请求定义了未实现）
* 线程的工作分配为：
    * 主线程负责等待epoll中的事件，并把到来的事件放进任务队列，在每次循环的结束剔除超时请求和被置为删除的时间结点
    * 工作线程阻塞在条件变量的等待中，新任务到来后，某一工作线程会被唤醒，执行具体的IO操作和计算任务，如果需要继续监听，会添加到epoll中  

* 锁的使用有两处：
    * 第一处是任务队列的添加和取操作，都需要加锁，并配合条件变量，跨越了多个线程。
    * 第二处是定时器结点的添加和删除，需要加锁，主线程和工作线程都要操作定时器队列。

# 程序主逻辑

./webServer -t 4 -p 9950 -q 65535

1.通过t:p:q读取Server的参数线程数，端口号以及任务队列数；
2.初始化Server对象，完成epoll对象（epoll_fd）初始化，线程池创建，以port创建监听fd,并设置监听fd为epoll边沿触发+EPOLLONESHOT+非阻塞IO；
3.调用Server.start函数，首先创建RequestData的智能指针并完成初始化，设置其fd为监听fd，将其加入epoll_fd所需要监听的描述符中
 EPOLLIN ：表示对应的文件描述符可以读，EPOLLET： 将EPOLL设为边缘触发。
4.即第三步已经完成主线程负责等待epoll中的事件的工作。
5.Server::start的最后调用Epoll::my_epoll_wait，即收到_listenFd的读消息，即可返回处理。
6.处理细节Epoll::getEventsRequest中acceptConnection函数，会将listen_fd对应的accept_fd？？？
应该是建立的http连接中，客户端accept_fd向服务端发送消息的时候，会触发加入req_data加入任务队列

# 使用了epoll边沿触发+EPOLLONESHOT+非阻塞IO

epoll有两种触发的方式即`LT（水平触发）和ET（边缘触发）`两种，在前者，只要存在着事件就会不断的触发，直到处理完成，而后者只触发一次相同事件或者说只在从非触发到触发两个状态转换的时候儿才触发。
这会出现下面一种情况，如果是多线程在处理，一个SOCKET事件到来，数据开始解析，这时候这个SOCKET又来了同样一个这样的事件，而你的数据解析尚未完成，那么程序会自动调度另外一个线程或者进程来处理新的事件。这造成一个很严重的问题：`不同的线程或者进程在处理同一个SOCKET的事件`，这会使程序的健壮性大降低而编程的复杂度大大增加！！即使在ET模式下也有可能出现这种情况！
本文要提到的EPOLLONESHOT这种方法，可以在epoll上注册这个事件，注册这个事件后，如果在处理写成当前的SOCKET后不再重新注册相关事件，那么这个事件就不再响应了或者说触发了。要想重新注册事件则需要调用epoll_ctl重置文件描述符上的事件，这样前面的socket就不会出现竞态这样就可以通过手动的方式来保证同一SOCKET只能被一个线程处理，不会跨越多个线程。

```c++
Epoll::acceptConnection:
// 文件描述符可以读，边缘触发(Edge Triggered)模式，保证一个socket连接在任一时刻只被一个线程处理
        __uint32_t _epo_event = EPOLLIN | EPOLLET | EPOLLONESHOT;
        Epoll::epoll_add(accept_fd, req_info, _epo_event);

setSocketNonBlocking

1、阻塞IO模型
　　最传统的一种IO模型，即在读写数据过程中会发生阻塞现象。
　　当用户线程发出IO请求之后，内核会去查看数据是否就绪，如果没有就绪就会等待数据就绪，而用户线程就会处于阻塞状态，用户线程交出CPU。当数据就绪之后，内核会将数据拷贝到用户线程，并返回结果给用户线程，用户线程才解除block状态。
    典型的阻塞IO模型的例子为：
    data = socket.read(); 
    如果数据没有就绪，就会一直阻塞在read方法。

2、非阻塞IO模型
    当用户线程发起一个read操作后，并不需要等待，而是马上就得到了一个结果。如果结果是一个error时，它就知道数据还没有准备好，于是它可以再次发送read操作。一旦内核中的数据准备好了，并且又再次收到了用户线程的请求，那么它马上就将数据拷贝到了用户线程，然后返回。
    所以事实上，在非阻塞IO模型中，用户线程需要不断地询问内核数据是否就绪，也就说非阻塞IO不会交出CPU，而会一直占用CPU。
    典型的非阻塞IO模型一般如下：
     while(true){ 
            data = socket.read(); 
            if(data!= error){ 
                处理数据 
                break; 
            } 
        } 
```

# 使用了一个固定线程数的线程池，且在其成员变量中维护了一个任务队列，由条件变量触发通知新任务的到来

在Server类中调用ThreadPool::threadpool_create完成线程池的创建(规定了工作线程及任务队列数量)；
且在Server::start()中循环检测是否有活跃事件Epoll::my_epoll_wait，如果有则调用ThreadPool::threadpool_add,将其加入任务队列，且通过pthread_cond_signal唤醒工作线程，去处理该任务。

# 实现了一个小根堆的定时器及时剔除超时请求，使用了STL的优先队列来管理定时器

在活跃事件到来时，得到对应accept的accept_fd,设置其属性为epoll边沿触+EPOLLONESHOT+非阻塞IO。同时，加入到fd2req的映射数组中，使用addTimer方法将带有时间信息的RequestData数据TimerNode(RequestData, timeout)加入到_timerNodeQueue。然后，将RequestData通过ThreadPool::threadpool_add分配给工作线程进行处理。最后_timerManager.handleExpiredEvent()不断处理_timerNodeQueue里的内容。

getEventsRequest——>acceptConnection

static SP_ReqData fd2req[MAXFDS];
static TimerManager _timerManager;

# 线程的工作分配

* 主线程负责等待epoll中的事件，并把到来的事件放进任务队列，在每次循环的结束剔除超时请求和被置为删除的时间结点
Epoll::my_epoll_wait——>getEventsRequest——>acceptConnection——>ThreadPool::threadpool_add——>handleExpiredEvent

* 工作线程阻塞在条件变量的等待中，新任务到来后，某一工作线程会被唤醒，执行具体的IO操作和计算任务，如果需要继续监听，会添加到epoll中。
ThreadPool::threadpool_add——>pthread_cond_signal(&notify)

# 锁的使用有两处

* 第一处是任务队列的添加和取操作，都需要加锁，并配合条件变量，跨越了多个线程。

* 第二处是定时器结点的添加和删除，需要加锁，主线程和工作线程都要操作定时器队列。

# 智能指针的使用

当主线程，监听到活跃事件得到

C++11标准库提供了三种智能指针(smart pointer)类型来管理动态对象，包含在头文件<memory>中，shared_ptr、weak_ptr和unique_ptr（auto_ptr或已废弃）。

shared_ptr控制对象的生命期。允许多个指针指向同一个对象,每使用它一次，引用计数+1，即强引用；
weak_ptr不控制对象的生命期。它也是一个引用计数型智能指针，但是它不增加对象的引用计数，即弱引用。但是它知道对象是否还活着。如果对象还活着，那么它可以提升为有效的shared_ptr；如果对象已经死了，提升会失败，返回一个空的shared_ptr。即 weak_ptr只是提供了对管理对象的一个访问手段；

# RequestData中linkTimer 和 seperateTimer

RequestData和TimerNode进行绑定是为了便于管理超时的请求。
因为RequestData中timer的类型为std::weak_ptr<TimerNode> 。
使用时，需要通过lock函数来进行提升为shared_ptr，使用完后timer.reset()来减少引用次数。

void linkTimer(std::shared_ptr<TimerNode> mtimer){
    timer = mtimer;
}

void RequestData::seperateTimer()
{
    cout << "seperateTimer" << endl;
    if (timer.lock())
    {
        shared_ptr<TimerNode> my_timer(timer.lock());
        my_timer->clearReq();
        timer.reset();
    }
}


# 实现不可拷贝的类
```
class MutexLockGuard
{
public:
    explicit MutexLockGuard();
    ~MutexLockGuard();

private:
    static pthread_mutex_t lock;

private:
    MutexLockGuard(const MutexLockGuard&);//拷贝构造函数
    MutexLockGuard& operator=(const MutexLockGuard&);//赋值构造函数
};

优化成：
class MutexLockGuard: Noncopyable
{...}

通过将拷贝构造函数和赋值函数声明为私有化，使得其不能被调用。

拷贝构造函数的参数一定是引用，不能不是引用，不然会出现无限递归

赋值构造函数必须注意它的函数原型，参数必须是引用类型，
返回值也必须是引用类型，否则在传参和返回的时候都会再次调用一次拷贝构造函数

上述思想类似：
lock_guard对象只是简化了mutex对象的上锁和解锁操作，方便线程对互斥量上锁，
即在某个lock_guard对象的生命周期内，它所管理的锁对象会一直保持上锁状态；
而lock_guard的生命周期结束之后，它所管理的锁对象会被解锁。
```

[epoll的作用和原理介绍](https://blog.csdn.net/weixin_34071713/article/details/92373075)
