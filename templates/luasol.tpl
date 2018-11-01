//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
#include <sol/sol.hpp>
{{if .HeaderNameJ}}{{if .HeaderGlobalJ}}
#include <{{.HeaderNameJ}}>
{{else}}
#include "{{.HeaderNameJ}}"
{{end}}{{end}}

{{if .NameSpace}}namespace {{.NameSpace}} {
{{- end}}
{{$sn := .TopStruct.Name}}
void {{$sn}}::setLUA(sol::state& lua)
{
  lua.new_usertype<{{$sn}}>(
    "{{$sn}}",
    sol::constructors<>()
{{- with .TopStruct.BitField}}{{range .}},
    "{{.Name}}", sol::property(&{{$sn}}::get{{.CapName}}, &{{$sn}}::set{{.CapName}})
{{- end}}{{end}}
{{- with .TopStruct.Members}}{{range .}}{{if not .HasChild}},
    "{{.Name}}", &{{$sn}}::{{.Name}}
{{- end}}{{end}}{{end}});
{{- with .TopStruct.EnumList}}{{range .}}{{$tn := printf "t_%s" .Name}}{{$en := .Name}}
  sol::table {{$tn}} = lua.create_table_with();
{{- range .List}}
  {{$tn}}["{{.}}"] = (int){{$en}}::{{.}};{{end}}
  lua["{{$sn}}"]["{{.Name}}"] = {{$tn}};
{{- end}}{{end}}
}

{{if .NameSpace}}} // namespace {{.NameSpace}}{{end}}
