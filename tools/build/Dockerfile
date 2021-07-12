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

#配置环境变量
ENV project_name  StiBel

#执行命令
RUN chmod 777 /home/project/StiBel/start.sh
CMD ["/bin/bash" "-c","/home/project/StiBel/start.sh"] 