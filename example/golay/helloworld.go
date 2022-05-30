package helloworld

import (
	"context"
	"errors"
	"fmt"

	"google.golang.org/grpc"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/status"
	"google.golang.org/protobuf/proto"

	"github.com/general252/protoc-gen-crpc/call"
)

type GreeterLibrary struct {
	libraryName string        // c库路径
	lib         *call.Library // c库

	desc *grpc.ServiceDesc // 服务描述
	impl GreeterServer     // 服务实现
}

func NewGreeterLibrary(name string) *GreeterLibrary {
	return &GreeterLibrary{
		libraryName: name,
	}
}

func (c *GreeterLibrary) Load() error {
	lib, err := call.Open(c.libraryName, c.onServer)
	if err != nil {
		return err
	}

	c.lib = lib
	return nil
}

func (c *GreeterLibrary) onServer(request *call.CRPCProtocol) (*call.CRPCProtocol, error) {
	if c.desc == nil {
		return nil, errors.New("server not init")
	}

	var (
		err   error
		reply interface{}
		dec   = func(v interface{}) error {
			return proto.Unmarshal(request.GetResponse(), v.(proto.Message))
		}
	)

	// 匹配服务函数
	for _, method := range c.desc.Methods {
		if method.MethodName == request.GetMethod() {
			reply, err = method.Handler(c.impl, context.TODO(), dec, nil)
			break
		}
	}

	if err != nil {
		return nil, err
	}

	replyData, err := proto.Marshal(reply.(proto.Message))
	if err != nil {
		return nil, err
	}

	request.Response = replyData
	request.Code = call.CRPCProtocol_OK
	return &call.CRPCProtocol{
		Method:   request.GetMethod(),
		Request:  request.GetRequest(),
		Response: replyData,
		Code:     call.CRPCProtocol_OK,
		Msg:      "ok",
		Inner:    nil,
	}, nil
}

func (c *GreeterLibrary) NewCC() grpc.ClientConnInterface {
	return c
}

func (c *GreeterLibrary) RegisterServer(srv GreeterServer) {
	c.impl = srv
	RegisterGreeterServer(c, srv)
}

func (c *GreeterLibrary) call(method string, args interface{}, reply proto.Message) error {
	reqData, err := proto.Marshal(args.(proto.Message))
	if err != nil {
		return err
	}

	resp, err := c.lib.Call(&call.CRPCProtocol{
		Method:  method,
		Request: reqData,
	})
	if err != nil {
		return err
	}

	if resp.GetCode() != call.CRPCProtocol_OK {
		return errors.New(fmt.Sprintf("Call fail. [%v] %v, %v", method, resp.GetCode(), resp.GetMsg()))
	}

	if err = proto.Unmarshal(resp.GetResponse(), reply); err != nil {
		return err
	}

	return nil
}

func (c *GreeterLibrary) Invoke(ctx context.Context, method string, args interface{}, reply interface{}, opts ...grpc.CallOption) error {
	return c.call(method, args, reply.(proto.Message))
}

func (c *GreeterLibrary) NewStream(ctx context.Context, desc *grpc.StreamDesc, method string, opts ...grpc.CallOption) (grpc.ClientStream, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Hello not implemented")
}

func (c *GreeterLibrary) RegisterService(desc *grpc.ServiceDesc, impl interface{}) {
	c.desc = desc
}
