#ToolClass
add_subdirectory(fastcgi)
add_subdirectory(jsoncpp)
add_subdirectory(log4cpp)
add_subdirectory(tinyxml)

if(HIREDIS_INCLUDE_DIR AND HIREDIS_LIBRARY)
  add_subdirectory(redis)
else()
  add_subdirectory(redis EXCLUDE_FROM_ALL)#加上EXCLUDE_FROM_ALL属性就不需要每次编译它
endif()

#if(ZeroMQ_FOUND)
if(ZEROMQ_INCLUDE_DIR AND ZEROMQ_LIBRARY)
  add_subdirectory(zeromq)
else()
  add_subdirectory(zeromq EXCLUDE_FROM_ALL)
endif()

add_subdirectory(cppunit)

add_subdirectory(yamlcpp)