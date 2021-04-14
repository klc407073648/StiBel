# fastcgi程序

模型结构如下：
* Event中进行注册监听事件。
* 目前实现注册、登陆、上传等功能。


# 程序主逻辑

预处理过程:

程序需要依赖redis和nginx,详细见shell下redis.sh和nginx.sh
redis.sh主要是根据配置文件，启动redis服务器；
nginx.sh负责拷贝源文件到/usr/share/nginx/test下(需要修改html中IP)，更新/etc/nginx/nginx.conf

程序主要通过nginx + fcgi的方式接收网页的请求消息，由fcgi框架函数FCGI_Accept()等接收。具体处理逻辑在Event中进行注册
 mEvent.addEvent("/register", eventBind(Register::reg, mReg));

主程序运行:

./lib/3partlib/bin/spawn-fcgi  -a 127.0.0.1 -p 8050 -f ./deploy/fastcgiTEST


# FastCGI

通用网关接口(Common Gateway Interface、CGI)描述了客户端和服务器程序之间传输数据
的一种标准，可以让一个客户端，从网页浏览器向执行在网络服务器上的程序请求数据。
CGI独立于任何语言的，CGI 程序可以用任何脚本语言或者是完全独立编程语言实现，只要这个语言可以在这个系统上运行。

Nginx支持FastCGI代理，接收客户端的请求，然后将请求转发给后端FastCGI进程。
spawn-fcgi是一个通用的FastCGI进程管理器， 下述就是利用spawn-fcgi管理fastcgiTEST的例子，其中fastcgiTEST为一个针对client一个http请求的业务服务应用程序。
spawn-fcgi  -a 127.0.0.1 -p 8050 -f ./deploy/fastcgiTEST
