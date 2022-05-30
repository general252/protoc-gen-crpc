package main

import (
	"context"
	"log"

	helloworld "example/golay"
)

func init() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
}

type GreeterServer struct {
	helloworld.UnimplementedGreeterServer
}

func (tis *GreeterServer) SayHello(context.Context, *helloworld.HelloRequest) (*helloworld.HelloReply, error) {
	return &helloworld.HelloReply{
		Message: "aaa",
	}, nil
}

func (tis *GreeterServer) Hello(ctx context.Context, req *helloworld.A) (*helloworld.B, error) {
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
