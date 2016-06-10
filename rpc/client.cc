#include <stdio.h>
#include <muduo/base/Logging.h>
#include<rpc/init.h>

using namespace muduo;

int main(int argc, char* argv[])
{

    if (argc > 3)
    {
        printf("app %s , id %s, name %s\n", argv[1], argv[2], argv[3]);
        kApp = argv[0];
        kId = argv[2];
        kName = argv[3];
        kIP = argv[1];

        LOG_DEBUG << "pid = " << getpid();
        initRpcClient();
    }
    else
    {
        printf("Usage: %s host_ip\n", argv[0]);
    }
    closeRpcClient();
    printf("exit\n");
}

