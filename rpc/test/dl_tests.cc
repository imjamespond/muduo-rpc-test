#include <stdio.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Singleton.h>
#include <rpc/utils.h>
#include <rpc/init.h>
#include <rpc/impl/PvpSearchServiceImpl.hpp>
using namespace muduo;

int main(int argc, char* argv[])
{
    muduo::Logger::setLogLevel(muduo::Logger::DEBUG);
    pvp_timer_start();

	ShareObject<FlightImport> flight;
	flight.init();
	PvpSearch* pvp = flight.getValue();
	if(pvp)
	{
		for(long i=10;i<100;i++)
		{
		LOG_INFO<<pvp->searchImpl(i,0);
		}
	}

	muduo::net::EventLoop loop;
	loop.loop();
}

