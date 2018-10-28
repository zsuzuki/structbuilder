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

//
class {{.TopStruct.Name}} {
public:
{{- with .TopStruct.ChildStruct}}
  // child class
{{- range .}}
{{template "struct_base" .}}
{{- end}}
{{- end}}
protected:
{{- with .TopStruct.BitField}}
  struct BitField {
{{- range .}}
    {{if .IsBool}}unsigned {{.Name}} : 1;
{{- else}}{{if .IsSigned}}signed  {{else}}unsigned{{end}} {{.Name}} : {{.Bits}};{{end}}
{{- end}}
  };
  BitField bit_field;
{{end}}
  // members
{{- range .TopStruct.Members}}
{{- if .Container}}
  {{- if .IsStatic}}
  {{.Container}}<{{.Type}}, {{.Size}}> {{.Name}};
  {{- else}}
  {{.Container}}<{{.Type}}> {{.Name}};
  {{- end}}
{{- else}}
  {{.Type}} {{.Name}};
{{- end}}
{{- end}}
public:
  // constructor
  {{.TopStruct.Name}}() {
{{- with .TopStruct.ReserveList}}
{{- range .}}
    {{.Name}}.resize({{.Size}});{{end}}
{{- end}}
  }

  // interface
{{- range .TopStruct.BitField}}
{{- if .IsBool}}
  //
  bool get{{.CapName}}() const { return bit_field.{{.Name}}; }
  void set{{.CapName}}(bool f) { bit_field.{{.Name}} = f; }
{{- else}}
{{- if .IsSigned}}
  //
  signed get{{.CapName}}() const { return bit_field.{{.Name}} * {{.Scale}} + {{.Offset}}; }
  void set{{.CapName}}(signed n) { bit_field.{{.Name}} = (n - {{.Offset}}) / {{.Scale}}; }
{{- else}}
  //
  unsigned get{{.CapName}}() const { return bit_field.{{.Name}} * {{.Scale}} + {{.Offset}}; }
  void set{{.CapName}}(unsigned n) { bit_field.{{.Name}} = (n - {{.Offset}}) / {{.Scale}}; }{{end}}
{{- end}}
{{- end}}
{{- range .TopStruct.Members}}
{{- if .Container}}
  //
  const {{.Type}}{{.Ref}} get{{.CapName}}(int idx) const { return {{.Name}}[idx]; }
  void set{{.CapName}}(int idx, {{.Type}}{{.Ref}} n) { {{.Name}}[idx] = n; }
  size_t get{{.CapName}}Size() const { return {{.Name}}.size(); }
  {{- if not .IsStatic}}
  void append{{.CapName}}({{.Type}}{{.Ref}} n) { {{.Name}}.emplace_back(n); }
  void resize{{.CapName}}(size_t sz) { {{.Name}}.resize(sz); }
  {{- end}}
{{- else}}
  //
  const {{.Type}}{{.Ref}} get{{.CapName}}() const { return {{.Name}}; }
  void set{{.CapName}}({{.Type}}{{.Ref}} n) { {{.Name}} = n; }
{{- end}}
{{- end}}
};
{{if .NameSpace}}} // namespace {{.NameSpace}}{{end}}
