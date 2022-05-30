#include "{{.GeneratedFilenamePrefix}}_service.h"


#ifdef  _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "lib/debug/lib/libprotobufd.lib")
#pragma comment(lib, "lib/debug/lib/libprotobuf-lited.lib")
#else
#pragma comment(lib, "lib/libprotobuf.lib")
#pragma comment(lib, "lib/libprotobuf-lite.lib")
#endif // _DEBUG
#endif

{{range .Services}}

class my{{.ServiceName}}ServiceImpl : public {{.ServiceName}}ServiceImpl
{
public:

  {{range .Methods}}
    virtual void {{.MethodName}}(const {{.PacketName}}::{{.Input}}& request,  {{.PacketName}}::{{.Output}}& response) {

    }
  {{end}}

};

{{end}}



class InitApp
{
public:
    InitApp() {
    {{range .Services}}
        static my{{.ServiceName}}ServiceImpl ins{{.ServiceName}}ServiceImpl;
        Set{{.ServiceName}}ServiceImpl(ins{{.ServiceName}}ServiceImpl);
    {{end}}
    }
}_init_app;


