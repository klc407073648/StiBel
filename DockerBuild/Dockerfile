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