# 求和测试
add_library(sum sum.cpp)
add_executable(sum_up sum_test.cpp)
target_link_libraries(sum_up sum)
add_test(
    NAME cpp_test
    COMMAND $<TARGET_FILE:sum_up>
)

# 字符串相等测试
add_executable(str_test str_test.cpp)
add_test(
    NAME str_test
    COMMAND $<TARGET_FILE:str_test>
)

# 不同的系统编译不同的代码,TODO
add_executable(system_test system_test.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_compile_definitions(system_test PUBLIC "IS_LINUX")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  target_compile_definitions(system_test PUBLIC "IS_MACOS")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_compile_definitions(system_test PUBLIC "IS_WINDOWS")
endif()

add_test(
    NAME system_test
    COMMAND $<TARGET_FILE:system_test>
)

