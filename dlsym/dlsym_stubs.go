//go:build (linux && cgo) || (darwin && cgo) || (freebsd && cgo)

package dlsym

/*
#cgo linux LDFLAGS: -ldl
#include <dlfcn.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

static uintptr_t dlsymSetEnv(const char* key, const char* value) {
	setenv(key, value, 1);
	printf(">>> LD_LIBRARY_PATH: %s\n", getenv("LD_LIBRARY_PATH"));
}

static uintptr_t dlsymOpen(const char* path, char** err) {
	void* h = dlopen(path, RTLD_NOW|RTLD_GLOBAL);
	if (h == NULL) {
		*err = (char*)dlerror();
	} else {
		printf("library handle: %" PRId64 "\n", (uintptr_t)h);
	}

	return (uintptr_t)h;
}

static uintptr_t dlsymLookup(uintptr_t h, const char* name, char** err) {
	void* r = dlsym((void*)h, name);
	if (r == NULL) {
		printf("lib handle: %" PRId64 " name: %s\n", h, name);
		*err = (char*)dlerror();
	}
	return (uintptr_t)r;
}

static int dlsymClose(uintptr_t h) {
	printf("library handle close: %" PRId64 "\n", h);
	dlclose((void*)h);
	return 0;
}
*/
import "C"

import (
	"errors"
	"os"
	"path/filepath"
	"unsafe"
)

func setEnv() {
	dir := filepath.Dir(os.Args[0])
	dir, _ = filepath.Abs(dir)
	dir = filepath.ToSlash(dir)

	key := C.CString("LD_LIBRARY_PATH")
	defer C.free(unsafe.Pointer(key))

	value := C.CString(dir)
	defer C.free(unsafe.Pointer(value))

	C.dlsymSetEnv(key, value)
}

func openSym(name string) (*DlSym, error) {
	var cErr *C.char
	in := C.CString(name)
	defer C.free(unsafe.Pointer(in))

	setEnv()

	h := C.dlsymOpen(in, &cErr)
	if h == 0 {
		return nil, errors.New(`dlsym.Open("` + name + `"): ` + C.GoString(cErr))
	}

	return newDlSym(uintptr(h)), nil
}

func closeSym(p *DlSym) {
	C.dlsymClose(C.uintptr_t(p.h))
}

func lookupSym(p *DlSym, symName string) (uintptr, error) {
	var cErr *C.char
	in := C.CString(symName)
	defer C.free(unsafe.Pointer(in))

	v := C.dlsymLookup(C.uintptr_t(p.h), in, &cErr)
	if v == 0 {
		return uintptr(0), errors.New(`lookup("` + symName + `"): ` + C.GoString(cErr))
	}

	return uintptr(v), nil
}
