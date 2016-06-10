 #include "init.h"

#include <stdio.h>
#include <sys/resource.h>
#include <string.h>
#include <muduo/base/Logging.h>
#include <muduo/base/AsyncLogging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include<rpc/impl/RpcClient.hpp>
#include<rpc/impl/PvpSearchServiceImpl.hpp>
using namespace muduo;
using namespace muduo::net;
using namespace com::metasoft::node;

char* kApp = NULL;
char* kId = NULL;
char* kName = NULL;
char* kIP = NULL;

int kRollSize = 500*1000*1000;
AsyncLogging* g_asyncLog = NULL;

EventLoopThread gEventLoopThread;
EventLoop* gEventLoopPtr = NULL;

RpcClient* gClientPtr = NULL;
FlightSO* gFlightPtr = NULL;

void initRpcClient()
{

    set_log(kApp);
    pvp_timer_start();

    FlightSO flight;
    flight.init();
    gFlightPtr = & flight;

    EventLoop loop;
    InetAddress serverAddr(kIP , 7004);

    PvpSearchServiceImpl pvp;

    RpcClient rpcClient(&loop, serverAddr);
    gClientPtr = &rpcClient;
    rpcClient.registerService(&pvp);
    rpcClient.connect();
    loop.loop();
}

void closeRpcClient()
{
  google::protobuf::ShutdownProtobufLibrary();
}

void asyncOutput(const char* msg, int len)
{
    g_asyncLog->append(msg, len);
}

void set_log(const char* appName)
{
    {
        // set max virtual memory to 2GB.
        size_t kOneGB = 1000*1024*1024;
        rlimit rl = { 2*kOneGB, 2*kOneGB };
        setrlimit(RLIMIT_AS, &rl);
    }

    printf("pid = %d\n", getpid());

    char name[256];
    strncpy(name, appName, 256);
    g_asyncLog= new AsyncLogging(::basename(name), kRollSize);
    g_asyncLog->start();
#ifdef RPCDEBUG
    Logger::setLogLevel(muduo::Logger::DEBUG);
    printf("set loglevel to debug\n");
#else
    Logger::setOutput(asyncOutput);
#endif
}

void pvp_timer_start()
{
    gEventLoopPtr = gEventLoopThread.startLoop();
}


