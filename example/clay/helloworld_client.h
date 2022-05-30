
#pragma once
#include "helloworld.pb.h"

typedef void (*fn_cpp_invoke)(helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response);



class GreeterClient
{
public:
    GreeterClient() { invoke = 0; }
    fn_cpp_invoke invoke;

private:
    void m_invoke(const std::string& method, const google::protobuf::Message& request, google::protobuf::Message& response);

public:
  
    void SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response);
  
    void Hello(const helloworld::A& request, helloworld::B& response);
  
};


GreeterClient* GetGreeterClinet();
void SetGreeterClinetInvoke(fn_cpp_invoke invoke);





