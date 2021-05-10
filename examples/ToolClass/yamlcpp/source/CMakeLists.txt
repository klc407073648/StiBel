cmake_minimum_required(VERSION 2.8)
project(yaml_test)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -ggdb -std=c++11")

include_directories(/home/new/build_lib/StiBel/include/3partlib/yamlcpp/include)

#添加外部库的搜索路径
LINK_DIRECTORIES(/home/new/build_lib/StiBel/lib/3partlib)

#查找库yaml-cpp，将结果存在变量YAMLCPP
#find_library(YAMLCPP yaml-cpp)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_executable(yaml_test /home/new/build_lib/StiBel/examples/ToolClass/yamlcpp/source/main.cpp)

target_link_libraries(yaml_test yaml-cpp)