//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
#pragma once
{{if gt (len .LocalInclude) 0}}{{range .LocalInclude}}#include "{{.}}"
{{end -}}
{{end}}
{{if gt (len .Include) 0}}{{range .Include}}#include <{{.}}>
{{end -}}
{{end}}
{{if .NameSpace}}namespace {{.NameSpace}} {
{{- end}}

class {{.TopStruct.Name}} {
public:
  // child class
{{- range .TopStruct.ChildStruct}}
  struct {{.Name}} {
{{- range .Members}}
    {{.Type}} {{.Name}};
{{- end}}
  };
{{- end}}
  struct BitField {
{{- range .TopStruct.BitField}}
    {{if .IsBool}}unsigned {{.Name}} : 1;
{{- else}}{{if .IsSigned}}signed  {{else}}unsigned{{end}} {{.Name}} : {{.Bits}};{{end}}
{{- end}}
  };

protected:
  // members
  BitField bit_field;
{{- range .TopStruct.Members}}
{{- if .Container}}
  {{.Container}} {{.Name}};
{{- else}}
  {{.Type}} {{.Name}};
{{- end}}
{{- end}}

public:
  // interface
{{- range .TopStruct.BitField -}}
  {{if .IsBool}}bool get{{.CapName}}() const { return bit_field.{{.Name}}; }
  void set{{.CapName}}(bool f) { bit_field.{{.Name}} = f; }
{{- else}}
  {{if .IsSigned}}signed get{{.CapName}}() const { return bit_field.{{.Name}}; }
  void set{{.CapName}}(signed n) { bit_field.{{.Name}} = n; }
  {{else}}unsigned get{{.CapName}}() const { return bit_field.{{.Name}}; }
  void set{{.CapName}}(unsigned n) { bit_field.{{.Name}} = n; }{{end}}
{{- end}}
{{- end}}
{{- range .TopStruct.Members}}
{{- if .Container}}
  const {{.Type}}{{.Ref}} get{{.CapName}}(int idx) const { return {{.Name}}[idx]; }
  void set{{.CapName}}(int idx, {{.Type}}{{.Ref}} n) { {{.Name}}[idx] = n; }
{{- else}}
  const {{.Type}}{{.Ref}} get{{.CapName}}() const { return {{.Name}}; }
  void set{{.CapName}}({{.Type}}{{.Ref}} n) { {{.Name}} = n; }
{{- end}}
{{- end}}
};
{{if .NameSpace}}} // namespace {{.NameSpace}}{{end}}
