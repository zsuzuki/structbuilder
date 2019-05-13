{{- define "luasol_child"}}{{$sn := .Name}}
{{- with .ChildStruct}}{{pushStr $sn}}{{range .}}{{template "luasol_child" .}}{{end}}{{popStr}}{{end}}
  lua.new_usertype<{{getStrSep "::" $sn}}>(
    "{{getStr}}{{$sn}}"
{{- with .BitField}}{{range .}},
    "{{.Name}}", sol::property(&{{getStrSep "::" $sn}}::get{{.CapName}}, &{{getStrSep "::" $sn}}::set{{.CapName}})
{{- end}}{{end}}
{{- with .Members}}{{range .}},
    "{{.Name}}", &{{getStrSep "::" $sn}}::{{.Name}}
{{- end}}{{end}}
{{- if getFlag "Copy"}},
    "copyFrom", &{{getStrSep "::" $sn}}::copyFrom
{{- end}});
{{- with .EnumList}}{{pushStr $sn}}{{range .}}{{$tn := printf "t_%s" .Name}}{{$en := getStrSep "::" .Name}}
  sol::table {{$tn}} = lua.create_table_with();
{{- range .List}}
  {{$tn}}["{{.}}"] = (int){{$en}}::{{.}};{{end}}
  lua["{{getStr}}"]["{{.Name}}"] = {{$tn}};
{{- end}}{{popStr}}{{end}}
{{- end}}
