{{- define "json_child_out"}}
{{- with .BitField}}{{range .}}{{if .Cast}}
    {{getObj}}{{getStr}}["{{.Name}}"] = enum_{{.Cast}}_list[(int){{myName}}bit_field.{{.Name}}];
    {{- else}}
    {{getObj}}{{getStr}}["{{.Name}}"] = ({{if .IsSigned}}signed{{else}}unsigned{{end}}){{myName}}bit_field.{{.Name}};
    {{- end}}
{{- end}}{{end}}
{{- with .Members}}{{range .}}{{$vn := printf "t%s" .CapName}}
{{- if .Container}}
    for (auto& {{$vn}} : {{.Name}}) {
{{- if .HasChild}}
        {{$on := printf "    j%s" .CapName}}json {{$on}};{{pushObj $on}}
{{- setMyName $vn}}{{template "json_child_out" .Child}}{{clearMyName}}
        {{popObj}}{{getObj}}{{getStr}}["{{.Name}}"].push_back(j{{.CapName}});
{{- else}}
        {{getObj}}{{getStr}}["{{.Name}}"].push_back(t{{.CapName}});{{end}}
    }
{{- else if .HasChild -}}
{{setMyName .Name}}{{pushStr "[\"" .Name "\"]" -}}{{template "json_child_out" .Child}}{{popStr}}{{clearMyName}}
{{- else}}
    {{getObj}}{{getStr}}["{{.Name}}"] = {{myName}}{{.Name}};
{{- end}}
{{- end}}{{end -}}
{{- end}}
