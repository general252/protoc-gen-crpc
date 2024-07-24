
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


    virtual helloworld::CRPCProtocol_ErrorCode SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response) {
        response.set_message("this is msg");

        helloworld::A a;
        helloworld::B b;
        helloworld::CRPCProtocol out;
        a.set_a("this is a");

        auto r = GetGreeterClient()->Hello(a, b, &out);
        printf("r: %d, [%s] %s\n", r, out.msg().data(), b.b().data());

        return helloworld::CRPCProtocol_ErrorCode_OK;;
    }

    virtual helloworld::CRPCProtocol_ErrorCode Hello(const helloworld::A& request, helloworld::B& response) {
        return helloworld::CRPCProtocol_ErrorCode_OK;;
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




