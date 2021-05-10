# StiBel使用

本章涉及的内容有：



export LD_LIBRARY_PATH=/home/new/build_lib/StiBel/lib/3partlib:$LD_LIBRARY_PATH
ldconfig
g++ JsonUtil.cpp main.cpp -o main -std=c++11 -I/home/new/build_lib/StiBel/include/3partlib/jsoncpp/include -L/home/new/build_lib/StiBel/lib/3partlib -ljsoncpp
./main
ln -sf /usr/local/lib64/libstdc++.so.6.0.26 /usr/lib64/libstdc++.so.6


