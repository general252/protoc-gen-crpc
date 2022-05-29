
#include "helloworld_service.h"




static GreeterService* insGreeter = NULL;
GreeterService* GetGreeterService() {
    if (NULL == insGreeter) {
        static GreeterService mInsGreeter;
        return &mInsGreeter;
    }
    return insGreeter;
}

void SetGreeterService(GreeterService* ins) {
    insGreeter = ins;
}


void GreeterService::OnInvoke(const helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response)
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



void GreeterService::SayHello(const helloworld::HelloRequest& request, helloworld::HelloReply& response) {
    printf("GreeterService::SayHello un implemented\n");
}



void GreeterService::Hello(const helloworld::A& request, helloworld::B& response) {
    printf("GreeterService::Hello un implemented\n");
}






