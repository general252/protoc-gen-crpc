#include "helloworld_service.h"

class myService : public GreeterService
{
public:

    virtual void SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response);

    virtual void Hello(const helloworld::A& request, helloworld::B& response);
};



void myService::SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response)
{
    printf("SayHello 222\n");
    response.set_message("hello " + request.name());
}

void myService::Hello(const helloworld::A& request, helloworld::B& response)
{
    printf("Hello 222\n");
}



void init()
{
    SetGreeterService(new(myService));
}


class InitApp
{
public:
    InitApp() {
        init();
    }
}_init_app;

