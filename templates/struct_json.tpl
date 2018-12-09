//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//

{{if .HeaderNameJ}}{{if .HeaderGlobalJ}}
#include <{{.HeaderNameJ}}>
{{else}}
#include "{{.HeaderNameJ}}"
{{end}}{{end}}

#include <map>

using json = {{.TopStruct.SJson}};

{{if .NameSpace}}namespace {{.NameSpace}} {
{{- end}}
{{$StructName := .TopStruct.Name}}
namespace {
{{- with .TopStruct.EnumList}}{{range .}}
//{{$EnumName := print $StructName "::" .Name}}
const char* enum_{{.Name}}_list[] = {
   {{range .List}} "{{.}}",{{end}}
};
const std::map<std::string, {{$EnumName}}> enum_{{.Name}}_map = {
{{- range .List}}
    { "{{.}}", {{$EnumName}}::{{.}} },{{end}}
};
{{end}}{{end -}}
} // namespace

//
{{- with .TopStruct.EnumList}}{{range .}}
const char*
{{$StructName}}::getString{{.Name}}({{.Name}} n)
{
    return enum_{{.Name}}_list[(int)n];
}
{{- end}}{{end}}

//
void {{$StructName}}::serializeJSON(json& j) {
    json jsonObject;
{{pushObj "jsonObject"}}{{template "json_child_out" .TopStruct}}{{popObj}}
    //
    j["{{$StructName}}"] = jsonObject;
}
//
void {{$StructName}}::deserializeJSON(json& j) {
    json jsonReader = j["{{$StructName}}"];
{{pushObj "jsonReader"}}{{template "json_child_in" .TopStruct}}{{popObj}}
}

{{if .NameSpace}}} // namespace {{.NameSpace}}{{end}}
