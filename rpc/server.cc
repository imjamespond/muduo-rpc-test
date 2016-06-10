#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Singleton.h>

#include <boost/bind.hpp>

#include <rpc/impl/RpcServer.h>
#include <rpc/impl/NodeServiceImpl.hpp>
#include <rpc/utils.h>
using namespace muduo;
using namespace muduo::net;
using namespace com::metasoft::node;

int main()
{
  LOG_INFO << "pid = " << getpid();
  EventLoop loop;
  InetAddress listenAddr(7004);
  NodeServiceImpl impl;
  ::RpcServerEx server(&loop, listenAddr);
  server.registerService(&impl);
  server.start();
  loop.loop();
  google::protobuf::ShutdownProtobufLibrary();
}
