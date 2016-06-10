#include "utils.h"
#include <muduo/base/ThreadLocal.h>
#include <muduo/net/protorpc/RpcChannel.h>
using namespace muduo::net;
using namespace muduo;
ThreadLocal<RpcChannelPtr> gRpcChannelPtr;

RpcChannelPtr
getRpcChannelPtr()
{
    return gRpcChannelPtr.value();
}

void
setRpcChannelPtr(const RpcChannelPtr& ptr)
{
    gRpcChannelPtr.value() = ptr;
}

void
resetRpcChannelPtr()
{
    gRpcChannelPtr.value().reset();
}
