#ifndef PVPSEARCHSERVICEIMPL_H
#define PVPSEARCHSERVICEIMPL_H
#include <rpc/flight.pb.h>
#include <rpc/utils.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>

class RpcClient;
extern RpcClient* gClientPtr;
extern muduo::net::EventLoop* gEventLoopPtr;

class PvpSearch;
class PvpSearchImpl;

template<class T>
class ShareObject;
class FlightImport;
typedef ShareObject<FlightImport> FlightSO;
extern FlightSO* gFlightPtr;

class FlightImport
{
public:
	typedef PvpSearch value_type;
	static const char* filename;
	static const char* createfunc;
	static const char* deletefunc;
};
class PvpSearchServiceImpl : public com::metasoft::node::PvpSearchService
{
public:
    PvpSearchServiceImpl();
    virtual void search(::google::protobuf::RpcController* controller,
                        const com::metasoft::node::PvpSearchRequest* request,
                        com::metasoft::node::PvpSearchResponse* response,
                        ::google::protobuf::Closure* done);
    virtual void begin(::google::protobuf::RpcController* controller,
                       const com::metasoft::node::PvpBeginRequest* request,
                       com::metasoft::node::PvpResponse* response,
                       ::google::protobuf::Closure* done);
    //ShareObject<PvpSearchImport> pvpSearchSO;
};

class PvpSearch
{
public:
    virtual long searchImpl(long,int)=0;
};


#endif // PVPSEARCHSERVICEIMPL_H
