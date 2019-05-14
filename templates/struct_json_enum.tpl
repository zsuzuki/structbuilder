{{- define "json_enum"}}{{$StructName := getStrSep "::" .Name}}{{pushStr .Name}}
//
namespace {
{{- with .EnumList}}
{{- range .}}{{$EnumName := getStrSep "::" .Name}}
const char* enum_{{.Name}}_list[] = {
   {{range .List}} "{{.}}",{{end}}
};
const std::map<std::string, {{$EnumName}}> enum_{{.Name}}_map = {
{{- range .List}}
    { "{{.}}", {{$EnumName}}::{{.}} },{{end}}
};
{{- end}}{{end}}
} // namespace

//
{{- with .EnumList}}{{range .}}{{$EnumName := getStrSep "::" .Name}}
const char*
{{$StructName}}::getStringFrom{{.Name}}({{$EnumName}} n)
{
    return enum_{{.Name}}_list[static_cast<int>(n)];
}
{{$EnumName}}
{{$StructName}}::getEnumFrom{{.Name}}(const std::string s)
{
    return enum_{{.Name}}_map.at(s);
}
{{- end}}{{end}}
{{- range .ChildStruct}}{{template "json_enum" .}}{{end -}}
{{popStr}}{{- end}}
