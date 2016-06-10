#include "RpcServer.h"
#include "RpcChannel.hpp"

#include <muduo/base/Logging.h>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

RpcServerEx::RpcServerEx(EventLoop* loop,
                     const InetAddress& listenAddr)
    : server_(loop, listenAddr, "RpcServerEx")
{
    server_.setConnectionCallback(
        boost::bind(&RpcServerEx::onConnection, this, _1));
//   server_.setMessageCallback(
//       boost::bind(&RpcServer::onMessage, this, _1, _2, _3));
}

void RpcServerEx::registerService(google::protobuf::Service* service)
{
    const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
    services_[desc->name()] = service;
}

void RpcServerEx::start()
{
    server_.start();
}

void RpcServerEx::onConnection(const TcpConnectionPtr& conn)
{
    LOG_INFO << "RpcServer - " << conn->peerAddress().toIpPort() << " -> "
    << conn->localAddress().toIpPort() << " is "
    << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
        RpcChannelEx* rpcChannelExPtr = new RpcChannelEx(conn);
        RpcChannelPtr channel(static_cast<RpcChannel*>(rpcChannelExPtr));
        channel->setServices(&services_);
        conn->setMessageCallback(
            boost::bind(&RpcChannelEx::onMessage, rpcChannelExPtr, _1, _2, _3));
        conn->setContext(channel);
    }
    else
    {
        conn->setContext(RpcChannelPtr());
        // FIXME:
    }
}
