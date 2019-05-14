{{- define "luasol_child"}}{{$sn := .Name}}
{{- with .ChildStruct}}{{pushStr $sn}}{{range .}}{{template "luasol_child" .}}{{end}}{{popStr}}{{end}}
  {{- $tempuname := getStr}}{{$uname := printf "%s%s" $tempuname $sn}}{{$bname := getStrSep "::" $sn}}
  {{- if ne $uname $bname}}
  using {{$uname}} = {{$bname}};
  {{- end}}
  lua.new_usertype<{{$uname}}>(
    "{{$uname}}"
{{- with .BitField}}{{range .}},
    "{{.Name}}", sol::property(&{{$uname}}::get{{.CapName}}, &{{$uname}}::set{{.CapName}})
{{- end}}{{end}}
{{- with .Members}}{{range .}},
    "{{.Name}}", &{{$uname}}::{{.Name}}
{{- end}}{{end}}
{{- if getFlag "Copy"}},
    "copyFrom", &{{$uname}}::copyFrom
{{- end}});
{{- with .EnumList}}{{pushStr $sn}}{{range .}}{{$tn := printf "t_%s" .Name}}{{$en := getStrSep "::" .Name}}
  sol::table {{$tn}} = lua.create_table_with();
{{- range .List}}
  {{$tn}}["{{.}}"] = (int){{$en}}::{{.}};{{end}}
  lua["{{getStr}}"]["{{.Name}}"] = {{$tn}};
{{- end}}{{popStr}}{{end}}
{{- end}}
