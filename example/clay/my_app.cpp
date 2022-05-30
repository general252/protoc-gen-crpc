
#include "helloworld_service.h"


#ifdef  _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "lib/debug/lib/libprotobufd.lib")
#pragma comment(lib, "lib/debug/lib/libprotobuf-lited.lib")
#else
#pragma comment(lib, "lib/libprotobuf.lib")
#pragma comment(lib, "lib/libprotobuf-lite.lib")
#endif // _DEBUG
#endif



class myGreeterServiceImpl : public GreeterServiceImpl
{
public:

  
    virtual void SayHello(const helloworld::HelloRequest& request,  helloworld::HelloReply& response) {

    }
  
    virtual void Hello(const helloworld::A& request,  helloworld::B& response) {

    }
  

};





class InitApp
{
public:
    InitApp() {
    
        static myGreeterServiceImpl insGreeterServiceImpl;
        SetGreeterServiceImpl(&insGreeterServiceImpl);
    
    }
}_init_app;




