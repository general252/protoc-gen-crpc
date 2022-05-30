
#pragma once

#include "helloworld.pb.h"



class GreeterServiceImpl
{
public:
  
    virtual void SayHello(const helloworld::HelloRequest& request,  helloworld::HelloReply& response);
  
    virtual void Hello(const helloworld::A& request,  helloworld::B& response);
  

public:
  void OnInvoke(const helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response);
};

GreeterServiceImpl* GetGreeterServiceImpl();
void SetGreeterServiceImpl(GreeterServiceImpl* ins);





