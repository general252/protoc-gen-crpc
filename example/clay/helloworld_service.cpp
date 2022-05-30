
#include "helloworld_service.h"




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


void GreeterServiceImpl::OnInvoke(const helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response)
{

    if (request.method() == "/helloworld.Greeter/SayHello") {

        helloworld::HelloRequest req;
        helloworld::HelloReply res;

        if (!req.ParseFromString(request.request())) {
            printf("parse fail. [%s]\n", request.method().data());
            return;
        }

        this->SayHello(req, res);

        response.set_response(res.SerializeAsString());
        response.set_code(helloworld::CRPCProtocol_ErrorCode_OK);

        return;
    }

    if (request.method() == "/helloworld.Greeter/Hello") {

        helloworld::A req;
        helloworld::B res;

        if (!req.ParseFromString(request.request())) {
            printf("parse fail. [%s]\n", request.method().data());
            return;
        }

        this->Hello(req, res);

        response.set_response(res.SerializeAsString());
        response.set_code(helloworld::CRPCProtocol_ErrorCode_OK);

        return;
    }


    response.set_code(helloworld::CRPCProtocol_ErrorCode_Unimplemented);
    response.set_msg(request.method() + " un implemented");
}



void GreeterServiceImpl::SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response) {
    printf("GreeterService::SayHello un implemented\n");
}



void GreeterServiceImpl::Hello(const helloworld::A& request, helloworld::B& response) {
    printf("GreeterService::Hello un implemented\n");
}






