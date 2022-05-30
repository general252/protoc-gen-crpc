#include "helloworld_service.h"
#include "helloworld_client.h"

class myService : public GreeterServiceImpl
{
public:

    virtual void SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response);

    virtual void Hello(const helloworld::A& request, helloworld::B& response);
};



void myService::SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response)
{
    printf("SayHello 222\n");
    response.set_message("hello " + request.name());

    helloworld::A req;
    helloworld::B res;

    req.set_a("aaaaaaaaaaaaaaaaaaaaaaaaaaa");

    GetGreeterClinet()->Hello(req, res);

    printf("res %s\n", res.DebugString().data());
}

void myService::Hello(const helloworld::A& request, helloworld::B& response)
{
    printf("Hello 222\n");
}



void init()
{
    SetGreeterServiceImpl(new(myService));
}


class InitApp
{
public:
    InitApp() {
        init();
    }
}_init_app;

