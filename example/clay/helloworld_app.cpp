
#include "helloworld_app.h"
#include "helloworld.pb.h"
#include "helloworld_service.h"


#ifdef  _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "lib/debug/lib/libprotobufd.lib")
#pragma comment(lib, "lib/debug/lib/libprotobuf-lited.lib")
#else
#pragma comment(lib, "lib/libprotobuf.lib")
#pragma comment(lib, "lib/libprotobuf-lite.lib")
#endif // _DEBUG
#endif


#define set_callback_on_event  "set_callback_on_event"
#define set_callback_on_server "set_callback_on_server"

typedef int32_t(*FnMethod)(char* data, int32_t length);

FnMethod on_callback = 0;
FnMethod on_server = 0;


void go_callback(const helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response)
{
    if (on_callback) {
        response.set_method(request.method());
        response.set_funhandle(request.funhandle());
        response.set_request(request.request());
        if (request.has_inner()) {
            response.mutable_inner()->set_method(response.inner().method());
        }

        std::string data = response.SerializeAsString();

        on_callback((char*)data.data(), data.size());
    }
}

void go_server(const helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response)
{

}



DYC_API int crpc_call(char* data, int32_t length)
{
    helloworld::CRPCProtocol request;
    if (!request.ParseFromArray(data, length)) {
        printf("parse CRPCProtocol fail.\n");
        return -1;
    }

    helloworld::CRPCProtocol response;

    if (request.has_inner())
    {
        const helloworld::CRPCProtocol_Inner& inner = request.inner();

        if (0 == strcmp(set_callback_on_event, inner.method().data())) {
            on_callback = (FnMethod)inner.callback();
            response.set_code(helloworld::CRPCProtocol_ErrorCode_OK);
        }
        else if (0 == strcmp(set_callback_on_server, inner.method().data())) {
            on_server = (FnMethod)inner.callback();
            response.set_code(helloworld::CRPCProtocol_ErrorCode_OK);
        }
        else {
            response.set_code(helloworld::CRPCProtocol_ErrorCode_Fail);
            response.set_msg("unknown method " + inner.method());
        }

        go_callback(request, response);
    }
    else
    {
      
        if (std::string::npos != request.method().find("/helloworld.Greeter/")) {
            GetGreeterService()->OnInvoke(request, response);
            go_callback(request, response);
            return 0;
        }
      

        response.set_code(helloworld::CRPCProtocol_ErrorCode_Unimplemented);
        response.set_msg("unknown service " + request.method());
        return 0;
    }
}


