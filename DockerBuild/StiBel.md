# StiBel项目docker镜像构建


## docker镜像制作

所需文件的目录结构如下：

```bash
[root@VM-0-10-centos my_build_lib]# tree
.
|-- Dockerfile
|-- StiBel_20210821.tar.gz
`-- start.sh

```

构建命令：

```bash
docker build -t docker.io/klc407073648/centos_build_lib:v2.0 .
```

运行镜像：

```bash
docker run -it -d -p 9950:9950 --name docker-stibel-2 docker.io/klc407073648/centos_build_lib:v2.0
```

推送到dockerhub:

```
docker push docker.io/klc407073648/centos_build_lib:v2.0
```



## Dockerfile内容解析

主要是以docker.io/klc407073648/centos_build_lib为基础镜像，StiBel_20210627.tar.gz，grpc_install_so.tar.gz为源库文件，StiBel.zip为工程项目文件进行构建。

```
#基础镜像
FROM docker.io/klc407073648/centos_build_lib:v1.0

#创建路径
RUN mkdir -p /home/project/StiBel

#拷贝源文件
ADD ./StiBel_20210821.tar.gz  /home/project/StiBel
ADD ./start.sh  /home/project/StiBel

#添加动态搜索库路径
RUN cp -rf /home/project/StiBel/conf/etc/ld.so.conf.d/mylib.conf  /etc/ld.so.conf.d
RUN chmod 777 /etc/ld.so.conf.d/mylib.conf 

#添加环境变量到 /etc/profile
RUN echo 'ldconfig'  >> /etc/profile

#source 使得环境变量生效
RUN source /etc/profile

#解决重启配置不生效（source /etc/profile）
RUN echo 'source /etc/profile'   >> ~/.bashrc

#配置环境变量
ENV project_name  StiBel

#执行命令
RUN chmod 777 /home/project/StiBel/start.sh
CMD ["/bin/bash","-c","/home/project/StiBel/start.sh"] 
```


## docker常用命令

```
#清理退出的容器
docker ps -a | sed '/^CONTAINER/d' | grep "Exited" | gawk '{cmd="docker rm "$1; system(cmd)}'
docker rm $(docker ps -q -f status=exited)
```

## StiBel的容器项目运行

```
1.打包原始文件：tar zcvf StiBel.tar.gz ./StiBel
2.生成docker镜像：docker build -f ./Dockerfile -t docker.io/klc407073648/centos_build_lib:v3.0 .
3.以默认方式(Dockerfile中的CMD命令)，运行容器。docker run -it -d docker.io/klc407073648/centos_build_lib:v3.0
```

## 遗留处理

```
1.看门狗脚本，负责把异常退出的进程重新拉起来
2.编写start.sh,在Dockerfile 的CMD里，容器拉起来的时候，编译整个项目，并启动相关进程
```

