#pragma once

#include "{{.PacketName}}.pb.h"

{{range .Services}}

class {{.ServiceName}}Service
{
public:
    void OnInvoke(const helloworld::CRPCProtocol& request, helloworld::CRPCProtocol& response);
  {{range .Methods}}
    virtual void {{.MethodName}}(const {{.PacketName}}::{{.Input}}& request,  {{.PacketName}}::{{.Output}}& response);
  {{end}}
};

{{.ServiceName}}Service* Get{{.ServiceName}}Service();
void Set{{.ServiceName}}Service({{.ServiceName}}Service* ins);

{{end}}

