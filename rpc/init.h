 #ifndef INIT_H
#define INIT_H
#include <muduo/base/Types.h>
extern char* kApp;
extern char* kId;
extern char* kName;
extern char* kIP;

void initRpcClient();
void closeRpcClient();
void set_log(const char* );
void asyncOutput(const char* , int );
void pvp_timer_start();

#endif // INIT_H
