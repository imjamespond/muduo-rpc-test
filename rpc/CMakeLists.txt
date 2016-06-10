
add_custom_command(OUTPUT flight.pb.cc flight.pb.h
  COMMAND protoc
  ARGS --cpp_out . ${CMAKE_CURRENT_SOURCE_DIR}/flight.proto -I${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS flight.proto)
add_custom_command(OUTPUT node.pb.cc node.pb.h
  COMMAND protoc
  ARGS --cpp_out . ${CMAKE_CURRENT_SOURCE_DIR}/node.proto -I${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS node.proto)

#add_definitions(-DRPCDEBUG=1)

set (SOURCE flight.pb.cc node.pb.cc utils.cpp)

add_library(flight SHARED impl/Flight.cpp)

#add_executable (server server.cc ${SOURCE} impl/RpcServer.cpp)
#target_link_libraries (server ${MY_LIBS})

add_executable (client client.cc ${SOURCE} impl/PvpSearchServiceImpl.cpp init.cpp)
target_link_libraries (client ${MY_LIBS})

add_executable (dl_tests test/dl_tests.cc ${SOURCE} impl/PvpSearchServiceImpl.cpp init.cpp)
target_link_libraries (dl_tests ${MY_LIBS})
#add_executable (echo_client_tests test/EchoClient_unittest.cc)
#target_link_libraries (echo_client_tests muduo_base muduo_net pthread)
#add_executable (echo_server_tests test/EchoServer_unittest.cc)
#target_link_libraries (echo_server_tests muduo_base muduo_net pthread)
add_executable (timerQueue_unittest test/timerQueue_unittest.cc)
target_link_libraries (timerQueue_unittest muduo_base muduo_net pthread rt)
