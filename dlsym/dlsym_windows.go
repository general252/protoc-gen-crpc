package dlsym

/*
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static char* GetLastErrorStr()
{
#define BUFFER_LEN 256
    static char buffer[BUFFER_LEN] = { 0 };
    memset(buffer, 0, BUFFER_LEN);

    DWORD errorCode = GetLastError();
	sprintf(buffer, "[GetLastError(): %d, %X]", (int)errorCode, (int)errorCode);

    //FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, buffer, BUFFER_LEN, NULL);

    return buffer;
}

static uintptr_t dlsymOpen(const char* path, char** err) {
	void* h = LoadLibraryA(path);
	if (h == NULL) {
		*err = (char*)GetLastErrorStr();
	} else {
		printf("library handle: %I64X\n", (uintptr_t)h);
	}

	return (uintptr_t)h;
}


static uintptr_t dlsymLookup(uintptr_t h, const char* name, char** err) {
	void* r = GetProcAddress((HMODULE)h, name);
	if (r == NULL) {
		printf("lib handle: %I64X name: %s\n", h, name);
		*err = (char*)GetLastErrorStr();
	}
	return (uintptr_t)r;
}

static int dlsymClose(uintptr_t h) {
	printf("library handle close: %I64X\n", h);
	FreeLibrary((HMODULE)h);
	return 0;
}

*/
import "C"

import (
	"errors"
	"unsafe"
)

func openSym(name string) (*DlSym, error) {
	cPath := C.CString(name)
	defer C.free(unsafe.Pointer(cPath))

	var cErr *C.char
	h := C.dlsymOpen(cPath, &cErr)
	if h == 0 {
		return nil, errors.New(`dlsym.Open("` + name + `"): ` + C.GoString(cErr))
	}

	return newDlSym(uintptr(h)), nil
}

func closeSym(p *DlSym) {
	C.dlsymClose(C.uintptr_t(p.h))
}

func lookupSym(p *DlSym, symName string) (uintptr, error) {
	var (
		cErr *C.char
		name = make([]byte, C.PATH_MAX+1)
	)
	copy(name, symName)

	in := (*C.char)(unsafe.Pointer(&name[0]))

	v := C.dlsymLookup(C.uintptr_t(p.h), in, &cErr)
	if v == 0 {
		return uintptr(0), errors.New(`lookup("` + symName + `"): ` + C.GoString(cErr))
	}

	return uintptr(v), nil
}
