#include "{{.GeneratedFilenamePrefix}}_service.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{{range .Services}}

static {{.ServiceName}}ServiceImpl* ins{{.ServiceName}} = NULL;
{{.ServiceName}}ServiceImpl* Get{{.ServiceName}}ServiceImpl() {
    if (NULL == ins{{.ServiceName}}) {
        static {{.ServiceName}}ServiceImpl mIns{{.ServiceName}};
        return &mIns{{.ServiceName}};
    }
    return ins{{.ServiceName}};
}

void Set{{.ServiceName}}ServiceImpl({{.ServiceName}}ServiceImpl* ins) {
    ins{{.ServiceName}} = ins;
}


void {{.ServiceName}}ServiceImpl::OnInvoke({{.PacketName}}::CRPCProtocol& request, {{.PacketName}}::CRPCProtocol& response, fn_cpp_invoke on_invoke)
{
    do {

    {{range .Methods}}
        if (request.method() == "/{{.PacketName}}.{{.ServiceName}}/{{.MethodName}}") {

            {{.PacketName}}::{{.Input}} req;
            {{.PacketName}}::{{.Output}} res;

            if (!req.ParseFromString(request.request())) {
                printf("parse fail. [%s]\n", request.method().data());
                response.set_code({{.PacketName}}::CRPCProtocol_ErrorCode_InvalidArgument);
                response.set_msg("parse protobuf fail " + request.method());
                break;
            }

            {{.PacketName}}::CRPCProtocol_ErrorCode code = this->{{.MethodName}}(req, res);

            response.set_response(res.SerializeAsString());
            response.set_code(code);

            break;
        }
    {{end}}

        response.set_code({{.PacketName}}::CRPCProtocol_ErrorCode_Unimplemented);
        response.set_msg(request.method() + " un implemented");
    }while(0);

    if (on_invoke) {
        on_invoke(request, response);
    }
}

{{range .Methods}}

{{.PacketName}}::CRPCProtocol_ErrorCode {{.ServiceName}}ServiceImpl::{{.MethodName}}(const {{.PacketName}}::{{.Input}}& request, {{.PacketName}}::{{.Output}}& response) {
    printf("{{.ServiceName}}Service::{{.MethodName}} un implemented\n");
    return {{.PacketName}}::CRPCProtocol_ErrorCode_Unimplemented;;
}

{{end}}

{{end}}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



{{range .Services}}

static {{.ServiceName}}Client ins{{.ServiceName}}Client;
{{.ServiceName}}Client* Get{{.ServiceName}}Client() {
    return &ins{{.ServiceName}}Client;
}

void Set{{.ServiceName}}ClientInvoke(fn_cpp_invoke invoke) {
    ins{{.ServiceName}}Client.invoke = invoke;
}


{{.PacketName}}::CRPCProtocol_ErrorCode {{.ServiceName}}Client::m_invoke(const std::string& method, const google::protobuf::Message& request, google::protobuf::Message& response, {{.PacketName}}::CRPCProtocol* out) {
    if (!this->invoke) {
        return {{.PacketName}}::CRPCProtocol_ErrorCode_NotInit;
    }

    {{.PacketName}}::CRPCProtocol req, res;
    req.set_method(method);
    req.set_request(request.SerializeAsString());

    if (!out) {
        out = &res;
    }

    {{.PacketName}}::CRPCProtocol_ErrorCode result = this->invoke(req, *out);
    if (result != {{.PacketName}}::CRPCProtocol_ErrorCode_OK) {
        return result;
    }

    if (out->code() != {{.PacketName}}::CRPCProtocol_ErrorCode_OK) {
        return out->code();
    }

    if (!response.ParseFromString(out->response())) {
        printf("parse fail %s\n", method.data());
        return {{.PacketName}}::CRPCProtocol_ErrorCode_Fail;
    }
    else {
        // printf("[%s] debug: %s\n", method.data(), response.DebugString().data());
    }

    return {{.PacketName}}::CRPCProtocol_ErrorCode_OK;
}

{{range .Methods}}
{{.PacketName}}::CRPCProtocol_ErrorCode {{.ServiceName}}Client::{{.MethodName}}(const {{.PacketName}}::{{.Input}}& request, {{.PacketName}}::{{.Output}}& response, {{.PacketName}}::CRPCProtocol* out) {
    return this->m_invoke("/{{.PacketName}}.{{.ServiceName}}/{{.MethodName}}", request, response, out);
}
{{end}}

{{end}}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////