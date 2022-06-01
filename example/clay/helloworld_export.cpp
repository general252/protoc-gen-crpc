

#ifdef _MSC_VER
#define HELLOWORLD_EXPORTS
#endif // _MSC_VER

#include "helloworld_export.h"
#include "helloworld.pb.h"
#include "helloworld_service.h"


#define set_callback_on_event  "set_callback_on_event"
#define set_callback_on_server "set_callback_on_server"
#define on_service_data        "on_service_data"

typedef int32_t (*FnMethod)(char* data, int32_t length);

FnMethod on_callback = 0;
FnMethod on_server = 0;


helloworld::CRPCProtocol_ErrorCode cpp_on_invoke(helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response)
{
    if (on_callback) {
        response.set_method(request.method());
        response.set_funhandle(request.funhandle());
        response.set_request(request.request());
        if (request.has_inner()) {
            response.mutable_inner()->set_method(response.inner().method());
        }

        std::string data = response.SerializeAsString();

        int32_t rc = on_callback((char*)data.data(), data.size());
        return (helloworld::CRPCProtocol_ErrorCode)rc;
    }

    return helloworld::CRPCProtocol_ErrorCode_OK;
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


helloworld::CRPCProtocol_ErrorCode cpp_invoke(helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response)
{
    if (!on_server) {
        return helloworld::CRPCProtocol_ErrorCode_NotInit;
    }

    helloworld::CRPCProtocol_ErrorCode result = helloworld::CRPCProtocol_ErrorCode_Fail;
    uintptr_t handle = (uintptr_t)cpp_invoke_callback;

    JsonTag* tag = new JsonTag();
    tag->body.assign("hello");

    do {
        helloworld::CRPCProtocol_Inner* inner = request.mutable_inner();
        inner->set_callback(handle);
        inner->set_method(on_service_data);
        inner->set_callbackargs((uint64_t)tag);

        std::string data = request.SerializeAsString();

        int32_t rc = on_server((char*)data.data(), data.size());
        if ((helloworld::CRPCProtocol_ErrorCode)rc != helloworld::CRPCProtocol_ErrorCode_OK) {
            result = (helloworld::CRPCProtocol_ErrorCode)rc;
            break;
        }

        if (!response.ParseFromString(tag->body)) {
            printf("parse app response fail.\n");
            result = helloworld::CRPCProtocol_ErrorCode_Fail;
            break;
        }
        else {
            // printf("C recv: debug string: %s\n", response.DebugString().data());
        }

        result = helloworld::CRPCProtocol_ErrorCode_OK;
    } while(0);

    delete tag;

    return result;
}


HELLOWORLD_API int crpc_call(char* data, int32_t length)
{
    helloworld::CRPCProtocol request;
    if (!request.ParseFromArray(data, length)) {
        printf("parse CRPCProtocol fail.\n");
        return helloworld::CRPCProtocol_ErrorCode_InvalidArgument;
    }

    helloworld::CRPCProtocol response;

    if (request.has_inner())
    {
        const helloworld::CRPCProtocol_Inner& inner = request.inner();

        if (0 == strcmp(set_callback_on_event, inner.method().data())) {
           // 回复上层
            on_callback = (FnMethod)inner.callback();
            response.set_code(helloworld::CRPCProtocol_ErrorCode_OK);
        }
        else if (0 == strcmp(set_callback_on_server, inner.method().data())) {
           // 调用上层的服务
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
            return helloworld::CRPCProtocol_ErrorCode_OK;;
        } else {
            response.set_code(helloworld::CRPCProtocol_ErrorCode_Fail);
            response.set_msg("unknown method " + inner.method());
        }

        return cpp_on_invoke(request, response);
    }
    else
    {
      
        if (std::string::npos != request.method().find("/helloworld.Greeter/")) {
            GetGreeterServiceImpl()->OnInvoke(request, response, cpp_on_invoke);
            return helloworld::CRPCProtocol_ErrorCode_OK;;
        }
      

        response.set_code(helloworld::CRPCProtocol_ErrorCode_Unimplemented);
        response.set_msg("unknown service " + request.method());
        return helloworld::CRPCProtocol_ErrorCode_OK;;
    }
}


