{{- define "serialize_size"}}
{{- if .BitField}}    r += sizeof({{myName}}bit_field);{{end}}
{{- with .Members}}{{range .}}
{{- if .Container}}
{{- if .HasChild}}
    r += sizeof(uint16_t);{{$cn := printf "t%s" .Type}}
    for (auto& {{$cn}} : {{myName}}{{.Name}}) {
{{- setMyName $cn}}{{template "serialize_size" .Child}}{{clearMyName}}
    }
{{- else}}
    r += sizeof(uint16_t) + sizeof({{.Type}}) * {{myName}}{{.Name}}.size();
{{- end}}
{{- else if .HasChild}}
{{setMyName .Name}}{{- template "serialize_size" .Child}}{{clearMyName}}
{{- else if eq .Type "std::string"}}
    r += sizeof(uint16_t) + sizeof(char) * {{myName}}{{.Name}}.size();
{{- else}}
    r += sizeof({{myName}}{{.Name}});
{{- end}}
{{- end}}{{end}}
{{- end}}
