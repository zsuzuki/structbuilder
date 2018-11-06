{{- define "deserialize"}}
{{- if .BitField}}    ser.getStruct({{myName}}bit_field);{{end}}
{{- with .Members}}{{range .}}
{{- if .Container}}
{{- if .HasChild}}
{{- $mn := printf "t%s" .Type}}{{$mnc := printf "c%s" .Type}}
    {{- if .IsStatic}}
    auto {{$mn}}Size = ser.get<uint16_t>();
    for (size_t {{$mnc}} = 0; {{$mnc}} < {{$mn}}Size; {{$mnc}}++) {
        {{.Type}} {{$mn}}{};
    {{- else}}
    {{myName}}{{.Name}}.resize(ser.get<uint16_t>());
    for (auto& {{$mn}} : {{myName}}{{.Name}}) {
    {{- end}}
{{- setMyName $mn}}{{template "deserialize" .Child}}{{clearMyName}}
    {{- if .IsStatic}}
    if ({{$mnc}} < {{.Size}})
        {{myName}}{{.Name}}[{{$mnc}}] = {{$mn}};
    {{- end}}
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
