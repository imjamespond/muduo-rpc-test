#ifndef RPC_RPCCHANNEL_H
#define RPC_RPCCHANNEL_H

#include <boost/any.hpp>
#include <muduo/net/protorpc/RpcChannel.h>
#include <muduo/base/Logging.h>
#include <rpc/utils.h>
namespace
{
class RpcChannelEx : public muduo::net::RpcChannel
{
public:
    RpcChannelEx(const muduo::net::TcpConnectionPtr& conn):
    RpcChannel(conn),
    tcpConn(conn) {}
    RpcChannelEx() {}

    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp receiveTime)
    {
        LOG_DEBUG<<"RpcChannelEx onMessage";
        setRpcChannelPtr(boost::any_cast<const muduo::net::RpcChannelPtr& >(conn->getContext()));
        muduo::net::RpcChannel::onMessage(conn, buf, receiveTime);
        resetRpcChannelPtr();
    }

    muduo::net::TcpConnectionPtr tcpConn;
};
}

#endif  // RPC_RPCCHANNEL_H
