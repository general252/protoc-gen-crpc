#ifdef {{.ExportHeader}}_EXPORTS
#define {{.ExportHeader}}_API __declspec(dllexport)
#else
#define {{.ExportHeader}}_API __declspec(dllimport)
#endif

#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


{{.ExportHeader}}_API int crpc_call(char* data, int32_t length);


#ifdef __cplusplus
};
#endif // __cplusplus
