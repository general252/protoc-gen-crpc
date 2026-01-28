
#ifndef __HELLOWORLD_EXPORT_H_
#define __HELLOWORLD_EXPORT_H_

// g++ -fPIC -shared *.cpp *.cc -o clay.so -lprotobuf -lpthread -std=c++11

#ifdef _MSC_VER

#ifdef HELLOWORLD_EXPORTS
#define HELLOWORLD_API __declspec(dllexport)
#else
#define HELLOWORLD_API __declspec(dllimport)
#endif

#else

#define HELLOWORLD_API

#endif // _MSC_VER

#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


HELLOWORLD_API int crpc_call(char* data, int32_t length);


#ifdef __cplusplus
};
#endif // __cplusplus


#endif // __HELLOWORLD_EXPORT_H_


