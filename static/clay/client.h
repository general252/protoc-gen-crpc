#pragma once
#include "{{.GeneratedFilenamePrefix}}.pb.h"

typedef void (*fn_cpp_invoke)({{.PacketName}}::CRPCProtocol& request, {{.PacketName}}::CRPCProtocol& response);

{{range .Services}}

class {{.ServiceName}}Client
{
public:
    {{.ServiceName}}Client() { invoke = 0; }
    fn_cpp_invoke invoke;

private:
    void m_invoke(const std::string& method, const google::protobuf::Message& request, google::protobuf::Message& response);

public:
  {{range .Methods}}
    void {{.MethodName}}(const {{.PacketName}}::{{.Input}}& request, {{.PacketName}}::{{.Output}}& response);
  {{end}}
};


{{.ServiceName}}Client* Get{{.ServiceName}}Clinet();
void Set{{.ServiceName}}ClinetInvoke(fn_cpp_invoke invoke);



{{end}}