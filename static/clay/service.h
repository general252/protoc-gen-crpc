#pragma once

#include "{{.PacketName}}.pb.h"

{{range .Services}}

class {{.ServiceName}}ServiceImpl
{
public:
    void OnInvoke(const {{.PacketName}}::CRPCProtocol& request, {{.PacketName}}::CRPCProtocol& response);
  {{range .Methods}}
    virtual void {{.MethodName}}(const {{.PacketName}}::{{.Input}}& request,  {{.PacketName}}::{{.Output}}& response);
  {{end}}
};

{{.ServiceName}}ServiceImpl* Get{{.ServiceName}}ServiceImpl();
void Set{{.ServiceName}}ServiceImpl({{.ServiceName}}ServiceImpl* ins);

{{end}}

