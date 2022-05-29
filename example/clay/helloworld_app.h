
#ifdef DYC_EXPORTS
#define DYC_API __declspec(dllexport)
#else
#define DYC_API __declspec(dllimport)
#endif

#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


DYC_API int crpc_call(char* data, int32_t length);


#ifdef __cplusplus
};
#endif // __cplusplus


