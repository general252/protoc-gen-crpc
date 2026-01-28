#ifndef __{{.ExportHeader}}_EXPORT_H_
#define __{{.ExportHeader}}_EXPORT_H_

// g++ -fPIC -shared *.cpp *.cc -o clay.so -lprotobuf -lpthread -std=c++11

#ifdef _MSC_VER

#ifdef {{.ExportHeader}}_EXPORTS
#define {{.ExportHeader}}_API __declspec(dllexport)
#else
#define {{.ExportHeader}}_API __declspec(dllimport)
#endif

#else

#define {{.ExportHeader}}_API

#endif // _MSC_VER

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


#endif // __{{.ExportHeader}}_EXPORT_H_
