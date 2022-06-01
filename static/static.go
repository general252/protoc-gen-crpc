package static

import (
	_ "embed"
)

var Version = "0.1.1"

//go:embed golay/hello.gotpl
var helloGoTpl []byte

func GetGoTpl() string {
	return string(helloGoTpl)
}

//go:embed clay/export.h
var exportH []byte

func GetExportH() string {
	return string(exportH)
}

//go:embed clay/export.cpp
var exportCPP []byte

func GetExportCpp() string {
	return string(exportCPP)
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

//go:embed clay/my_app.cpp
var myAppCPP []byte

func GetMyAppCPP() string {
	return string(myAppCPP)
}
