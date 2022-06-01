
#include "helloworld_service.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static GreeterServiceImpl* insGreeter = NULL;
GreeterServiceImpl* GetGreeterServiceImpl() {
    if (NULL == insGreeter) {
        static GreeterServiceImpl mInsGreeter;
        return &mInsGreeter;
    }
    return insGreeter;
}

void SetGreeterServiceImpl(GreeterServiceImpl* ins) {
    insGreeter = ins;
}


void GreeterServiceImpl::OnInvoke(helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response, fn_cpp_invoke on_invoke)
{
    do {

    
        if (request.method() == "/helloworld.Greeter/SayHello") {

            helloworld::HelloRequest req;
            helloworld::HelloReply res;

            if (!req.ParseFromString(request.request())) {
                printf("parse fail. [%s]\n", request.method().data());
                response.set_code(helloworld::CRPCProtocol_ErrorCode_InvalidArgument);
                response.set_msg("parse protobuf fail " + request.method());
                break;
            }

            helloworld::CRPCProtocol_ErrorCode code = this->SayHello(req, res);

            response.set_response(res.SerializeAsString());
            response.set_code(code);

            break;
        }
    
        if (request.method() == "/helloworld.Greeter/Hello") {

            helloworld::A req;
            helloworld::B res;

            if (!req.ParseFromString(request.request())) {
                printf("parse fail. [%s]\n", request.method().data());
                response.set_code(helloworld::CRPCProtocol_ErrorCode_InvalidArgument);
                response.set_msg("parse protobuf fail " + request.method());
                break;
            }

            helloworld::CRPCProtocol_ErrorCode code = this->Hello(req, res);

            response.set_response(res.SerializeAsString());
            response.set_code(code);

            break;
        }
    

        response.set_code(helloworld::CRPCProtocol_ErrorCode_Unimplemented);
        response.set_msg(request.method() + " un implemented");
    }while(0);

    if (on_invoke) {
        on_invoke(request, response);
    }
}



helloworld::CRPCProtocol_ErrorCode GreeterServiceImpl::SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response) {
    printf("GreeterService::SayHello un implemented\n");
    return helloworld::CRPCProtocol_ErrorCode_Unimplemented;;
}



helloworld::CRPCProtocol_ErrorCode GreeterServiceImpl::Hello(const helloworld::A& request, helloworld::B& response) {
    printf("GreeterService::Hello un implemented\n");
    return helloworld::CRPCProtocol_ErrorCode_Unimplemented;;
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





static GreeterClient insGreeterClient;
GreeterClient* GetGreeterClinet() {
    return &insGreeterClient;
}

void SetGreeterClinetInvoke(fn_cpp_invoke invoke) {
    insGreeterClient.invoke = invoke;
}


helloworld::CRPCProtocol_ErrorCode GreeterClient::m_invoke(const std::string& method, const google::protobuf::Message& request, google::protobuf::Message& response) {
    if (!this->invoke) {
        return helloworld::CRPCProtocol_ErrorCode_NotInit;
    }

    helloworld::CRPCProtocol req, res;
    req.set_method(method);
    req.set_request(request.SerializeAsString());

    helloworld::CRPCProtocol_ErrorCode result = this->invoke(req, res);
    if (result != helloworld::CRPCProtocol_ErrorCode_OK) {
        return result;
    }

    if (!response.ParseFromString(res.response())) {
        printf("parse fail %s\n", method.data());
        return helloworld::CRPCProtocol_ErrorCode_Fail;
    }
    else {
        // printf("[%s] debug: %s\n", method.data(), response.DebugString().data());
    }
}


helloworld::CRPCProtocol_ErrorCode GreeterClient::SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response) {
    return this->m_invoke("/helloworld.Greeter/SayHello", request, response);
}

helloworld::CRPCProtocol_ErrorCode GreeterClient::Hello(const helloworld::A& request, helloworld::B& response) {
    return this->m_invoke("/helloworld.Greeter/Hello", request, response);
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

