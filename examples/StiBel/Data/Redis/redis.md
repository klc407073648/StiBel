# hiredis函数解析

## 参考资料

* [数据存储 --- redis(三) --- hiredis(Synchronous API）](http://blog.sina.com.cn/s/blog_bd7449900101jgzw.html)
* [学习视频](https://www.bilibili.com/video/BV1c4411d7jb?p=29&spm_id_from=pageDriver)


## 函数解析

关于hiredis支持的Synchronous API，主要包括以下函数:

```c++
redisContext *redisConnect(const char *ip, int port);
void *redisCommand(redisContext *c, const char *format, ...);
void freeReplyObject(void *reply);
```

## 连接

redisConnect函数用来生成redisContext。该上下文用来存储connect状态。redisContext结构有一个整形err字段（非0值）用来保存连接的错误状态。字段errstr用来保存错误描述。当通过redisConnect连接redis结束后，可以check err字段来查看连接是否成功。

```c++
redisContext *c = redisConnect("127.0.0.1", 6379);
if (c != NULL && c->err) {
    printf("Error: %s\n", c->errstr);
    // handle error
}
//redisContext结构体存储中存储错误信息，文件描述符fd，写缓冲区，redisReader类对象指针。
typedef struct redisContext {
    int err;
    char errstr[128];
    int fd;
    int flags;
    char *obuf;
    redisReader *reader;
    ...
} redisContext;
```

redisConnect函数实现连接到一个redis server上。

```c++
redisContext *redisConnect(const char *ip, int port) {
    redisContext *c = redisContextInit();
    c->flags |= REDIS_BLOCK;  //设置连接类型是阻塞的
   redisContextConnectTcp(c,ip,port,NULL);
    return c;
}
```

* redisContextInit函数用于初始化redisContext结构体指针。
* redisContextConnectTcp函数的定义位于net.c中
    * 根据输入的ip和port绑定地址，创建TCP连接。

## 命令发送

redisCommand函数实现:
```c++
reply = redisCommand(context, "SET foo %s", value);

void *redisCommand(redisContext *c, const char *format, ...) {
    va_list ap;
    void *reply = NULL;
   va_start(ap,format);
    reply = redisvCommand(c,format,ap);
    va_end(ap);
    return reply;
}
```
1. 采用不定参数的函数实现
2. 调用redisvCommand函数来解析
    * redisvCommand调用redisvAppendCommand，后者再调用redisvFormatCommand和__redisAppendCommand
    * redisvFormatCommand(char **target, const char *format, va_list ap)，解析format字符串，根据format的内容从ap中取相应的数据
    * __redisAppendCommand(redisContext *c, char *cmd, size_t len)将格式化的命令写到输出缓冲区中。

## 返回值

通过redisCommand函数的返回值来查看是否执行成功。当有错误发生时，返回值为NULL，同时err字段被标示。一旦context返回错误，则该context不能被重用，需要新建一个新的connect。

返回类型，m_pRedisReply->type:
```
REDIS_REPLY_STATUS:
返回状态，状态字符串被放在reply->str中，状态字符串长度放在reply->len中。
REDIS_REPLY_ERROR:
返回错误，错误str可以从REDIS_REPLY_STATUS中获取
REDIS_REPLY_INTEGER:
返回整型值，该值被reply->integer接收，类型为long long
REDIS_REPLY_NIL:
返回一个nil对象，没有数据被接收。
REDIS_REPLY_STRING:
返回一个字符串，字符串被放在reply->str中，字符串长度放在reply->len中。
REDIS_REPLY_ARRAY:
多个元素被返回，元素的数量存放在reply->elements中。每个元素的返回值存放在redisReply中。
```

返回值需要被释放通过freeReplyObject()函数。
void freeReplyObject(void *reply)根据reply->type的不同值执行不同的free操作。


## 清空
断开及清空context，使用下列函数：

```c++
void redisFree(redisContext *c);

//该函数关闭socket，同时做释放操作。
//根据不同对象，执行不同的free操作。
void redisFree(redisContext *c) {
    if (c->fd > 0)
       close(c->fd);
    if (c->obuf != NULL)
       sdsfree(c->obuf);
    if (c->reader != NULL)
       redisReaderFree(c->reader);
    free(c);
}
```

## Redis.h和Redis.cpp和redisTest.cpp

使用方法:

1. 可以直接通过配置map去初始化Redis对象，然后调用cmd函数进行redis命令调用和获取返回值。
2. 通知RedisManager的方式，初始化多组对不同端口redis组的连接，需要的时候去get对象，然后使用。
redisTest里面，将6379设为主，6380设为从，分别创建主从的两个连接进行使用。
