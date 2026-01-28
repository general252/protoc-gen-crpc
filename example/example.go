package main

import (
	"context"
	helloworld "example/golay"
	"log"
)

func init() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
}

type GreeterServer struct {
	helloworld.UnimplementedGreeterServer
}

func (tis *GreeterServer) SayHello(_ context.Context, req *helloworld.HelloRequest) (*helloworld.HelloReply, error) {
	log.Printf("recv %v %v", req.GetName(), req.GetAge())
	return &helloworld.HelloReply{
		Message: "aaa",
	}, nil
}

func (tis *GreeterServer) Hello(ctx context.Context, req *helloworld.A) (*helloworld.B, error) {
	log.Printf("recv [%v]", req.GetA())
	return &helloworld.B{
		B: "nb+" + req.GetA(),
	}, nil
}

func main() {
	lib := `dyc.dll`
	conn := helloworld.NewGreeterLibrary(lib)
	if err := conn.Load(); err != nil {
		log.Println(err)
		return
	}
	helloworld.RegisterGreeterServer(conn, new(GreeterServer))

	cli := helloworld.NewGreeterClient(conn.NewCC())

	helloReply, err := cli.SayHello(context.TODO(), &helloworld.HelloRequest{
		Name: "tina",
		Age:  20,
	})
	if err != nil {
		log.Println(err)
		return
	}

	log.Println(helloReply.String())
}
