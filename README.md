
- golang call C library with rpc style

```
package main

import (
	"context"
	helloworld "example/golay"
	"log"
)

func init() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
}

func main() {
	lib := `your_library_path/library-name.dll`
	conn := helloworld.NewGreeterLibrary(lib)

	if err := conn.Load(); err != nil {
		log.Println(err)
		return
	}

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

```


- Install (like [GRPC](https://www.grpc.io/docs/languages/go/quickstart/))
```
# go install google.golang.org/protobuf/cmd/protoc-gen-go@v1.28
# go install github.com/general252/protoc-gen-crpc@0.1.6
```

- reference
[protoc-gen-go-grpc](https://github.com/grpc/grpc-go/tree/master/cmd/protoc-gen-go-grpc)
