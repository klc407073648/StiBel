# build_lib镜像

## 参考资料


## 宿主机安装以及使用

安装：
```
cd /home/klc/tools/redis/
tar -xzf redis-6.2.0.tar.gz
cd ./redis-6.2.0/
make
cd ./src/
make install
```

使用：

```
[root@VM-0-10-centos ~]# cd /home/klc/tools/redis/

[root@VM-0-10-centos redis]# mkdir -p /home/klc/tools/redis/logs/redis
[root@VM-0-10-centos redis]# redis-server ./redis-6379.conf
[root@VM-0-10-centos redis]# ps -ef |grep redis
root     25143     1  0 16:13 ?        00:00:00 redis-server *:6379
root     25179 24880  0 16:13 pts/2    00:00:00 grep --color=auto redis
```

## 需要在宿主机上关闭保护模式，且不要绑定IP。

容器里，操作方法：
cd conf
redis-server redis-6379.conf

备注：注意redis-6379.conf中  dir ../logs/redis

