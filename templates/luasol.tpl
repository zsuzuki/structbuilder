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
void {{.TopStruct.Name}}::setLUA(sol::state& lua)
{
{{- setFlag "Compare" .Compare}}{{setFlag "Copy" .Copy}}
{{- template "luasol_child" .TopStruct}}
}

{{if .NameSpace}}} // namespace {{.NameSpace}}{{end}}
