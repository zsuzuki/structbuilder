{{- define "json_child_in"}}
{{- with .BitField}}{{range .}}
    if (!{{getObj}}{{getStr}}["{{.Name}}"].is_null())
    {{- if .Cast}}
    {{myName}}bit_field.{{.Name}} = static_cast<unsigned>
        (enum_{{.CapName}}_map.at({{getObj}}{{getStr}}["{{.Name}}"].get<std::string>()));
    {{- else}}
        {{myName}}bit_field.{{.Name}} = {{getObj}}{{getStr}}["{{.Name}}"];
    {{- end}}
{{- end}}{{end}}
{{- with .Members}}{{range .}}
    if (!{{getObj}}{{getStr}}["{{.Name}}"].is_null()) {
{{- if .Container}}{{$jn := printf "j%s" .CapName}}
    json {{$jn}} = {{getObj}}{{getStr}}["{{.Name}}"];
{{- if .IsStatic}}
    int {{$jn}}Index = 0;
{{- else}}
    {{.Name}}.reserve({{$jn}}.size());
    {{.Name}}.resize(0);
{{- end}}
    for (auto& {{$jn}}It : {{$jn}}) {
{{- if .IsStatic}}
      if ({{$jn}}Index < {{.Size}}) {
{{- end}}
{{- if .HasChild}}
{{- if .IsStatic}}
        auto& tObj = {{.Name}}[{{$jn}}Index];
{{- else -}}
        {{.Type}} tObj{};
{{- end -}}
        {{$jnt := printf "%sIt" $jn}}{{pushObj $jnt}}
{{- setMyName "tObj"}}{{template "json_child_in" .Child}}{{clearMyName}}
        {{popObj}}
{{- if not .IsStatic}}{{.Name}}.emplace_back(tObj);{{end}}
{{- else}}
        {{.Name}}.push_back({{$jn}}It);{{end}}
{{- if .IsStatic}}{{$jn}}Index++;}{{end}}
    }
{{- else if .HasChild -}}
{{- setMyName .Name}}{{pushStr "[\"" .Name "\"]" -}}{{template "json_child_in" .Child}}{{popStr}}{{clearMyName}}
{{- else}}
    {{myName}}{{.Name}} = {{getObj}}{{getStr}}["{{.Name}}"];
{{- end}}
    }
{{- end}}{{end -}}
{{- end}}
