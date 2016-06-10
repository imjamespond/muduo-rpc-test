#ifndef NODESERVICEIMPL_H
#define NODESERVICEIMPL_H
#include <stdio.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/base/Singleton.h>
#include <rpc/utils.h>
#include <rpc/node.pb.h>
#include <rpc/node.hpp>
using namespace com::metasoft::node;

namespace
{

class NodeServiceImpl : public NodeService
{
public:
    virtual void registNode(::google::protobuf::RpcController* controller,
                            const NodeRequest* request,
                            GenericResponse* response,
                            ::google::protobuf::Closure* done)
    {
        LOG_DEBUG << "called by server registNode uid:"<<request->id();//;
        response->set_ok(true);
        response->set_msg("send by server");

        muduo::net::RpcChannelPtr rpcChannelPtr = getRpcChannelPtr();
        if(rpcChannelPtr)
        {
            LOG_DEBUG << "RpcChannelPtr is available";//;

            NodePtr nodePtr(new Node(request->id(),request->name(),rpcChannelPtr));
            muduo::Singleton<NodeManager>::instance().registNode(nodePtr);
            nodePtr->search(123456);
        }

        done->Run();
    }

};
}
#endif // NODESERVICEIMPL_H
