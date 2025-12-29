
default:
	go mod tidy
	go build -o ../bin/example.exe example.go

run:
	../bin/example.exe

build:
	protoc --go_out=./golay --go_opt=paths=source_relative \
		   --cpp_out=./clay \
		   --crpc_out=./golay --crpc_opt=paths=source_relative \
		   ./helloworld.proto

