
#ifdef _MSC_VER
#define {{.ExportHeader}}_EXPORTS
#endif // _MSC_VER

#include "{{.GeneratedFilenamePrefix}}_export.h"
#include "{{.GeneratedFilenamePrefix}}.pb.h"
#include "{{.GeneratedFilenamePrefix}}_service.h"


#define set_callback_on_event  "set_callback_on_event"
#define set_callback_on_server "set_callback_on_server"
#define on_service_data        "on_service_data"

typedef int32_t (*FnMethod)(char* data, int32_t length);

FnMethod on_callback = 0;
FnMethod on_server = 0;


{{.PacketName}}::CRPCProtocol_ErrorCode cpp_on_invoke({{.PacketName}}::CRPCProtocol& request, {{.PacketName}}::CRPCProtocol& response)
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
        return ({{.PacketName}}::CRPCProtocol_ErrorCode)rc;
    }

    return {{.PacketName}}::CRPCProtocol_ErrorCode_OK;
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


{{.PacketName}}::CRPCProtocol_ErrorCode cpp_invoke({{.PacketName}}::CRPCProtocol& request, {{.PacketName}}::CRPCProtocol& response)
{
    if (!on_server) {
        return {{.PacketName}}::CRPCProtocol_ErrorCode_NotInit;
    }

    {{.PacketName}}::CRPCProtocol_ErrorCode result = {{.PacketName}}::CRPCProtocol_ErrorCode_Fail;
    uintptr_t handle = (uintptr_t)cpp_invoke_callback;

    JsonTag* tag = new JsonTag();
    tag->body.assign("hello");

    do {
        {{.PacketName}}::CRPCProtocol_Inner* inner = request.mutable_inner();
        inner->set_callback(handle);
        inner->set_method(on_service_data);
        inner->set_callbackargs((uint64_t)tag);

        std::string data = request.SerializeAsString();

        int32_t rc = on_server((char*)data.data(), data.size());
        if (({{.PacketName}}::CRPCProtocol_ErrorCode)rc != {{.PacketName}}::CRPCProtocol_ErrorCode_OK) {
            result = ({{.PacketName}}::CRPCProtocol_ErrorCode)rc;
            break;
        }

        if (!response.ParseFromString(tag->body)) {
            printf("parse app response fail.\n");
            result = {{.PacketName}}::CRPCProtocol_ErrorCode_Fail;
            break;
        }
        else {
            // printf("C recv: debug string: %s\n", response.DebugString().data());
        }

        result = {{.PacketName}}::CRPCProtocol_ErrorCode_OK;
    } while(0);

    delete tag;

    return result;
}


{{.ExportHeader}}_API int crpc_call(char* data, int32_t length)
{
    {{.PacketName}}::CRPCProtocol request;
    if (!request.ParseFromArray(data, length)) {
        printf("parse CRPCProtocol fail.\n");
        return {{.PacketName}}::CRPCProtocol_ErrorCode_InvalidArgument;
    }

    {{.PacketName}}::CRPCProtocol response;

    if (request.has_inner())
    {
        const {{.PacketName}}::CRPCProtocol_Inner& inner = request.inner();

        if (0 == strcmp(set_callback_on_event, inner.method().data())) {
           // 回复上层
            on_callback = (FnMethod)inner.callback();
            response.set_code({{.PacketName}}::CRPCProtocol_ErrorCode_OK);
        }
        else if (0 == strcmp(set_callback_on_server, inner.method().data())) {
           // 调用上层的服务
            on_server = (FnMethod)inner.callback();
            if (on_server) {
              {{range .Services}}
                Set{{.ServiceName}}ClinetInvoke(cpp_invoke);
              {{end}}
            }

            response.set_code({{.PacketName}}::CRPCProtocol_ErrorCode_OK);
        }
        else if (0 == strcmp(on_service_data, inner.method().data())) {
            // 收到上层的回复
            fn_cpp_invoke_callback fn = (fn_cpp_invoke_callback)inner.callback();
            if (fn) {
                fn((void*)inner.callbackargs(), data, length);
            }
            return {{.PacketName}}::CRPCProtocol_ErrorCode_OK;;
        } else {
            response.set_code({{.PacketName}}::CRPCProtocol_ErrorCode_Fail);
            response.set_msg("unknown method " + inner.method());
        }

        return cpp_on_invoke(request, response);
    }
    else
    {
      {{range .Services}}
        if (std::string::npos != request.method().find("/{{.PacketName}}.{{.ServiceName}}/")) {
            Get{{.ServiceName}}ServiceImpl()->OnInvoke(request, response, cpp_on_invoke);
            return {{.PacketName}}::CRPCProtocol_ErrorCode_OK;;
        }
      {{end}}

        response.set_code({{.PacketName}}::CRPCProtocol_ErrorCode_Unimplemented);
        response.set_msg("unknown service " + request.method());
        return {{.PacketName}}::CRPCProtocol_ErrorCode_OK;;
    }
}
