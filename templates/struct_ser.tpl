//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
{{if .HeaderNameB}}{{if .HeaderGlobalB}}
#include <{{.HeaderNameB}}>
{{else}}
#include "{{.HeaderNameB}}"
{{end}}{{end}}
{{if .NameSpace}}namespace {{.NameSpace}} { {{- end}}
namespace {
} // namespace
{{$StructName := .TopStruct.Name}}
//
void {{$StructName}}::serialize({{.TopStruct.Serializer}}& ser) {
}
//
void {{$StructName}}::deserialize({{.TopStruct.Serializer}}& ser) {
}
{{if .NameSpace}}} // namespace {{.NameSpace}}{{end}}
