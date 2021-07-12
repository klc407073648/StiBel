# StiBel项目docker镜像构建



## docker镜像制作

所需文件的目录结构如下：

```bash
[root@VM-0-10-centos my_build_lib]# tree
.
|-- Dockerfile
|-- grpc_install_so.tar.gz
|-- StiBel_20210627.tar.gz
`-- StiBel.zip

```

构建命令：

```bash
docker build -f ./Dockerfile -t docker.io/klc407073648/centos_build_lib:v1.0 .
```

运行镜像：

```bash
docker run -it docker.io/klc407073648/centos_build_lib:v1.0 bash
```

进入容器后构建项目：

```bash
[root@c0120f944993 /]# cd /home/project/StiBel/build
[root@c0120f944993 build]# cmake ..
[root@c0120f944993 build]# make
```

测试程序是否正常运行：

```bash
[root@c0120f944993 build]# cd ../deploy/ToolClass/grpc/
[root@c0120f944993 grpc]# ./greeter_server
Server listening on 0.0.0.0:50051

另外开一个窗口：
[root@VM-0-10-centos ~]# docker exec -it c0120f944993 /home/project/StiBel/deploy/ToolClass/grpc/greeter_client
Greeter received: Hello world

grpc程序运行正常
```

遗留,推送到dockerhub:

```
docker push docker.io/klc407073648/centos_build_lib:v1.0
```



## Dockerfile内容解析

主要是以docker.io/klc407073648/centos_build_lib为基础镜像，StiBel_20210627.tar.gz，grpc_install_so.tar.gz为源库文件，StiBel.zip为工程项目文件进行构建。

```
#基础镜像
FROM docker.io/klc407073648/centos_build_lib

#创建路径
RUN mkdir -p /home/project

#拷贝源文件
ADD ./StiBel.zip  /home/project
RUN cd /home/project && unzip StiBel.zip 
ADD ./StiBel_20210627.tar.gz /home/project/StiBel
ADD ./grpc_install_so.tar.gz /home/project

#拷贝grpc
RUN mkdir -p /home/project/StiBel/lib/3partlib/grpc
RUN mkdir -p /home/project/StiBel/include/3partlib/grpc
RUN cp -rf /home/project/grpc_install_so/include    /home/project/StiBel/include/3partlib/grpc
RUN cp -rf /home/project/grpc_install_so/lib        /home/project/StiBel/lib/3partlib/grpc
RUN cp -rf /home/project/grpc_install_so/lib64      /home/project/StiBel/lib/3partlib/grpc
RUN cp -rf /home/project/grpc_install_so/bin/*      /home/project/StiBel/lib/3partlib/bin

#添加动态搜索库路径
RUN cp -rf /home/project/StiBel/conf/etc/ld.so.conf.d/mylib.conf  /etc/ld.so.conf.d
RUN chmod 777 /etc/ld.so.conf.d/mylib.conf 

#更换libstdc++.so.6链接对象
RUN rm -rf /lib64/libstdc++.so.6.0.19
RUN rm -rf /lib64/libstdc++.so.6
RUN cp -rf /usr/local/lib64/libstdc++.so.6.0.26 /lib64
RUN cd /lib64 && ln -sf ./libstdc++.so.6.0.26 ./libstdc++.so.6

#添加环境变量到 /etc/profile
RUN echo 'export CC=/usr/local/bin/gcc'    >> /etc/profile
RUN echo 'export CXX=/usr/local/bin/g++'   >> /etc/profile
RUN echo 'ldconfig'  >> /etc/profile

#source 使得环境变量生效
RUN source /etc/profile

#解决重启配置不生效（source /etc/profile）
RUN echo 'source /etc/profile'   >> ~/.bashrc
```



## docker 版本

主要命令：

```
docker run -it docker.io/klc407073648/centos_build_lib bash
容器修改内容后，exit退出
docker commit -m "centos_build_lib:v1.0" 8ab8688cf371 docker.io/klc407073648/centos_build_lib:v1.0
docker push docker.io/klc407073648/centos_build_lib:v1.0
实际上就是在原有docker.io/klc407073648/centos_build_lib加上了一层，因此push速度快。
```

操作过程：

```
[root@VM-0-10-centos ~]# docker run -it docker.io/klc407073648/centos_build_lib bash
[root@8ab8688cf371 /]#
[root@8ab8688cf371 /]# rm -rf /lib64/libstdc++.so.6.0.19
[root@8ab8688cf371 /]# rm -rf /lib64/libstdc++.so.6
[root@8ab8688cf371 /]# cp -rf /usr/local/lib64/libstdc++.so.6.0.26 /lib64
[root@8ab8688cf371 /]# cd /lib64 && ln -sf ./libstdc++.so.6.0.26 ./libstdc++.so.6
[root@8ab8688cf371 lib64]# ll /lib64/libstdc++.so.6
lrwxrwxrwx 1 root root 21 Jul 12 13:56 /lib64/libstdc++.so.6 -> ./libstdc++.so.6.0.26
[root@8ab8688cf371 lib64]# exit
exit
[root@VM-0-10-centos ~]# docker commit -m "centos_build_lib:v1.0" 8ab8688cf371 docker.io/klc407073648/centos_build_lib:v1.0
sha256:b71c89a954058c443aaebb330a922ffd17d88f0525da8df15b57b67414015135
[root@VM-0-10-centos ~]# docker images
REPOSITORY                                            TAG                 IMAGE ID            CREATED             SIZE
docker.io/klc407073648/centos_build_lib               v1.0                b71c89a95405        10 seconds ago      2.06 GB
docker.io/nginx                                       latest              d1a364dc548d        6 weeks ago         133 MB
docker.io/redis                                       latest              739b59b96069        2 months ago        105 MB
docker.io/mysql                                       5.7                 87eca374c0ed        2 months ago        447 MB
docker.io/mysql                                       latest              0627ec6901db        2 months ago        556 MB
docker.io/klc407073648/centos_build_lib               latest              25434ccfba33        3 months ago        2.04 GB
centos_build_lib                                      latest              2cf8f517a0c3        3 months ago        1.11 GB
docker.io/centos                                      centos7             8652b9f0cb4c        8 months ago        204 MB
docker.io/jenkins/jenkins                             2.222.3-centos      efa263f634d5        14 months ago       783 MB
docker.io/valueho/cppcloud                            1                   cbf3891e0d91        2 years ago         384 MB
registry.access.redhat.com/rhel7/pod-infrastructure   latest              99965fb98423        3 years ago         209 MB
[root@VM-0-10-centos ~]# docker push docker.io/klc407073648/centos_build_lib:v1.0
The push refers to a repository [docker.io/klc407073648/centos_build_lib]
9371a964e09e: Pushed
1747a895672b: Layer already exists
457ec06c63ff: Layer already exists
00f74b2ea699: Layer already exists
ece8378b7dcc: Layer already exists
cbfd491200a9: Layer already exists
889dd8eae129: Layer already exists
cf140beb20bc: Layer already exists
6f26d06e07aa: Layer already exists
7e2ca2156d8d: Layer already exists
0fd24cb55c2b: Layer already exists
174f56854903: Layer already exists
v1.0: digest: sha256:d283af88b5f0769028d7548af0bd149d211b7da4c7962a43387e984a6482aa36 size: 2851
[root@VM-0-10-centos ~]#

```

## docker常用命令

```
#清理退出的容器
docker ps -a | sed '/^CONTAINER/d' | grep "Exited" | gawk '{cmd="docker rm "$1; system(cmd)}'
```

