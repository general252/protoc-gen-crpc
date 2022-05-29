
#pragma once

#include "helloworld.pb.h"



class GreeterService
{
public:
    void OnInvoke(const helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response);
  
    virtual void SayHello(const helloworld::HelloRequest& request,  helloworld::HelloReply& response);
  
    virtual void Hello(const helloworld::A& request,  helloworld::B& response);
  
};

GreeterService* GetGreeterService();
void SetGreeterService(GreeterService* ins);





