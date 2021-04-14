# nginx

## nginx安装

yum install nginx -y

## conf配置文件以及源文件

[root@VM-0-10-centos ~]# whereis nginx
nginx: /usr/sbin/nginx /usr/lib64/nginx /etc/nginx /usr/share/nginx /usr/share/man/man3/nginx.3pm.gz /usr/share/man/man8/nginx.8.gz

/etc/nginx  添加conf文件
/usr/share/nginx 添加源文件

## 启动nginx

[root@VM-0-10-centos ~]# nginx -c /etc/nginx/nginx.conf
[root@VM-0-10-centos ~]# nginx -s reload

