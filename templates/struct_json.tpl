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
{{template "json_enum" .TopStruct}}
{{$StructName := .TopStruct.Name}}

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
