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
{{if .HasNameSpace}}namespace {{.NameSpace}} {
//
size_t get{{.Struct.Name}}PackSize(const {{.Struct.Name}}& s) {
    return 0;
}
//
void pack{{.Struct.Name}}(Serializer& ser, {{.Struct.Name}}& s) {
{{range .Struct.DispMembers}}{{.Brank}}{{if .BracketClose}}  {{.Brank}}}
{{else if len .Children}}put<uint16_t>({{.DispName}}.size());
{{.Brank}}for (auto& {{.Type}} : {{.DispName}}) {
{{else if eq .Type "struct"}}putStruct({{.DispName}});
{{else if .Container}}putVector<{{.Type}}>({{.DispName}});
{{else if lt (len .Type) 0}}putBuffer<{{.Type}}, {{.SizeType}}>({{.DispName}}, {{.SizeName}});
{{else}}put<{{.Type}}>({{.DispName}});
{{end -}}
{{end -}}
}
//
void unpack{{.Struct.Name}}(Serializer& ser, {{.Struct.Name}}& s) {
}
} // namespace {{.NameSpace}}
{{end}}
