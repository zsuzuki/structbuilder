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
size_t {{$StructName}}::getSerializeSize() const {
    size_t r = sizeof({{.TopStruct.Serializer}}::version_t);
{{template "serialize_size" .TopStruct}}
    return r;
}
//
void {{$StructName}}::serialize({{.TopStruct.Serializer}}& ser) {
    ser.putVersion({{.BinVersion}});
{{template "serialize" .TopStruct}}
}
//
void {{$StructName}}::deserialize({{.TopStruct.Serializer}}& ser) {
    auto version = ser.getVersion("{{$StructName}}", {{.BinVersion}});
    (void)version;
{{template "deserialize" .TopStruct}}
}
{{if .NameSpace}}} // namespace {{.NameSpace}}{{end}}
