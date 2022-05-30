package dlsym

import "C"

type DlSym struct {
	h       uintptr
	symList map[string]uintptr // unsafe.Pointer
}

func Open(name string) (*DlSym, error) {
	c, err := openSym(name)
	if err != nil {
		return nil, err
	} else {
		// log.Printf("library handle %X", c.h)
	}

	return c, nil
}

func newDlSym(h uintptr) *DlSym {
	return &DlSym{
		h:       h,
		symList: map[string]uintptr{},
	}
}

func (c *DlSym) Close() {
	closeSym(c)
}

// Lookup For example
//
// package main
//
///*
//
//#include <stdint.h>
//
//#include <string.h>
//
//#include <stdlib.h>
//
//typedef int32_t (*Fn_MessageBoxA)(void* hWnd, char* txt, char* caption, uint8_t tpe);
//
//int callFunc(uintptr_t h, char* txt, char* caption, uint8_t tpe) {
//
//	((Fn_MessageBoxA)h)(NULL, txt, caption, tpe);
// 	return 0;
//}
//
//*/
//
//import "C"
//
//
//import (
//	"log"
//	"unsafe"
//
//	"github.com/general252/protoc-gen-crpc/dlsym"
//)
//
//func init() {
//	log.SetFlags(log.LstdFlags | log.Lshortfile)
//}
//
//func main() {
//	o, err := dlsym.Open(`User32.dll`)
//	if err != nil {
//		log.Println(err)
//		return
//	}
//
//	defer o.Close()
//
//	f, err := o.Lookup("MessageBoxA")
//	if err != nil {
//		log.Println(err)
//		return
//	}
//
//	caption := C.CString("this is caption")
//	defer C.free(unsafe.Pointer(caption))
//
//	txt := C.CString("this is text")
//	defer C.free(unsafe.Pointer(txt))
//
//	const (
//		MB_OK          = 0
//		MB_OKCANCEL    = 1
//		MB_YESNOCANCEL = 3
//	)
//
//	C.callFunc(C.uintptr_t(f), txt, caption, C.uint8_t(MB_OK))
//}
//
func (c *DlSym) Lookup(name string) (uintptr, error) {
	if v, ok := c.symList[name]; ok {
		return v, nil
	}

	v, err := lookupSym(c, name)
	if err != nil {
		return uintptr(0), err
	}

	c.symList[name] = v
	return v, nil
}
