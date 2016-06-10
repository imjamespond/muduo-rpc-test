#include <stdio.h>
#include <sys/resource.h>
#include <string>
#include <map>
#include <muduo/base/Logging.h>
#include <muduo/base/AsyncLogging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#include <rpc/impl/RpcClient.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "PvpSearchServiceImpl.hpp"
using namespace muduo;
using namespace muduo::net;
class UidSeq
{
public:
    explicit UidSeq(int );
    long  search(long uid);
    void beginImpl();
    void beginImplWithLock();
private:
    const static int kUidSeqSize = 4;
    long uidSeq_[kUidSeqSize];
    bool ok_;
    int type_;
    MutexLock mutex_;
};

class PvpSearchImpl : public PvpSearch
{
public:
    //PvpSearchImpl();
    virtual long searchImpl(long, int);

private:
    MutexLock mutex_;

    typedef boost::shared_ptr<UidSeq> UidSeqPtr;
    typedef std::map<int, UidSeqPtr > UidSeqMap;
    UidSeqMap usMap_;
};


long
PvpSearchImpl::searchImpl(long uid, int gold)
{
    if(gold<0)
    {
        return -1;
    }

    UidSeqPtr uidSeqPtr ;
    {
        MutexLockGuard lock(mutex_) ;
        UidSeqMap::iterator it = usMap_.find(gold);
        if(it==usMap_.end())
        {
            uidSeqPtr.reset(new UidSeq(gold));
            usMap_[gold] = uidSeqPtr;
        }else
        {
            uidSeqPtr = it->second;
        }
    }
    return uidSeqPtr->search(uid);
}

UidSeq::UidSeq(int type):
    ok_(false),
    type_(type)
{
    ::memset(uidSeq_,0,sizeof uidSeq_);
}

long  UidSeq::search(long uid)
{
    MutexLockGuard lock(mutex_) ;
    for(int i=0; i<kUidSeqSize; i++)
    {
        if(uid == uidSeq_[i])
        {
            return uid;
        }
    }
    for(int i=0; i<kUidSeqSize; i++)
    {
        if(uidSeq_[i]==0)
        {
            uidSeq_[i] = uid;
            if(i==0)
            {
                //TODO invite rpc
                ok_=true;
                //TODO timer 15 secs
                if(gEventLoopPtr)
                {
                    gEventLoopPtr->runAfter(15, boost::bind(&UidSeq::beginImplWithLock,this));
                }
                return uidSeq_[0];
            }
            else if(i==3)
            {
                //开局广播
                if(ok_)
                {
                    long uidTmp = uidSeq_[0];
                    beginImpl();
                    return uidTmp;
                }
            }
            else
            {
                return uidSeq_[0];
            }
        }
    }

    return 0;
}

void
UidSeq::beginImpl()
{
    if(gClientPtr&&ok_)
    {
        gClientPtr->begin(uidSeq_,kUidSeqSize,type_);
    }
    ok_=false;
    ::memset(uidSeq_,0,sizeof uidSeq_);
}

void
UidSeq::beginImplWithLock()
{
    LOG_DEBUG << "";
    MutexLockGuard lock(mutex_);
    beginImpl();
}


extern"C" PvpSearch* Create()
{
    return new PvpSearchImpl();
}

extern"C" void Delete(PvpSearch* ptr)
{
    delete ptr;
}
