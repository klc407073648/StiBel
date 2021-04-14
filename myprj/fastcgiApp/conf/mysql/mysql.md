# mysql安装相关

## 宿主机安装Mysql:

```
cd /home/klc/tools/mysql/
wget http://repo.mysql.com/mysql57-community-release-el7-10.noarch.rpm
rpm -Uvh mysql57-community-release-el7-10.noarch.rpm
yum install  -y  mysql-community-server
systemctl start mysqld.service
yum install mysql-devel
```

## myqsl登陆密码问题

```
1.vi  /etc/my.cnf
在[mysqld]后面任意一行添加“skip-grant-tables”用来跳过密码验证的过程，保存文档退出。

2.重启MySQL,systemctl restart mysqld.service

3.mysql 进入数据库，用以下命令修改root密码
use mysql 进入一个数据库
update user set authentication_string=password("your password") where user="root";

4. 重新编辑vi /etc/my.cnf ,去掉skip-grant-tables”，再次重启MySQL,systemctl restart mysqld.service

上述的1-4步完成了密码的修改。

5.mysql -u  root -p  进入数据库

mysql> show databases;
ERROR 1820 (HY000): You must reset your password using ALTER USER statement before executing this statement.
mysql> ALTER USER "root"@"localhost" IDENTIFIED BY "your password";
ERROR 1819 (HY000): Your password does not satisfy the current policy requirements

6.解决以上两个ERROR，使用

set global validate_password_policy=LOW;

ALTER USER "root"@"localhost" IDENTIFIED BY "your password";

成功执行后，退出数据库，重启mysql.

7. mysql -u root  -p 进入后，能正常执行命令：
```

## mysql解决容器中远程登录问题

```
[root@master01 ~]# mysql -u root -p
Enter password:

mysql> set global validate_password_policy=LOW;
Query OK, 0 rows affected (0.00 sec)

mysql> GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'yourpasswd' WITH GRANT OPTION;
Query OK, 0 rows affected, 1 warning (0.03 sec)

mysql> FLUSH   PRIVILEGES;
Query OK, 0 rows affected (0.01 sec)
```