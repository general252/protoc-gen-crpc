BUILD_VERSION := $(shell git rev-list --tags --max-count=1 | git describe --tags )

default:
	protoc --go_out=./call --go_opt=paths=source_relative crpc.proto
	gofmt -d -l -e -w .
	go mod tidy
	go build -ldflags="-w -s -X 'github.com/general252/protoc-gen-crpc/main.Version=${BUILD_VERSION}'" -o ./bin/protoc-gen-crpc.exe
