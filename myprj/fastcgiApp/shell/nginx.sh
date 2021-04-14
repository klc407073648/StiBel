#!/bin/bash
#需要拷贝html的文件到/usr/share/nginx/test下
#需要更新配置文件/etc/nginx/nginx.conf
#同时，需要更新.html中的IP地址
#安装fcgi

conf_file=/etc/nginx/nginx.conf
source_path=/usr/share/nginx
cmd_para=/usr/sbin/nginx
cnt=0

if [ $# -eq 0 ];then
    echo "please input the argument: "
    echo "  start    ----->    start the nginx"
    echo "  stop     ----->    stop  the nginx"
    echo "  reload   ----->    reload the nginx"
    exit 1
fi

if [ ! -d $source_path/test ];then
    mkdir -p $source_path/test
	cp -rf ../html/*  $source_path/test
	echo "cp -rf ../html/*  $source_path/test success"
fi

if [ -f $conf_file ] && [ ! -f ${conf_file}.bak ];then
    mv $conf_file ${conf_file}.bak
	cp -rf ../conf/nginx/conf/nginx.conf $conf_file
	echo "cp -rf ../conf/nginx/conf/nginx.conf $conf_file success"
fi

case $1 in
    start)
        cnt=`ps aux | grep $cmd_para | grep -v grep |wc -l`
        if [ $cnt -ne 0 ];then
            echo "nginx start is runing ..."
        else
            echo "nginx starting ..."
            sudo $cmd_para
            if [ $? -eq 0 ];then
                echo "nginx start success!!!"
            else
                echo "nginx start fail ..."
            fi
        fi
        ;;
    stop)
        cnt=`ps -ef | grep $cmd_para | grep -v grep |wc -l` 
        if [ $cnt -eq 0 ];then
            echo "nginx is not runing..."
        else
            echo "nginx stopping ..."
            sudo $cmd_para -s quit
            if [ $? -eq 0 ];then
                echo "nginx stop success"
            else
                echo "nginx stop fail ..."
            fi
        fi
        ;;
    reload)
		cnt=`ps -ef | grep $cmd_para | grep -v grep |wc -l` 
        if [ $cnt -eq 0 ];then
            echo "nginx is not runing, can not reload..."
        else
			echo "nginx reloading ..."
			sudo $cmd_para -s reload
			if [ $? -eq 0 ];then
				echo "nginx reload success ..."
			else
				echo "nginx reload fail ..."
			fi
        fi
        ;;
    *)
        echo "do nothing ..."
        ;;
esac

