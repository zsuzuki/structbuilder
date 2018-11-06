{{- define "serialize"}}
{{- if .BitField}}    ser.putStruct({{myName}}bit_field);{{end}}
{{- with .Members}}{{range .}}
{{- if .Container}}
{{- if .HasChild}}
    ser.put<uint16_t>({{myName}}{{.Name}}.size());
{{- $mn := printf "t%s" .Type}}
    for (auto& {{$mn}} : {{.Name}}) {
{{- setMyName $mn}}{{template "serialize" .Child}}{{clearMyName}}
    }
{{- else}}
    ser.putVector<{{.Type}}>({{myName}}{{.Name}});
{{- end}}
{{- else}}
{{- if .HasChild}}
{{setMyName .Name}}{{template "serialize" .Child}}{{clearMyName}}
{{- else if eq .Type "std::string"}}
    ser.put({{myName}}{{.Name}});
{{- else}}
    ser.put<{{.Type}}>({{myName}}{{.Name}});
{{- end}}
{{- end}}
{{- end}}{{end}}{{- end}}
