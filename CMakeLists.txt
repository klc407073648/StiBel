# 设定版本,低于2.8版本就报错
CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
# 设定工程名 和支持的编程语言(CXX代表C++)
PROJECT(STIBEL LANGUAGES CXX) 
# 设定编译器
SET(CMAKE_CXX_COMPILER "g++")

# 构建类型
if(NOT CMAKE_BUILD_TYPE)
  set(BUILD_TYPE "Debug")
else()
  set(BUILD_TYPE "Release")
endif()

# 构建版本
if(NOT CMAKE_BUILD_VERSION)
  set(LIB_VERSION "V1.1.0")
else()
  set(LIB_VERSION ${CMAKE_BUILD_VERSION})
endif()

# 选项: 构建用例 和 CTest用例
option(BUILD_EXAMPLES "Enable build all examples" OFF)
option(BUILD_CTEST    "Enable CTest" OFF)

# 设置C++编译选项
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall -g -fpermissive")

# 自定义 cmake 模块所在的路径 ———— 考虑所有的内容都通过FindYamlCpp来查找不需要自定义查找
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# 包含colors.cmake模块,调用其中定义的宏
include(colors)
define_colors()

# 获取当前编译的时间
STRING(TIMESTAMP COMPILE_TIME  "%Y-%m-%d %H:%M:%S [UTC]" UTC)

#PROJECT_BINARY_DIR：运行cmake命令的目录。
#PROJECT_SOURCE_DIR：工程根目录
MESSAGE (STATUS "STIBEL_BINARY_DIR:        " ${STIBEL_BINARY_DIR})
MESSAGE (STATUS "STIBEL_SOURCE_DIR:        " ${STIBEL_SOURCE_DIR})
MESSAGE (STATUS "BUILD_TYPE:               " ${BUILD_TYPE})
MESSAGE (STATUS "BUILD_EXAMPLES:           " ${BUILD_EXAMPLES})
MESSAGE (STATUS "BUILD_CTEST:              " ${BUILD_CTEST})
MESSAGE (STATUS "LIB_VERSION:              " ${LIB_VERSION})
MESSAGE (STATUS "COMPILE_TIME:             " ${COMPILE_TIME})
MESSAGE (STATUS "COMPILE_SYSTEM:           " ${CMAKE_SYSTEM_NAME})

#设置是否生成共享库
SET(BUILD_SHARED_LIBS ON)

# 设定可执行二进制文件和库目录
#EXECUTABLE_OUTPUT_PATH为生成可执行文件路径，LIBRARY_OUTPUT_PATH为生成库文件路径，BUILD_LIB_DIR生成库的路径
SET(EXECUTABLE_OUTPUT_PATH      ${PROJECT_SOURCE_DIR}/deploy/)
SET(BUILD_LIB_DIR               ${PROJECT_SOURCE_DIR}/deploy/lib)
SET(LIBRARY_OUTPUT_PATH         ${BUILD_LIB_DIR})
SET(3PART_BIN_OUTPUT_PATH       ${PROJECT_SOURCE_DIR}/lib/3partlib/bin)

# 为各个examples文件下目录设置输出路径
SET(DATASTRUCT_EXECUTABLE_OUTPUT_PATH       ${PROJECT_SOURCE_DIR}/deploy/DataStruct)
SET(DESIGNPATTERN_EXECUTABLE_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/deploy/DesignPattern)
SET(FUNCTIONSTUDY_EXECUTABLE_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/deploy/FunctionStudy)
SET(TOOLCLASS_EXECUTABLE_OUTPUT_PATH        ${PROJECT_SOURCE_DIR}/deploy/ToolClass)
SET(STIBEL_EXECUTABLE_OUTPUT_PATH           ${PROJECT_SOURCE_DIR}/deploy/StiBel)

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
SET(PROTOBUF_INCLUDE        ${3PART_INCLUDE}/protobuf/include)
#SET(GRPC_INCLUDE            ${3PART_INCLUDE}/grpc/include)
#SET(MYQSL_INCLUDE           ${3PART_INCLUDE}/mysql/include)

# 设置自封装应用库的头文件
SET(COMM_INCLUDE  ${PROJECT_SOURCE_DIR}/include/comlib)

SET(STIBEL_COMMON_INCLUDE   ${COMM_INCLUDE}/StiBel/Common/include)
SET(STIBEL_MYSQL_INCLUDE    ${COMM_INCLUDE}/StiBel/Data/MySQL/include ${MYQSL_INCLUDE})
SET(STIBEL_REDIS_INCLUDE    ${COMM_INCLUDE}/StiBel/Data/Redis/include)
SET(STIBEL_LOG_INCLUDE      ${COMM_INCLUDE}/StiBel/Log/include)
SET(STIBEL_JSON_INCLUDE     ${COMM_INCLUDE}/StiBel/JSON/include)

# 设置各类应用库的文件路径
#SET(3PART_DIR  ${PROJECT_SOURCE_DIR}/lib/3partlib ${PROJECT_SOURCE_DIR}/lib/3partlib/grpc/lib  ${PROJECT_SOURCE_DIR}/lib/3partlib/grpc/lib64 ${PROJECT_SOURCE_DIR}/lib/3partlib/mysql)
SET(3PART_DIR  ${PROJECT_SOURCE_DIR}/lib/3partlib)
SET(COMM_DIR   ${PROJECT_SOURCE_DIR}/lib/comlib)

# cmake 在find_path和find_library时,会搜索一些默认的路径
#g++在编译过程中，能够搜索到mysql和hiredis的头文件和库文件路径，因为
#这些头文件和库文件都放置/usr/include、/usr/local/include，/usr/local/lib/、/usr/lib64/ 下
#属于g++搜索路径下的内容，即不在需要指定MYSQL_INCLUDE、MYSQL_DIR来添加工程的路径

# 设置自定义搜索路径
set(CMAKE_INCLUDE_PATH    ${JSONCPP_INCLUDE} ${LOG4CPP_INCLUDE} ${TINYXML_INCLUDE} ${ZEROMQ_INCLUDE} ${HIREDIS_INCLUDE} ${FASTCGI_INCLUDE} ${POCO_INCLUDE} ${CPPUNIT_INCLUDE} ${YAMLCPP_INCLUDE} ${GRPC_INCLUDE} ${PROTOBUF_INCLUDE})
set(CMAKE_LIBRARY_PATH    ${3PART_DIR} ${COMM_DIR})

# 搜索第三方应用库对应的头文件和库是否存在,来判断是否安装了对应库
find_path(JSONCPP_INCLUDE_DIR json/json.h)
find_library(JSONCPP_LIBRARY NAMES jsoncpp)

find_path(LOG4CPP_INCLUDE_DIR log4cpp/Category.hh)
find_library(LOG4CPP_LIBRARY NAMES log4cpp)

find_path(TINYXML_INCLUDE_DIR tinyxml.h)
find_library(TINYXML_LIBRARY NAMES tinyxml)

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

find_path(PROTOBUF_INCLUDE_DIR google/protobuf/text_format.h)
find_library(PROTOBUF_LIBRARY NAMES protobuf)

# 打印第三方应用库的存在信息
SET(3PART_LIBS JSONCPP LOG4CPP TINYXML ZEROMQ HIREDIS FASTCGI POCO CPPUNIT YAMLCPP PROTOBUF MYSQL)

FOREACH(3PART_LIB ${3PART_LIBS})	 
  if(${3PART_LIB}_INCLUDE_DIR AND ${3PART_LIB}_LIBRARY)
    message(STATUS "found ${3PART_LIB}")
  else()
    message(STATUS "not found ${3PART_LIB}")
  endif()	 
ENDFOREACH()

# 设置第三方库的链接名称
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
SET(LIB_PROTOBUF protobuf)

SET(LIB_REFLECTION grpc++_reflection)
SET(LIB_GRPC_GRPCPP grpc++)
SET(LIB_PROTOC protoc)

# 自封装应用库是否存在
SET(STIBEL_COMMON_LIB   StiBel_common_${LIB_VERSION})
SET(STIBEL_MYSQL_LIB    StiBel_mysql_${LIB_VERSION})
SET(STIBEL_LOG_LIB      StiBel_log_${LIB_VERSION})
SET(STIBEL_JSON_LIB     StiBel_json_${LIB_VERSION})
SET(STIBEL_REDIS_LIB    StiBel_redis_${LIB_VERSION})

find_library(STIBEL_COMMON_LIBRARY  NAMES   ${STIBEL_COMMON_LIB})
find_library(STIBEL_MYSQL_LIBRARY   NAMES   ${STIBEL_MYSQL_LIB})
find_library(STIBEL_LOG_LIBRARY     NAMES   ${STIBEL_LOG_LIB})
find_library(STIBEL_JSON_LIBRARY    NAMES   ${STIBEL_JSON_LIB})
find_library(STIBEL_REDIS_LIBRARY   NAMES   ${STIBEL_REDIS_LIB})

# 校验自封装库是否存在
SET(STIBEL_LIBS COMMON MYSQL LOG JSON REDIS)

FOREACH(STIBEL_LIB ${STIBEL_LIBS})	 
  #string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
  if(STIBEL_${STIBEL_LIB}_LIBRARY)
    message(STATUS "found STIBEL_${STIBEL_LIB}_LIBRARY")
  else()
    message(STATUS "not found STIBEL_${STIBEL_LIB}_LIBRARY")
  endif()	 
ENDFOREACH()

# 设置自封装应用库的lib链接名称
SET(LIB_STIBEL_COMMON   ${STIBEL_COMMON_LIB}  ${LIB_PTHREAD})
SET(LIB_STIBEL_MYSQL    ${STIBEL_MYSQL_LIB}   ${LIB_MYSQL} )
SET(LIB_STIBEL_LOG      ${STIBEL_LOG_LIB}     ${LIB_LOG4CPP}    ${LIB_PTHREAD}    ${LIB_STIBEL_COMMON})
SET(LIB_STIBEL_JSON     ${STIBEL_JSON_LIB}    ${LIB_JSONCPP})
SET(LIB_STIBEL_REDIS    ${STIBEL_REDIS_LIB}   ${LIB_HIREDIS})

# 添加外部库的搜索路径
LINK_DIRECTORIES(${3PART_DIR} ${COMM_DIR})

# CMake 运行CTest进行测试,需要在根目录的CMakeLists.txt 文件加上 enable_testing()
if(BUILD_CTEST)
  enable_testing()
endif()

# 根据构建参数STIBEL_BUILD_RELEASE_TYPE、STIBEL_BUILD_EXAMPLES添加子路径构建内容
if(BUILD_TYPE)
    message(STATUS "build ./myprj/webServerTest")
    SUBDIRS(./myprj/webServerTest)
else()
    message(STATUS "build ./myprj")
    SUBDIRS(./myprj)
endif()

if(BUILD_EXAMPLES)
  message(STATUS "build ./examples")
  SUBDIRS(./examples)
endif()