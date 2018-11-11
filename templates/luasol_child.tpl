{{- define "luasol_child"}}{{$sn := .Name}}
{{- with .ChildStruct}}{{pushStr $sn}}{{range .}}{{template "luasol_child" .}}{{end}}{{popStr}}{{end}}
  lua.new_usertype<{{$sn}}>(
    "{{getStr}}{{$sn}}"
{{- with .BitField}}{{range .}},
    "{{.Name}}", sol::property(&{{$sn}}::get{{.CapName}}, &{{$sn}}::set{{.CapName}})
{{- end}}{{end}}
{{- with .Members}}{{range .}},
    "{{.Name}}", &{{$sn}}::{{.Name}}
{{- end}}{{end}}
{{- if getFlag "Copy"}},
    "copy", &{{$sn}}::copyFrom
{{- end}});
{{- with .EnumList}}{{range .}}{{$tn := printf "t_%s" .Name}}{{$en := .Name}}
  sol::table {{$tn}} = lua.create_table_with();
{{- range .List}}
  {{$tn}}["{{.}}"] = (int){{$en}}::{{.}};{{end}}
  lua["{{getStr}}{{$sn}}"]["{{.Name}}"] = {{$tn}};
{{- end}}{{end}}
{{- end}}
