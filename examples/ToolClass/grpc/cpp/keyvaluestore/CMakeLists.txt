# Proto file
get_filename_component(kvs_proto "../../protos/keyvaluestore.proto" ABSOLUTE)
get_filename_component(kvs_proto_path "${kvs_proto}" PATH)

# Generated sources
set(kvs_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/keyvaluestore.pb.cc")
set(kvs_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/keyvaluestore.pb.h")
set(kvs_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/keyvaluestore.grpc.pb.cc")
set(kvs_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/keyvaluestore.grpc.pb.h")
add_custom_command(
      OUTPUT "${kvs_proto_srcs}" "${kvs_proto_hdrs}" "${kvs_grpc_srcs}" "${kvs_grpc_hdrs}"
      COMMAND "${3PART_BIN_OUTPUT_PATH}/protoc"
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${kvs_proto_path}"
        --plugin=protoc-gen-grpc="${3PART_BIN_OUTPUT_PATH}/grpc_cpp_plugin"
        "${kvs_proto}"
      DEPENDS "${kvs_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}" ${GRPC_INCLUDE})

# kvs_grpc_proto
add_library(kvs_grpc_proto
  ${kvs_grpc_srcs}
  ${kvs_grpc_hdrs}
  ${kvs_proto_srcs}
  ${kvs_proto_hdrs})
target_link_libraries(kvs_grpc_proto
  ${LIB_REFLECTION}
  ${LIB_PROTOBUF}
  ${LIB_GRPC_GRPCPP}
  ${LIB_PROTOC})

# client
add_executable(client "client.cc" "caching_interceptor.h")
target_link_libraries(client
  kvs_grpc_proto
  ${LIB_REFLECTION}
  ${LIB_PROTOBUF}
  ${LIB_GRPC_GRPCPP}
  ${LIB_PROTOC}
  gpr)

# server
add_executable(server "server.cc")
target_link_libraries(server
  kvs_grpc_proto
  ${LIB_REFLECTION}
  ${LIB_PROTOBUF}
  ${LIB_GRPC_GRPCPP}
  ${LIB_PROTOC})
