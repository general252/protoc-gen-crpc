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
	"io/fs"
	"log"
	"path/filepath"
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

	var writeTpl = func(filename string, text string) {
		g := gen.NewGeneratedFile(filename, file.GoImportPath)

		generateFileContent(gen, file, g, text)
	}

	writeTpl(file.GeneratedFilenamePrefix+".go", static.GetGoTpl())
	writeTpl("../clay/"+file.GeneratedFilenamePrefix+"_export.h", static.GetExportH())
	writeTpl("../clay/"+file.GeneratedFilenamePrefix+"_export.cpp", static.GetExportCpp())
	writeTpl("../clay/"+file.GeneratedFilenamePrefix+"_service.h", static.GetServiceH())
	writeTpl("../clay/"+file.GeneratedFilenamePrefix+"_service.cpp", static.GetServiceCpp())

	var haveFile = false
	_ = filepath.WalkDir(".", func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		if d.IsDir() {
			return nil
		}

		if strings.Contains(path, "my_app.cpp") {
			haveFile = true
		}

		return nil
	})
	if haveFile {
	} else {
		writeTpl("../clay/"+"my_app.cpp", static.GetMyAppCPP())
	}

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
		ExportHeader            string
		GeneratedFilenamePrefix string
		PacketName              string
		Services                []JsonService
		GoService               JsonService
		NotifyService           JsonService
	}

	// 参数
	var param = JsonServiceList{
		ExportHeader:            strings.ReplaceAll(strings.ToUpper(file.GeneratedFilenamePrefix), "-", "_"),
		GeneratedFilenamePrefix: file.GeneratedFilenamePrefix,
		PacketName:              string(file.GoPackageName),
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
}
