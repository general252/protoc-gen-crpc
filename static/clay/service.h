#ifndef __{{.ExportHeader}}_SERVICE_H__
#define __{{.ExportHeader}}_SERVICE_H__

#include "{{.GeneratedFilenamePrefix}}.pb.h"

typedef {{.PacketName}}::CRPCProtocol_ErrorCode (*fn_cpp_invoke)({{.PacketName}}::CRPCProtocol& request, {{.PacketName}}::CRPCProtocol& response);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

{{range .Services}}

class {{.ServiceName}}ServiceImpl
{
public:
  {{range .Methods}}
    virtual {{.PacketName}}::CRPCProtocol_ErrorCode {{.MethodName}}(const {{.PacketName}}::{{.Input}}& request,  {{.PacketName}}::{{.Output}}& response);
  {{end}}

public:
  void OnInvoke({{.PacketName}}::CRPCProtocol& request, {{.PacketName}}::CRPCProtocol& response, fn_cpp_invoke on_invoke);
};

{{.ServiceName}}ServiceImpl* Get{{.ServiceName}}ServiceImpl();
void Set{{.ServiceName}}ServiceImpl({{.ServiceName}}ServiceImpl* ins);

{{end}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


{{range .Services}}

class {{.ServiceName}}Client
{
public:
    {{.ServiceName}}Client() { invoke = 0; }
    fn_cpp_invoke invoke;

private:
    {{.PacketName}}::CRPCProtocol_ErrorCode m_invoke(const std::string& method, const google::protobuf::Message& request, google::protobuf::Message& response, {{.PacketName}}::CRPCProtocol* out);

public:
  {{range .Methods}}
    {{.PacketName}}::CRPCProtocol_ErrorCode {{.MethodName}}(const {{.PacketName}}::{{.Input}}& request, {{.PacketName}}::{{.Output}}& response, {{.PacketName}}::CRPCProtocol* out = NULL);
  {{end}}
};


{{.ServiceName}}Client* Get{{.ServiceName}}Client();
void Set{{.ServiceName}}ClientInvoke(fn_cpp_invoke invoke);

{{end}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#endif //  __{{.ExportHeader}}_SERVICE_H__
