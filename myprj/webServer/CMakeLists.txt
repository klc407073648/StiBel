#头文件目录
INCLUDE_DIRECTORIES( ${STIBEL_COMMON_INCLUDE} ${JSONCPP_INCLUDE} ${MYQSL_INCLUDE} ./include )

#目录源码路径
SET( WEBSERVER_SRC_DIR ./source )

#代码列表
FILE (GLOB WEBSERVER_FILES ${WEBSERVER_SRC_DIR}/*.cpp)

#依赖的LIB
SET(WEBSERVER_LIB  ${LIB_PTHREAD} ${LIB_STIBEL_COMMON} ${LIB_JSONCPP})

#生成 
ADD_EXECUTABLE( webServer ${WEBSERVER_FILES})

#连接生成相关库
TARGET_LINK_LIBRARIES( webServer ${WEBSERVER_LIB})

#生成 
ADD_EXECUTABLE( CGISQL.cgi ./CGImysql/sign.cpp)

#连接生成相关库
TARGET_LINK_LIBRARIES( CGISQL.cgi ${LIB_MYSQL} ${LIB_PTHREAD})

