
app:
	go build -o ../bin/example.exe main.go

build:
	protoc --go_out=./golay --go_opt=paths=source_relative \
		   --cpp_out=./clay \
		   --go-grpc_out=./golay --go-grpc_opt=paths=source_relative \
		   --crpc_out=./golay --crpc_opt=paths=source_relative \
		   ./helloworld.proto

