
#include "helloworld_client.h"



static GreeterClient insGreeterClient;
GreeterClient* GetGreeterClinet() {
    return &insGreeterClient;
}

void SetGreeterClinetInvoke(fn_cpp_invoke invoke) {
    insGreeterClient.invoke = invoke;
}


void GreeterClient::m_invoke(const std::string& method, const google::protobuf::Message& request, google::protobuf::Message& response) {
    if (!this->invoke) {
        return;
    }

    helloworld::CRPCProtocol req, res;
    req.set_method(method);
    req.set_request(request.SerializeAsString());

    this->invoke(req, res);

    if (!response.ParseFromString(res.response())) {
        printf("parse fail %s\n", method.data());
    }
    else {
        // printf("[%s] debug: %s\n", method.data(), response.DebugString().data());
    }
}


void GreeterClient::SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response) {
    this->m_invoke("/helloworld.Greeter/SayHello", request, response);
}

void GreeterClient::Hello(const helloworld::A& request, helloworld::B& response) {
    this->m_invoke("/helloworld.Greeter/Hello", request, response);
}




