{{- define "deserialize"}}
{{- if .BitField}}    ser.getStruct({{myName}}bit_field);{{end}}
{{- with .Members}}{{range .}}
{{- if .Container}}
{{- if .HasChild}}
    {{myName}}{{.Name}}.resize(ser.get<uint16_t>());
{{- $mn := printf "t%s" .Type}}
    for (auto& {{$mn}} : {{.Name}}) {
{{- setMyName $mn}}{{template "deserialize" .Child}}{{clearMyName}}
    }
{{- else}}
    ser.getVector<{{.Type}}>({{myName}}{{.Name}});
{{- end}}
{{- else}}
{{- if .HasChild}}
{{setMyName .Name}}{{template "deserialize" .Child}}{{clearMyName}}
{{- else}}
    {{myName}}{{.Name}} = ser.get<{{.Type}}>();
{{- end}}
{{- end}}
{{- end}}{{end}}
{{- end}}
