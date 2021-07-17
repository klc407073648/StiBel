# StiBel项目

本项目主要用于记录C++学习过程中的各类知识，并加以总结。

涉及的内容有：

数据结构、设计模式、常用的工具类的使用(例如jsoncpp,tinyxml,poco,zeromq等)、StiBel个人库的封装以及实际小项目的学习(webServer、restful等)。

## 基础知识学习

文件位于example下各个文件夹中，记录了数据结构、设计模式、功能函数、工具类以及StiBel库的使用样例。

* [数据结构](./guide/DataStruct.md)
* [设计模式](./guide/DesignPattern.md)
* [功能函数](./guide/FunctionStudy.md)
* [工具类使用](./guide/ToolUse.md)
* [StiBel库使用](./guide/StiBel.md)


## 工程项目简介

文件位于myprj下，倾向于综合使用，例如nginx、mysql、redis、fastcgi来实现小网页前后端交互等。

* [fastcgi+nginx实践](./guide/fastcgiApp.md)
* [restful接口实现](./guide/restfulApp.md)
* [简单的webServer服务器搭建](./guide/webServer.md)

## 调试命令

```
g++ -g -o test_person test_person.cpp ./person.pb.cc -I/usr/local/protobuf/include -lprotobuf -pthread -std=c++11

export LD_LIBRARY_PATH=/usr/local/protobuf/lib
ldconfig

ln -sf /usr/local/lib64/libstdc++.so.6.0.26 /usr/lib64/libstdc++.so.6
```

```
grpc 例子编译问题：

export LD_LIBRARY_PATH=/home/build_lib/StiBel/lib/3partlib/grpc/lib:/home/build_lib/StiBel/lib/3partlib/grpc/lib64
ldconfig

ln -sf /usr/local/lib64/libstdc++.so.6.0.26 /lib64/libstdc++.so.6
直接把/lib64/libstdc++.so.6.0.19 移到其他路径下，创建上述软链接
```

```
遗留docker 中未安装mysql库
docker build -f ./Dockerfile -t docker.io/klc407073648/centos_build_lib:v2.0 .

#根据容器的状态，删除Exited状态的容器
sudo docker rm $(sudo docker ps -qf status=exited)
```