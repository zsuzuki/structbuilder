//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
#pragma once

{{if gt (len .LocalInclude) 0}}{{range .LocalInclude}}#include "{{.}}"
{{end -}}
{{end}}
{{if gt (len .Include) 0}}{{range .Include}}#include <{{.}}>
{{end -}}
{{end}}
{{- if .HasNameSpace}}namespace {{.NameSpace}} {
{{- end}}
// total data size
size_t get{{.Struct.Name}}PackSize(const {{.Struct.Name}}& s);
// pack interface
void pack{{.Struct.Name}}(Serializer& ser, {{.Struct.Name}}& s);
// unpack interface
void unpack{{.Struct.Name}}(Serializer& ser, {{.Struct.Name}}& s);
{{if .HasNameSpace}}} // namespace {{.NameSpace}}{{end}}
