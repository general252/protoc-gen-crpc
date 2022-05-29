

default:
	protoc --go_out=./call --go_opt=paths=source_relative crpc.proto
	gofmt -d -l -e -w .
	go mod tidy
	go build -ldflags="-w -s" -o C:/Users/tony/AppData/Local/GoEnv/path/bin/protoc-gen-crpc.exe
