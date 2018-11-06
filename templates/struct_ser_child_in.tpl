{{- define "deserialize"}}
{{- if .BitField}}    ser.getStruct({{myName}}bit_field);{{end}}
{{- with .Members}}{{range .}}
{{- if .Container}}
{{- if .HasChild}}
    {{- if .IsStatic}}
    {{- else}}
    {{myName}}{{.Name}}.resize(ser.get<uint16_t>());
    {{- end}}
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
{{- else if eq .Type "std::string"}}
    ser.get({{myName}}{{.Name}});
{{- else}}
    {{myName}}{{.Name}} = ser.get<{{.Type}}>();
{{- end}}
{{- end}}
{{- end}}{{end}}
{{- end}}
