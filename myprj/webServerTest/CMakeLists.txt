#头文件目录
INCLUDE_DIRECTORIES( ${STIBEL_COMMON_INCLUDE} ${JSONCPP_INCLUDE} ${STIBEL_JSON_INCLUDE} ${STIBEL_MYSQL_INCLUDE} ${ZEROMQ_INCLUDE} ${STIBEL_LOG_INCLUDE} ${LOG4CPP_INCLUDE} ./include ./req/include )

#目录源码路径
SET( WEBSERVER_SRC_DIR ./source )

#代码列表
FILE (GLOB WEBSERVER_FILES ${WEBSERVER_SRC_DIR}/*.cpp)

#依赖的LIB
SET(WEBSERVER_LIB  ${LIB_PTHREAD} ${LIB_STIBEL_COMMON} ${LIB_JSONCPP} ${LIB_STIBEL_MYSQL} ${LIB_STIBEL_JSON} ${LIB_STIBEL_LOG} ${LIB_STIBEL_COMMON} ${LIB_ZEROMQ})

#生成 
ADD_EXECUTABLE( webServer_main ${WEBSERVER_FILES})

#连接生成相关库
TARGET_LINK_LIBRARIES( webServer_main ${WEBSERVER_LIB})

#生成 
ADD_EXECUTABLE( rrbroker_main ./req/source/rrbroker_main.cpp ./req/source/rrBroker.cpp)

#连接生成相关库
TARGET_LINK_LIBRARIES( rrbroker_main ${WEBSERVER_LIB} )

#生成 
ADD_EXECUTABLE( rrserver_main ./req/source/rrserver_main.cpp ./req/source/rrServer.cpp)

#连接生成相关库
TARGET_LINK_LIBRARIES( rrserver_main ${WEBSERVER_LIB} )

