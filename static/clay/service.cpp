#include "{{.GeneratedFilenamePrefix}}_service.h"


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


void {{.ServiceName}}ServiceImpl::OnInvoke(const {{.PacketName}}::CRPCProtocol& request, {{.PacketName}}::CRPCProtocol& response)
{
{{range .Methods}}
    if (request.method() == "/{{.PacketName}}.{{.ServiceName}}/{{.MethodName}}") {

        {{.PacketName}}::{{.Input}} req;
        {{.PacketName}}::{{.Output}} res;

        if (!req.ParseFromString(request.request())) {
            printf("parse fail. [%s]\n", request.method().data());
            return;
        }

        this->{{.MethodName}}(req, res);

        response.set_response(res.SerializeAsString());
        response.set_code({{.PacketName}}::CRPCProtocol_ErrorCode_OK);

        return;
    }
{{end}}

    response.set_code({{.PacketName}}::CRPCProtocol_ErrorCode_Unimplemented);
    response.set_msg(request.method() + " un implemented");
}

{{range .Methods}}

void {{.ServiceName}}ServiceImpl::{{.MethodName}}(const {{.PacketName}}::{{.Input}}& request, {{.PacketName}}::{{.Output}}& response) {
    printf("{{.ServiceName}}Service::{{.MethodName}} un implemented\n");
}

{{end}}

{{end}}
