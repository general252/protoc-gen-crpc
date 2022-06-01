package call

/*
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef int32_t (*FnMethod)(char* data, int32_t length);

static int32_t call_method(uintptr_t h, char* data, int32_t length) {
	return ((FnMethod)h)(data, length);
}

int32_t FnOnEventGo(char* data, int32_t length);
int32_t FnServerGo(char* data, int32_t length);

static uintptr_t GetFnOnEventGo() {
	return (uintptr_t)FnOnEventGo;
}

static uintptr_t GetFnServerGo() {
	return (uintptr_t)FnServerGo;
}
*/
import "C"

import (
	"errors"
	"fmt"
	"log"
	"reflect"
	"runtime/cgo"
	"sync"
	"sync/atomic"
	"unsafe"

	"google.golang.org/protobuf/proto"

	"github.com/general252/protoc-gen-crpc/dlsym"
)

//export FnOnEventGo
func FnOnEventGo(data *C.char, length C.int32_t) C.int32_t {
	var payload []byte = C.GoBytes(unsafe.Pointer(data), length)

	var resp CRPCProtocol
	if err := proto.Unmarshal(payload, &resp); err != nil {
		log.Println(err)
		return C.int32_t(CRPCProtocol_InvalidArgument)
	}

	var h = resp.GetFunHandle()
	if h == 0 {
		log.Println("fun handle is nil")
		return C.int32_t(0)
	}

	defer func() {
		if err := recover(); err != nil {
			log.Println(err)
		}
	}()

	if v, ok := cgo.Handle(h).Value().(func(*CRPCProtocol)); ok {
		v(&resp)
	} else {
		log.Printf("cgo handle value not is function. %v", h)
	}

	return C.int32_t(CRPCProtocol_OK)
}

//export FnServerGo
func FnServerGo(data *C.char, length C.int32_t) C.int32_t {
	var payload []byte = C.GoBytes(unsafe.Pointer(data), length)

	var request CRPCProtocol
	if err := proto.Unmarshal(payload, &request); err != nil {
		log.Println(err)
		return C.int32_t(CRPCProtocol_InvalidArgument)
	}

	libraryMap.Range(func(key, value interface{}) bool {
		v, ok := value.(*Library)
		if ok && v != nil {
			response, err := v.onServer(&request)
			if err != nil {
				response = &CRPCProtocol{
					Method:   request.GetMethod(),
					Request:  request.GetRequest(),
					Response: nil,
					Code:     CRPCProtocol_Fail,
					Msg:      err.Error(),
					Inner:    request.GetInner(),
				}

				log.Printf("on server fail. %v", err)
			}
			response.Inner = request.GetInner()

			if err = v.OnCall(response); err != nil {
				log.Printf("on call fail. %v", err)
			}
		}
		return true
	})

	return C.int32_t(CRPCProtocol_OK)
}

var (
	libraryMap = sync.Map{}
	libraryIdx uint64
)

type OnLibraryServer func(request *CRPCProtocol) (*CRPCProtocol, error)

type Library struct {
	id   uint64
	name string

	sym    *dlsym.DlSym
	fnCall uintptr

	onServer OnLibraryServer
}

func NewLibrary(name string, onServer OnLibraryServer) *Library {
	c := &Library{
		id:       atomic.AddUint64(&libraryIdx, 1),
		name:     name,
		onServer: onServer,
	}

	return c
}

// OnCall 回复调用者
func (c *Library) OnCall(res *CRPCProtocol) error {
	data, err := proto.Marshal(res)
	if err != nil {
		return err
	}

	in := (*C.char)(unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&data)).Data))
	inLen := C.int32_t(len(data))

	var r = C.call_method(C.uintptr_t(c.fnCall), in, inLen)
	if CRPCProtocol_ErrorCode(r) != CRPCProtocol_OK {
		return errors.New(fmt.Sprintf("error result: %v", CRPCProtocol_ErrorCode(r)))
	}

	return nil
}

// Call 调用
func (c *Library) Call(req *CRPCProtocol) (*CRPCProtocol, error) {
	var res *CRPCProtocol
	h := cgo.NewHandle(func(response *CRPCProtocol) {
		res = response
	})

	req.FunHandle = uint64(h)

	data, err := proto.Marshal(req)
	if err != nil {
		return nil, err
	}

	in := (*C.char)(unsafe.Pointer((*reflect.StringHeader)(unsafe.Pointer(&data)).Data))
	inLen := C.int32_t(len(data))

	var r = C.call_method(C.uintptr_t(c.fnCall), in, inLen)
	if CRPCProtocol_ErrorCode(r) != CRPCProtocol_OK {
		return nil, errors.New(fmt.Sprintf("error result: %v", CRPCProtocol_ErrorCode(r)))
	}

	if res == nil {
		return nil, errors.New("no response")
	}

	return res, nil
}

func (c *Library) init() (err error) {

	const (
		crpc_call              = "crpc_call"
		set_callback_on_event  = "set_callback_on_event"
		set_callback_on_server = "set_callback_on_server"
	)

	// 加载动态库
	c.sym, err = dlsym.Open(c.name)
	if err != nil {
		return err
	}

	defer func() {
		if err != nil {
			c.sym.Close()
		}
	}()

	// 查找库函数
	c.fnCall, err = c.sym.Lookup(crpc_call)
	if err != nil {
		return err
	}

	// 设置函数回调
	res, err := c.Call(&CRPCProtocol{
		Inner: &CRPCProtocol_Inner{
			Method:   set_callback_on_event,
			Callback: uint64(C.GetFnOnEventGo()),
		},
	})
	if err != nil {
		return err
	}
	if CRPCProtocol_OK != res.GetCode() {
		return errors.New(fmt.Sprintf("set_callback_on_event fail. %v, %v", res.GetCode(), res.GetMsg()))
	}

	// 设置服务回调
	res, err = c.Call(&CRPCProtocol{
		Inner: &CRPCProtocol_Inner{
			Method:   set_callback_on_server,
			Callback: uint64(C.GetFnServerGo()),
		},
	})
	if err != nil {
		return err
	}
	if CRPCProtocol_OK != res.GetCode() {
		return errors.New(fmt.Sprintf("set_callback_on_server fail. %v, %v", res.GetCode(), res.GetMsg()))
	}

	// 初始化成功后保存
	libraryMap.Store(c.id, c)

	return nil
}

func Open(name string, onServer OnLibraryServer) (*Library, error) {
	c := NewLibrary(name, onServer)
	if err := c.init(); err != nil {
		return nil, err
	}

	return c, nil
}
