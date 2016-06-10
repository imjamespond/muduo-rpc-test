#include <muduo/base/Singleton.h>
#include <rpc/impl/RpcClient.hpp>
#include "PvpSearchServiceImpl.hpp"
using namespace muduo;
using namespace muduo::net;
using namespace com::metasoft::node;

const char*
FlightImport::filename = "./libflight.so";
const char*
FlightImport::createfunc = "Create";
const char*
FlightImport::deletefunc = "Delete";

PvpSearchServiceImpl::PvpSearchServiceImpl()
{
    if(!gFlightPtr)
    {
        LOG_ERROR<<FlightImport::filename<<" has not been init!";
        exit(EXIT_FAILURE);
    }
}

void
PvpSearchServiceImpl::search(::google::protobuf::RpcController* controller,
                             const PvpSearchRequest* request,
                             PvpSearchResponse* response,
                             ::google::protobuf::Closure* done)
{
    //if init_routine  is  a  cancellation  point and is canceled,
    //the effect on once_control shall be as if pthread_once() was never called.
    //PvpSearch* pvpSearch = muduo::Singleton<ShareObject<PvpSearchImport> >::instance().getValue();
    //PvpSearch* pvpSearch = pvpSearchSO.getValue();
    PvpSearch* pvpSearch = gFlightPtr->getValue();
    if(pvpSearch)
    {
        LOG_DEBUG << "search uid:"<<request->uid();
        response->set_ok(true);
        response->set_msg("search");
        response->set_uid(pvpSearch->searchImpl(request->uid(), request->gold()));
    }
    else
    {
        LOG_DEBUG << "not search uid:"<<request->uid();
        response->set_ok(false);
    }

    done->Run();
}

void
PvpSearchServiceImpl::begin(::google::protobuf::RpcController* controller,
                            const PvpBeginRequest* request,
                            PvpResponse* response,
                            ::google::protobuf::Closure* done)
{
    LOG_DEBUG << "this function should be called by server";
    response->set_ok(true);
    response->set_msg("begin");

    done->Run();
}


