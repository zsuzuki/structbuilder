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
{{- if .UseLua}}#include <sol/sol.hpp>{{end}}
{{if .NameSpace}}namespace {{.NameSpace}} {
{{- end}}
{{template "struct_base" .TopStruct}}
{{if .NameSpace}}} // namespace {{.NameSpace}}{{end}}
