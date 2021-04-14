# restful使用学习


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

与webServer不同点，在于其维护了一个rest_uri的map,利用restful风格进行返回

```
void analysisGetRequest();
void analysisPostRequest();
void analysisPutRequest();
void analysisDeleteRequest();
```

