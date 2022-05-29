
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