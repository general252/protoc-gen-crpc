#include "{{.GeneratedFilenamePrefix}}_client.h"

{{range .Services}}

static {{.ServiceName}}Client ins{{.ServiceName}}Client;
{{.ServiceName}}Client* Get{{.ServiceName}}Clinet() {
    return &ins{{.ServiceName}}Client;
}

void Set{{.ServiceName}}ClinetInvoke(fn_cpp_invoke invoke) {
    ins{{.ServiceName}}Client.invoke = invoke;
}


void {{.ServiceName}}Client::m_invoke(const std::string& method, const google::protobuf::Message& request, google::protobuf::Message& response) {
    if (!this->invoke) {
        return;
    }

    {{.PacketName}}::CRPCProtocol req, res;
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

{{range .Methods}}
void {{.ServiceName}}Client::{{.MethodName}}(const {{.PacketName}}::{{.Input}}& request, {{.PacketName}}::{{.Output}}& response) {
    this->m_invoke("/{{.PacketName}}.{{.ServiceName}}/{{.MethodName}}", request, response);
}
{{end}}

{{end}}