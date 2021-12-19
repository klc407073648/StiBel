CMAKE_MINIMUM_REQUIRED(VERSION 2.8)  # 设定版本
PROJECT(myprj) # 设定工程名
SET(CMAKE_CXX_COMPILER "g++")  # 设定编译器

option(STIBEL_BUILD_EXAMPLES "Enable build all examples" ON)
option(STIBEL_BUILD_RELEASE_TYPE "Enable build release type" ON)

#设置C++编译选项
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall -g -fpermissive")

#PROJECT_BINARY_DIR：运行cmake命令的目录。
#PROJECT_SOURCE_DIR：工程根目录
MESSAGE (STATUS "This is binary dir" ${myprj_BINARY_DIR})
MESSAGE (STATUS "This is source dir" ${myprj_SOURCE_DIR})

#设置是否生成共享库
SET(BUILD_SHARED_LIBS ON)

# 设定可执行二进制文件和库目录
#EXECUTABLE_OUTPUT_PATH为生成可执行文件路径，LIBRARY_OUTPUT_PATH为生成库文件路径，BUILD_LIB_DIR生成库的路径
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/)
SET(BUILD_LIB_DIR ${PROJECT_SOURCE_DIR}/deploy/lib)
SET(LIBRARY_OUTPUT_PATH ${BUILD_LIB_DIR})
SET(3PART_BIN_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/3partlib/bin)

#为各个examples文件下目录设置输出路径
SET(DATASTRUCT_EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/DataStruct)
SET(DESIGNPATTERN_EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/DesignPattern)
SET(FUNCTIONSTUDY_EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/FunctionStudy)
SET(TOOLCLASS_EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/ToolClass)
SET(STIBEL_EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/StiBel)

# 设置第三方应用库的头文件
SET(3PART_INCLUDE  ${PROJECT_SOURCE_DIR}/include/3partlib)

SET(JSONCPP_INCLUDE         ${3PART_INCLUDE}/jsoncpp/include)
SET(LOG4CPP_INCLUDE         ${3PART_INCLUDE}/log4cpp/include)
SET(TINYXML_INCLUDE         ${3PART_INCLUDE}/tinyxml/include)
SET(ZEROMQ_INCLUDE          ${3PART_INCLUDE}/zeromq/include) 
SET(HIREDIS_INCLUDE         ${3PART_INCLUDE}/hiredis/include)
SET(FASTCGI_INCLUDE         ${3PART_INCLUDE}/fastcgi/include)
SET(POCO_INCLUDE            ${3PART_INCLUDE}/poco/include)
SET(CPPUNIT_INCLUDE         ${3PART_INCLUDE}/cppunit/include)
SET(YAMLCPP_INCLUDE         ${3PART_INCLUDE}/yamlcpp/include)
#SET(GRPC_INCLUDE            ${3PART_INCLUDE}/grpc/include)
#SET(MYQSL_INCLUDE           ${3PART_INCLUDE}/mysql/include)

# 设置自封装应用库的头文件
SET(COMM_INCLUDE  ${PROJECT_SOURCE_DIR}/include/comlib)

SET(STIBEL_COMMON_INCLUDE   ${COMM_INCLUDE}/StiBel/Common/include)
SET(STIBEL_MYSQL_INCLUDE    ${COMM_INCLUDE}/StiBel/Data/MySQL/include ${MYQSL_INCLUDE})
SET(STIBEL_REDIS_INCLUDE    ${COMM_INCLUDE}/StiBel/Data/Redis/include)
SET(STIBEL_LOG_INCLUDE      ${COMM_INCLUDE}/StiBel/Log/include)
SET(STIBEL_JSON_INCLUDE     ${COMM_INCLUDE}/StiBel/JSON/include)

# 设置各类应用库的lib文件路径
#SET(3PART_DIR  ${PROJECT_SOURCE_DIR}/lib/3partlib ${PROJECT_SOURCE_DIR}/lib/3partlib/grpc/lib  ${PROJECT_SOURCE_DIR}/lib/3partlib/grpc/lib64 ${PROJECT_SOURCE_DIR}/lib/3partlib/mysql)
SET(3PART_DIR  ${PROJECT_SOURCE_DIR}/lib/3partlib)
SET(COMM_DIR   ${PROJECT_SOURCE_DIR}/lib/comlib)

#cmake 在find_path和find_library时,会搜索一些默认的路径
#g++在编译过程中，能够搜索到mysql和hiredis的头文件和库文件路径，因为
#这些头文件和库文件都放置/usr/include、/usr/local/include，/usr/local/lib/、/usr/lib64/ 下
#属于g++搜索路径下的内容，即不在需要指定MYSQL_INCLUDE、MYSQL_DIR来添加工程的路径

#设置自定义搜索路径
set(CMAKE_INCLUDE_PATH ${JSONCPP_INCLUDE} ${LOG4CPP_INCLUDE} ${TINYXML_INCLUDE} ${ZEROMQ_INCLUDE} ${HIREDIS_INCLUDE} ${FASTCGI_INCLUDE} ${POCO_INCLUDE} ${POCO_INCLUDE} ${CPPUNIT_INCLUDE} ${YAMLCPP_INCLUDE} ${GRPC_INCLUDE})
set(CMAKE_LIBRARY_PATH ${3PART_DIR} ${COMM_DIR})

#搜索第三方应用库对应的头文件和库是否存在,来判断是否安装了对应库
find_path(JSONCPP_INCLUDE_DIR json/json.h)
find_library(JSONCPP_LIBRARY NAMES jsoncpp)

find_path(LOG4CPP_INCLUDE_DIR log4cpp/Category.hh)
find_library(LOG4CPP_LIBRARY NAMES log4cpp)

find_path(TINYXML_INCLUDE_DIR tinyxml.h)
find_library(HIREDIS_LIBRARY NAMES tinyxml)

find_path(ZEROMQ_INCLUDE_DIR zmq.hpp)
find_library(ZEROMQ_LIBRARY NAMES zmq)

find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
find_library(HIREDIS_LIBRARY NAMES hiredis)

find_path(FASTCGI_INCLUDE_DIR fcgi_stdio.h)
find_library(FASTCGI_LIBRARY NAMES fcgi)

find_path(POCO_INCLUDE_DIR Poco/RegularExpression.h)
find_library(POCO_LIBRARY NAMES PocoEncodings)

find_path(CPPUNIT_INCLUDE_DIR cppunit/TestCase.h)
find_library(CPPUNIT_LIBRARY NAMES cppunit)

find_path(YAMLCPP_INCLUDE_DIR yaml-cpp/yaml.h)
find_library(YAMLCPP_LIBRARY NAMES yaml-cpp)

find_path(MYSQL_INCLUDE_DIR mysql/mysql.h)
find_library(MYSQL_LIBRARY NAMES mysqlclient)

find_package(Protobuf)

#打印第三方应用库的存在消息
if(JSONCPP_INCLUDE_DIR AND JSONCPP_LIBRARY)
  message(STATUS "found jsoncpp")
else()
  message(STATUS "not found jsoncpp")
endif()

if(LOG4CPP_INCLUDE_DIR AND LOG4CPP_LIBRARY)
  message(STATUS "found log4cpp")
else()
  message(STATUS "not found log4cpp")
endif()

if(TINYXML_INCLUDE_DIR AND HIREDIS_LIBRARY)
  message(STATUS "found tinyxml")
else()
  message(STATUS "not found tinyxml")
endif()

#if(ZeroMQ_FOUND)
if(ZEROMQ_INCLUDE_DIR AND ZEROMQ_LIBRARY)
  message(STATUS "found zeromq")
else()
  message(STATUS "not found zeromq")
endif()

if(HIREDIS_INCLUDE_DIR AND HIREDIS_LIBRARY)
  message(STATUS "found hiredis")
else()
  message(STATUS "not found hiredis")
endif()

if(FASTCGI_INCLUDE_DIR AND FASTCGI_LIBRARY)
  message(STATUS "found fastcgi")
else()
  message(STATUS "not found fastcgi")
endif()

if(POCO_INCLUDE_DIR AND POCO_LIBRARY)
  message(STATUS "found poco")
else()
  message(STATUS "not found poco")
endif()

if(CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
  message(STATUS "found cppunit")
else()
  message(STATUS "not found cppunit")
endif()

if(YAMLCPP_INCLUDE_DIR AND YAMLCPP_LIBRARY)
  message(STATUS "found yamlcpp")
else()
  message(STATUS "not found yamlcpp")
endif()

if(PROTOBUF_FOUND)
  message(STATUS "found protobuf")
endif()

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  message(STATUS "found mysql")
else()
  message(STATUS "not found mysql")
endif()

# 自封装应用库 是否存在
find_library(STIBEL_COMMON_LIBRARY  NAMES StiBel_common)
find_library(STIBEL_MYSQL_LIBRARY   NAMES StiBel_mysql)
find_library(STIBEL_LOG_LIBRARY     NAMES StiBel_log)
find_library(STIBEL_JSON_LIBRARY    NAMES StiBel_json)
find_library(STIBEL_REDIS_LIBRARY   NAMES StiBel_redis)

if(STIBEL_COMMON_LIBRARY)
  message(STATUS "found StiBel_common")
else()
  message(STATUS "not found StiBel_common")
endif()

if(STIBEL_MYSQL_LIBRARY)
  message(STATUS "found StiBel_mysql")
else()
  message(STATUS "not found StiBel_mysql")
endif()

if(STIBEL_LOG_LIBRARY)
  message(STATUS "found StiBel_log")
else()
  message(STATUS "not found StiBel_log")
endif()

if(STIBEL_JSON_LIBRARY)
  message(STATUS "found StiBel_json")
else()
  message(STATUS "not found StiBel_json")
endif()

if(STIBEL_REDIS_LIBRARY)
  message(STATUS "found StiBel_redis")
else()
  message(STATUS "not found StiBel_redis")
endif()

# 设置第三方库的lib链接名称
SET(LIB_JSONCPP jsoncpp)
SET(LIB_LOG4CPP log4cpp)
SET(LIB_TINYXML tinyxml)
SET(LIB_ZEROMQ  zmq)
SET(LIB_HIREDIS hiredis)
SET(LIB_PTHREAD pthread)
SET(LIB_FASTCGI fcgi)
SET(LIB_POCO       
    PocoCrypto
    PocoJSON
    PocoRedis
    PocoEncodings
    PocoJWT
    PocoUtil
    PocoFoundation
    PocoNet
    PocoXML
)
SET(LIB_CPPUNIT cppunit dl)
SET(LIB_MYSQL mysqlclient)
SET(LIB_YAMLCPP  yaml-cpp)
#SET(LIB_PROTOBUF protobuf)

SET(LIB_PROTOBUF protobuf)
SET(LIB_REFLECTION grpc++_reflection)
SET(LIB_GRPC_GRPCPP grpc++)
SET(LIB_PROTOC protoc)

# 设置自封装应用库的lib链接名称
SET(LIB_STIBEL_COMMON   StiBel_common ${LIB_PTHREAD})
SET(LIB_STIBEL_MYSQL    StiBel_mysql  ${LIB_MYSQL} )
SET(LIB_STIBEL_REDIS    StiBel_redis  ${LIB_HIREDIS})
SET(LIB_STIBEL_LOG      StiBel_log    ${LIB_LOG4CPP} ${LIB_PTHREAD} ${LIB_STIBEL_COMMON})
SET(LIB_STIBEL_JSON     StiBel_json   ${LIB_JSONCPP})

#添加外部库的搜索路径
LINK_DIRECTORIES(${3PART_DIR} ${COMM_DIR})

#根据构建参数STIBEL_BUILD_RELEASE_TYPE、STIBEL_BUILD_EXAMPLES添加子路径构建内容
if(STIBEL_BUILD_RELEASE_TYPE)
    message(STATUS "build ./myprj/webServerTest")
    SUBDIRS(./myprj/webServerTest)
else()
    message(STATUS "build ./myprj")
    SUBDIRS(./myprj)
endif()

if(STIBEL_BUILD_EXAMPLES)
  message(STATUS "build ./examples")
  SUBDIRS(./examples)   
endif()