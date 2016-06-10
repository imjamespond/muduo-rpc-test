#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
namespace boost
{
template <class T>
class shared_ptr;
}

namespace muduo
{
namespace net
{
class RpcChannel;
typedef boost::shared_ptr<RpcChannel> RpcChannelPtr;
}
}

muduo::net::RpcChannelPtr getRpcChannelPtr();
void setRpcChannelPtr(const muduo::net::RpcChannelPtr& );
void resetRpcChannelPtr();

template <class SO>
class ShareObject
{
public:
    typedef typename SO::value_type* value_type_ptr;
    typedef typename SO::value_type* (*create_func)();
    typedef void (*delete_func)(typename SO::value_type*);

    ShareObject():
    handle_(NULL),
    value_(NULL)
    {

    }

    void init()
    {
        handle_ = dlopen(SO::filename, RTLD_LAZY);
        if (!handle_)
        {
            fprintf(stderr, "%s\n", dlerror());
            exit(EXIT_FAILURE);
        }

        dlerror();    /* Clear any existing error */
        funcCreate_ = reinterpret_cast<create_func>(dlsym(handle_, SO::createfunc));
        funcDelete_ = reinterpret_cast<delete_func>(dlsym(handle_, SO::deletefunc));

        if(funcCreate_)
        {
            value_ = funcCreate_();
        }

        char *error = dlerror();
        if (error != NULL)
        {
            fprintf(stderr, "%s\n", error);
            exit(EXIT_FAILURE);
        }
    }

    ~ShareObject()
    {
        printf("ShareObject dtor\n");
        if(handle_)
        {
            printf("ShareObject dlclose\n");
            dlclose(handle_);
        }
        if(funcDelete_)
        {
            printf("ShareObject funcDelete\n");
            funcDelete_(value_);
        }
    }

    create_func getCreateFunc(){return funcCreate_;}
    delete_func getDeleteFunc(){return funcDelete_;}
    value_type_ptr getValue(){return value_;}

    void* getFunctor(const char*  name)
    {
        if(!handle_)
        {
            return NULL;
        }

        void* functor = dlsym(handle_, name);
        char *error = dlerror();
        if (error)
        {
            fprintf(stderr, "%s\n", error);
            exit(EXIT_FAILURE);
        }

        return functor;
    }
private:
    void *handle_;
    create_func funcCreate_;
    delete_func funcDelete_;
    value_type_ptr  value_;
};

#endif // UTILS_H_INCLUDED

