package static

import (
	_ "embed"
)

//go:embed golay/hello.gotpl
var helloGoTpl []byte

func GetGoTpl() string {
	return string(helloGoTpl)
}

//go:embed clay/app.h
var appH []byte

func GetAppH() string {
	return string(appH)
}

//go:embed clay/app.cpp
var appCPP []byte

func GetAppCpp() string {
	return string(appCPP)
}

//go:embed clay/service.h
var serviceH []byte

func GetServiceH() string {
	return string(serviceH)
}

//go:embed clay/service.cpp
var serviceCPP []byte

func GetServiceCpp() string {
	return string(serviceCPP)
}

//go:embed clay/client.h
var clientH []byte

func GetClientH() string {
	return string(clientH)
}

//go:embed clay/client.cpp
var clientCPP []byte

func GetClientCpp() string {
	return string(clientCPP)
}
