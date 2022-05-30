
#include "app.h"
#include "helloworld.pb.h"
#include "helloworld_service.h"
#include "helloworld_client.h"


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
#define on_service_data        "on_service_data"

typedef int32_t(*FnMethod)(char* data, int32_t length);

FnMethod on_callback = 0;
FnMethod on_server = 0;


void cpp_on_invoke(const helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response)
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



//
struct JsonTag {
    std::string body;
};

typedef void (*fn_cpp_invoke_callback)(void* arg, const char* payload, int32_t payloadLen);
void cpp_invoke_callback(void* arg, const char* payload, int32_t payloadLen) {
    JsonTag* tag = static_cast<JsonTag*>(arg);

    if (payload) {
        tag->body.assign(payload, payloadLen);
    }
}


void cpp_invoke(helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response)
{
    if (!on_server) {
        return;
    }

    uintptr_t handle = (uintptr_t)cpp_invoke_callback;

    JsonTag* tag = new JsonTag();
    tag->body.assign("hello");

    {
        helloworld::CRPCProtocol_Inner* inner = request.mutable_inner();
        inner->set_callback(handle);
        inner->set_method(on_service_data);
        inner->set_callbackargs((uint64_t)tag);

        std::string data = request.SerializeAsString();

        on_server((char*)data.data(), data.size());

        if (!response.ParseFromString(tag->body)) {
            printf("parse app response fail.\n");
        }
        else {
            // printf("C recv: debug string: %s\n", response.DebugString().data());
        }
    }

    delete tag;
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
            if (on_server) {
              
                SetGreeterClinetInvoke(cpp_invoke);
              
            }

            response.set_code(helloworld::CRPCProtocol_ErrorCode_OK);
        }
        else if (0 == strcmp(on_service_data, inner.method().data())) {
            // 收到上层的回复
            fn_cpp_invoke_callback fn = (fn_cpp_invoke_callback)inner.callback();
            if (fn) {
                fn((void*)inner.callbackargs(), data, length);
            }
            return 0;
        } else {
            response.set_code(helloworld::CRPCProtocol_ErrorCode_Fail);
            response.set_msg("unknown method " + inner.method());
        }

        cpp_on_invoke(request, response);
    }
    else
    {
      
        if (std::string::npos != request.method().find("/helloworld.Greeter/")) {
            GetGreeterServiceImpl()->OnInvoke(request, response);
            cpp_on_invoke(request, response);
            return 0;
        }
      

        response.set_code(helloworld::CRPCProtocol_ErrorCode_Unimplemented);
        response.set_msg("unknown service " + request.method());
        return 0;
    }
}


