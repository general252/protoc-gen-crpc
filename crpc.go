/*
 *
 * Copyright 2020 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

package main

import (
	"bytes"
	"github.com/general252/protoc-gen-crpc/static"
	"google.golang.org/protobuf/compiler/protogen"
	"log"
	"os"
	"strings"
	"text/template"
)

const (
	contextPackage = protogen.GoImportPath("context")
	grpcPackage    = protogen.GoImportPath("google.golang.org/grpc")
	codesPackage   = protogen.GoImportPath("google.golang.org/grpc/codes")
	statusPackage  = protogen.GoImportPath("google.golang.org/grpc/status")
)

// generateFile generates a _grpc.pb.go file containing gRPC service definitions.
func generateFile(gen *protogen.Plugin, file *protogen.File) *protogen.GeneratedFile {
	if len(file.Services) == 0 {
		return nil
	}

	var writeTpl = func(isC bool, name string, text string) {
		var filename = "../clay/" + file.GeneratedFilenamePrefix + name
		if !isC {
			filename = file.GeneratedFilenamePrefix + name
		}

		g := gen.NewGeneratedFile(filename, file.GoImportPath)

		generateFileContent(gen, file, g, text)
	}

	writeTpl(false, ".go", static.GetGoTpl())
	writeTpl(true, "_app.h", static.GetAppH())
	writeTpl(true, "_app.cpp", static.GetAppCpp())
	writeTpl(true, "_service.h", static.GetServiceH())
	writeTpl(true, "_service.cpp", static.GetServiceCpp())

	return nil
}

// generateFileContent generates the gRPC service definitions, excluding the package statement.
func generateFileContent(gen *protogen.Plugin, file *protogen.File, g *protogen.GeneratedFile, text string) {
	if len(file.Services) == 0 {
		return
	}

	type JsonMethod struct {
		PacketName  string
		ServiceName string
		MethodName  string
		Input       string
		Output      string
	}

	type JsonService struct {
		PacketName  string
		ServiceName string
		Methods     []JsonMethod
	}

	type JsonServiceList struct {
		PacketName    string
		Services      []JsonService
		GoService     JsonService
		NotifyService JsonService
	}

	// 参数
	var param = JsonServiceList{
		PacketName: string(file.GoPackageName),
	}

	for _, s := range file.Services {
		service := JsonService{
			PacketName:  string(file.GoPackageName),
			ServiceName: s.GoName,
			Methods:     nil,
		}
		for _, method := range s.Methods {
			service.Methods = append(service.Methods, JsonMethod{
				PacketName:  string(file.GoPackageName),
				ServiceName: service.ServiceName,
				MethodName:  method.GoName,
				Input:       g.QualifiedGoIdent(method.Input.GoIdent),
				Output:      g.QualifiedGoIdent(method.Output.GoIdent),
			})
		}

		if strings.HasPrefix(strings.ToUpper(service.ServiceName), "GO") {
			param.GoService = service
		} else if strings.HasPrefix(strings.ToUpper(service.ServiceName), "NOTIFY") {
			param.NotifyService = service
		} else {
			param.Services = append(param.Services, service)
		}
	}

	var getTpl = func(name string, text string) (*bytes.Buffer, error) {
		tmpl, err := template.New(name).Parse(text)
		if err != nil {
			return nil, err
		}

		buf := bytes.Buffer{}

		err = tmpl.Execute(&buf, param)
		if err != nil {
			return nil, err
		}

		return &buf, nil
	}

	// tmpl
	{
		buf, err := getTpl("tpl_name", text)
		if err != nil {
			log.Println(err)
			return
		}

		g.P()
		g.P(buf.String())
		g.P()
	}

	if false {
		var writeTpl = func(name string, text string) {
			filename := file.GeneratedFilenamePrefix + name
			buf, err := getTpl(name, static.GetAppH())
			if err != nil {
				log.Println(err)
				return
			}
			if err = os.WriteFile(filename, buf.Bytes(), os.ModePerm); err != nil {
				log.Println(err)
			}
		}

		writeTpl("_app.h", static.GetAppH())
		writeTpl("_app.cpp", static.GetAppCpp())
		writeTpl("_service.h", static.GetServiceH())
		writeTpl("_service.cpp", static.GetServiceCpp())
	}
}
