
#ifndef __HELLOWORLD_SERVICE_H__
#define __HELLOWORLD_SERVICE_H__

#include "helloworld.pb.h"

typedef helloworld::CRPCProtocol_ErrorCode (*fn_cpp_invoke)(helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class GreeterServiceImpl
{
public:
  
    virtual helloworld::CRPCProtocol_ErrorCode SayHello(const helloworld::HelloRequest& request,  helloworld::HelloReply& response);
  
    virtual helloworld::CRPCProtocol_ErrorCode Hello(const helloworld::A& request,  helloworld::B& response);
  

public:
  void OnInvoke(helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response, fn_cpp_invoke on_invoke);
};

GreeterServiceImpl* GetGreeterServiceImpl();
void SetGreeterServiceImpl(GreeterServiceImpl* ins);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




class GreeterClient
{
public:
    GreeterClient() { invoke = 0; }
    fn_cpp_invoke invoke;

private:
    helloworld::CRPCProtocol_ErrorCode m_invoke(const std::string& method, const google::protobuf::Message& request, google::protobuf::Message& response, helloworld::CRPCProtocol* out);

public:
  
    helloworld::CRPCProtocol_ErrorCode SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response, helloworld::CRPCProtocol* out = NULL);
  
    helloworld::CRPCProtocol_ErrorCode Hello(const helloworld::A& request, helloworld::B& response, helloworld::CRPCProtocol* out = NULL);
  
};


GreeterClient* GetGreeterClient();
void SetGreeterClientInvoke(fn_cpp_invoke invoke);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#endif //  __HELLOWORLD_SERVICE_H__


