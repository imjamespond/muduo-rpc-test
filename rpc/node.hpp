#ifndef NODE_H
#define NODE_H
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <muduo/base/Logging.h>
#include <muduo/net/protorpc/RpcChannel.h>
#include <rpc/impl/PvpSearchServiceImpl.hpp>
#include <rpc/impl/RpcChannel.hpp>
class Node
{
public:
    typedef com::metasoft::node::PvpSearchService PvpSearchService;
    typedef com::metasoft::node::PvpSearchRequest PvpSearchRequest;
    typedef com::metasoft::node::PvpSearchResponse PvpSearchResponse;
    typedef std::string string;
    typedef boost::shared_ptr<muduo::net::RpcChannel> RpcChannelPtr;

    Node(string idparam,string nameparam,const RpcChannelPtr& c):
        id(idparam),
        name(nameparam),
        pvpSearchStub_(boost::get_pointer(c))
    {
        channel = c;
    }

    void search(long uid)
    {
        PvpSearchRequest request;
        request.set_uid(uid);
        PvpSearchResponse* response = new PvpSearchResponse;

        pvpSearchStub_.search(NULL, &request, response, NewCallback(this, &Node::searchCallback, response));
    }

    string id;
    string name;

    muduo::net::RpcChannelPtr channel;
protected:
private:
    void searchCallback(PvpSearchResponse* resp)
    {
        LOG_INFO << "searched\n" << resp->DebugString().c_str();
    }

    PvpSearchService::Stub pvpSearchStub_;
};

typedef boost::shared_ptr<Node> NodePtr;

class NodeManager
{
public:
    typedef std::string string;

    NodePtr getOnlineNodeById(string& id)
    {
        return nodeMap_[id];
    }

    void registNode( NodePtr& node)
    {
        // multi sign in
        NodePtr dualNode = getOnlineNodeById(node->id);
        NodePtr returnNode;
        if (dualNode&&dualNode->channel)
        {
            //if (dualNode->channel-> != node->channel) {
            LOG_INFO<<"multi sign in id:" << dualNode->id << ",channel:?" ;
            RpcChannelEx* rpcChannel = static_cast<RpcChannelEx*>(boost::get_pointer(dualNode->channel));
            rpcChannel->tcpConn->shutdown();
            //}
            returnNode=dualNode;
        }
        else
        {
            returnNode=node;
            nodeMap_[node->id] = node;
        }

        //returnNode.setLoginTime(System.currentTimeMillis());
    }

    void onSignout(NodePtr& node)
    {
        if(node)
        {
            nodeMap_.erase(node->id);
        }
    }
private:
    typedef std::map<std::string,NodePtr > node_map;
    node_map nodeMap_;
};


#endif // NODE_H
