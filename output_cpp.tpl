//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
{{if gt (len .LocalInclude) 0}}{{range .LocalInclude}}#include "{{.}}"
{{end -}}
{{end}}
{{if gt (len .Include) 0}}{{range .Include}}#include <{{.}}>
{{end -}}
{{end}}
{{- if .HasNameSpace}}namespace {{.NameSpace}} {
{{- end}}
//
size_t get{{.Struct.Name}}PackSize(const {{.Struct.Name}}& s) {
    size_t r = sizeof(uint16_t);
{{range .Struct.DispMembers -}}
{{.Brank}}{{if .BracketClose}}}
{{else if len .Children}}r += sizeof({{.SizeType}});
{{.Brank}}for (auto& {{.Type}} : {{.DispName}}) {
{{else if eq .Type "struct"}}r += sizeof(uint16_t) + sizeof({{.DispName}});
{{else if .Container}}r += sizeof(size_t) + sizeof({{.Type}}) * {{.DispName}}.size();
{{else if len .SizeType}}r += sizeof({{.SizeType}}) + sizeof({{.Type}}) * {{.SizeName}};
{{else}}r += sizeof({{.Type}});
{{end -}}
{{end -}}
{{.Struct.Brank}}return r;
}
//
void pack{{.Struct.Name}}(Serializer& ser, {{.Struct.Name}}& s) {
{{.Struct.Brank}}ser.putVersion({{.Struct.Version}});
{{range .Struct.DispMembers -}}
{{.Brank}}{{if .BracketClose}}}
{{else if len .Children}}ser.put<{{.SizeType}}>({{.DispName}}.size());
{{.Brank}}for (auto& {{.Type}} : {{.DispName}}) {
{{else if eq .Type "struct"}}ser.putStruct({{.DispName}});
{{else if .Container}}ser.putVector<{{.Type}}>({{.DispName}});
{{else if len .SizeType}}ser.putBuffer<{{.Type}}, {{.SizeType}}>({{.DispName}}, {{.SizeName}});
{{else}}ser.put<{{.Type}}>({{.DispName}});
{{end -}}
{{end -}}
}
//
void unpack{{.Struct.Name}}(Serializer& ser, {{.Struct.Name}}& s) {
{{.Struct.Brank}}auto version = ser.getVersion("{{.Struct.Name}}", {{.Struct.Unsupport}});
{{range .Struct.DispMembers -}}
{{.Brank}}{{if .BracketClose}}}
{{else if len .Children}}auto {{.Type}}_size = ser.get<{{.SizeType}}>();
{{.Brank}}{{.DispName}}.resize({{.Type}}_size);
{{.Brank}}for (auto& {{.Type}} : {{.DispName}}) {
{{else if eq .Type "struct"}}ser.getStruct({{.DispName}});
{{else if .Container}}ser.getVector<{{.Type}}>({{.DispName}});
{{else if len .SizeType}}{
{{.Brank}} auto r = ser.getBuffer<{{.Type}}, {{.SizeType}}>();
{{- if .RawAccess}}
{{.Brank}} auto sz = (r.second < {{.SizeName}}) ? r.second : {{.SizeName}};
{{.Brank}} memcpy({{.DispName}}, r.first, sz);
{{else}}
{{.Brank}} {{.DispNameSet}}(r.first, r.second);
{{end}}{{.Brank}}}
{{else}}{{if .RawAccess}}{{.DispName}} = ser.get<{{.Type}}>(){{else}}{{.DispNameSet}}(ser.get<{{.Type}}>()){{end}};
{{end -}}
{{end -}}
}
{{if .HasNameSpace}}} // namespace {{.NameSpace}}{{end}}
