#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/protorpc/RpcChannel.h>

#include <boost/bind.hpp>
#include <map>

#include <rpc/flight.pb.h>
#include <rpc/node.pb.h>
#include <rpc/impl/PvpSearchServiceImpl.hpp>
#include <rpc/impl/NodeServiceImpl.hpp>
#include <rpc/impl/RpcChannel.hpp>
extern char* kId;
extern char* kName;


class GenericResponseEx : public com::metasoft::node::GenericResponse
{
public:
    virtual ~GenericResponseEx()
    {
        LOG_DEBUG<<"";
    }
};
class PvpResponseEx : public com::metasoft::node::PvpResponse
{
public:
    virtual ~PvpResponseEx()
    {
        LOG_DEBUG<<"";
    }
};
class RpcClient : boost::noncopyable
{
public:

    typedef boost::shared_ptr<muduo::net::TcpConnection> TcpConnectionPtr;
    typedef com::metasoft::node::GenericResponse GenericResponse;
    typedef com::metasoft::node::NodeRequest NodeRequest;
    typedef com::metasoft::node::PvpBeginRequest PvpBeginRequest;
    typedef com::metasoft::node::PvpResponse PvpResponse;

    RpcClient(muduo::net::EventLoop* loop, const muduo::net::InetAddress& serverAddr)
        : loop_(loop),
        client_(loop, serverAddr, "RpcClient"),
        channel_(static_cast<muduo::net::RpcChannel*>(new RpcChannelEx)),
        nodeStub_(get_pointer(channel_)),
        pvpStub_(get_pointer(channel_))
    {
        client_.setConnectionCallback(
            boost::bind(&RpcClient::onConnection, this, _1));
        client_.setMessageCallback(
            boost::bind(&RpcChannelEx::onMessage,  static_cast<RpcChannelEx*>(get_pointer(channel_)), _1, _2, _3));
        client_.enableRetry();
    }

    ~RpcClient()
    {
        LOG_DEBUG<<"";
    }

    void connect()
    {
        client_.connect();
    }

    void registerService(google::protobuf::Service* service)
    {
        const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
        services_[desc->name()] = service;
    }

    void begin(long* uidSeq,int uidLen,int gold)
    {
        LOG_DEBUG<<"";
        PvpBeginRequest request;
        for(int i=0; i<uidLen; i++)
        {
            request.add_uidseq(uidSeq[i]);
            LOG_INFO<<uidSeq[i];
        }
        request.set_gold(gold);
        PvpResponse* response = static_cast<PvpResponse*>(new PvpResponseEx);
        pvpStub_.begin(NULL, &request, response, NewCallback(this, &RpcClient::beginCallback, response));
    }

    void beginCallback(PvpResponse* resp)
    {
        LOG_DEBUG << "begin ok\n" << resp->DebugString().c_str();
    }

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn)
    {
        if (conn->connected())
        {
            //channel_.reset(new RpcChannel(conn));
            channel_->setConnection(conn);
            channel_->setServices(&services_);//crusual
            conn->setContext(channel_);

            if(kId && kName)
            {
                NodeRequest req;
                req.set_id(kId);
                req.set_type(1);
                req.set_name(kName);
                GenericResponse* resp = static_cast<GenericResponse*>(new GenericResponseEx);
                nodeStub_.registNode(NULL, &req, resp, google::protobuf::NewCallback(this, &RpcClient::registCallback, resp));
            }
        }
        else
        {
            //loop_->quit();
            LOG_DEBUG<<"connection lost";
        }
    }

    void registCallback(GenericResponse* resp)
    {
        LOG_DEBUG << "regist ok:\n" << resp->DebugString().c_str();
        //client_.disconnect();
    }

    muduo::net::EventLoop* loop_;
    muduo::net::TcpClient client_;
    muduo::net::RpcChannelPtr channel_;
    NodeService::Stub nodeStub_;
    PvpSearchService::Stub pvpStub_;

    typedef std::map<std::string, ::google::protobuf::Service*> service_map;
    service_map services_;
};
#endif // RPCCLIENT_H

