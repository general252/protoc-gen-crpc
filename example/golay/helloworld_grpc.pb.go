// Code generated by protoc-gen-go-grpc. DO NOT EDIT.
// versions:
// - protoc-gen-go-grpc vv0.1.7
// - protoc             v3.19.4
// source: helloworld.proto

package helloworld

import (
	context "context"
	fmt "fmt"
	call "github.com/general252/protoc-gen-crpc/call"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
	proto "google.golang.org/protobuf/proto"
)

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
// Requires gRPC-Go v1.32.0 or later.
const _ = grpc.SupportPackageIsVersion7

// GreeterClient is the client API for Greeter service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://pkg.go.dev/google.golang.org/grpc/?tab=doc#ClientConn.NewStream.
type GreeterClient interface {
	// Sends a greeting
	SayHello(ctx context.Context, in *HelloRequest, opts ...grpc.CallOption) (*HelloReply, error)
	// 你好
	Hello(ctx context.Context, in *A, opts ...grpc.CallOption) (*B, error)
}

type greeterClient struct {
	cc grpc.ClientConnInterface
}

func NewGreeterClient(cc grpc.ClientConnInterface) GreeterClient {
	return &greeterClient{cc}
}

func (c *greeterClient) SayHello(ctx context.Context, in *HelloRequest, opts ...grpc.CallOption) (*HelloReply, error) {
	out := new(HelloReply)
	err := c.cc.Invoke(ctx, "/helloworld.Greeter/SayHello", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *greeterClient) Hello(ctx context.Context, in *A, opts ...grpc.CallOption) (*B, error) {
	out := new(B)
	err := c.cc.Invoke(ctx, "/helloworld.Greeter/Hello", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// GreeterServer is the server API for Greeter service.
// All implementations must embed UnimplementedGreeterServer
// for forward compatibility
type GreeterServer interface {
	// Sends a greeting
	SayHello(context.Context, *HelloRequest) (*HelloReply, error)
	// 你好
	Hello(context.Context, *A) (*B, error)
	mustEmbedUnimplementedGreeterServer()
}

// UnimplementedGreeterServer must be embedded to have forward compatible implementations.
type UnimplementedGreeterServer struct {
}

func (UnimplementedGreeterServer) SayHello(context.Context, *HelloRequest) (*HelloReply, error) {
	return nil, status.Errorf(codes.Unimplemented, "method SayHello not implemented")
}
func (UnimplementedGreeterServer) Hello(context.Context, *A) (*B, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Hello not implemented")
}
func (UnimplementedGreeterServer) mustEmbedUnimplementedGreeterServer() {}

// UnsafeGreeterServer may be embedded to opt out of forward compatibility for this service.
// Use of this interface is not recommended, as added methods to GreeterServer will
// result in compilation errors.
type UnsafeGreeterServer interface {
	mustEmbedUnimplementedGreeterServer()
}

func RegisterGreeterServer(s grpc.ServiceRegistrar, srv GreeterServer) {
	s.RegisterService(&Greeter_ServiceDesc, srv)
}

func _Greeter_SayHello_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(HelloRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(GreeterServer).SayHello(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/helloworld.Greeter/SayHello",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(GreeterServer).SayHello(ctx, req.(*HelloRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _Greeter_Hello_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(A)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(GreeterServer).Hello(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/helloworld.Greeter/Hello",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(GreeterServer).Hello(ctx, req.(*A))
	}
	return interceptor(ctx, in, info, handler)
}

// Greeter_ServiceDesc is the grpc.ServiceDesc for Greeter service.
// It's only intended for direct use with grpc.RegisterService,
// and not to be introspected or modified (even as a copy)
var Greeter_ServiceDesc = grpc.ServiceDesc{
	ServiceName: "helloworld.Greeter",
	HandlerType: (*GreeterServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "SayHello",
			Handler:    _Greeter_SayHello_Handler,
		},
		{
			MethodName: "Hello",
			Handler:    _Greeter_Hello_Handler,
		},
	},
	Streams:  []grpc.StreamDesc{},
	Metadata: "helloworld.proto",
}

// -----------------------------------------------------------------------------------------------------

var _ = call.CRPCProtocol{}
var _ = proto.Marshal
var _ = fmt.Println

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
		return nil, fmt.Errorf("server not init")
	}

	var (
		err   error
		reply interface{}
		dec   = func(v interface{}) error {
			return proto.Unmarshal(request.GetRequest(), v.(proto.Message))
		}
	)

	// 匹配服务函数
	for _, method := range c.desc.Methods {
		methodName := fmt.Sprintf("/%s/%s", c.desc.ServiceName, method.MethodName)
		if methodName == request.GetMethod() {
			reply, err = method.Handler(c.impl, context.TODO(), dec, nil)
			break
		}
	}

	if err != nil {
		return nil, err
	}
	if reply == nil {
		return nil, fmt.Errorf("no reply")
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
		return fmt.Errorf("Call fail. [%v] %v, %v", method, resp.GetCode(), resp.GetMsg())
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
	c.impl = impl.(GreeterServer)
	c.desc = desc
}
