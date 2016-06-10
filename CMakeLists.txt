cmake_minimum_required(VERSION 2.6)

PROJECT (rpc)

message("message source dir:${CMAKE_SOURCE_DIR}")
message("message binary dir:${CMAKE_BINARY_DIR}")
message("message current dir:${CMAKE_CURRENT_SOURCE_DIR}")

set (BOOST ${BOOST_ROOT})
set (HEADER ${PROJECT_SOURCE_DIR})
set (HEADER ${HEADER} ${PROJECT_BINARY_DIR})
set (HEADER ${HEADER} ${BOOST})
set (HEADER ${HEADER} ${ROOT})
set (HEADER ${HEADER} ${MUDUO}/include)
message("message header dir:${HEADER}")
include_directories (${HEADER})

set (MY_LIBS muduo_protorpc muduo_protobuf_codec muduo_protorpc_wire muduo_base muduo_net pthread rt protobuf dl)
set (LIBDIR ${MUDUO}/muduo-lib)
link_directories (${LIBDIR})

add_subdirectory(rpc)
