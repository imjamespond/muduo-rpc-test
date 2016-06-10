#ifndef RPCSERVER_H
#define RPCSERVER_H


#include <muduo/net/TcpServer.h>

namespace google
{
namespace protobuf
{

class Service;

}  // namespace protobuf
}  // namespace google

class RpcServerEx
{
public:
    RpcServerEx(muduo::net::EventLoop* loop,
              const muduo::net::InetAddress& listenAddr);

    void setThreadNum(int numThreads)
    {
        server_.setThreadNum(numThreads);
    }

    void registerService(::google::protobuf::Service*);
    void start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    // void onMessage(const TcpConnectionPtr& conn,
    //                Buffer* buf,
    //                Timestamp time);

    muduo::net::TcpServer server_;
    std::map<std::string, ::google::protobuf::Service*> services_;
};


#endif // RPCSERVER_H
