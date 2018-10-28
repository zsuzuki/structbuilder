{{define "struct_base" -}}
//
class {{.Name}} {
public:
{{- if .ChildStruct}}
  // child class
  {{- range .ChildStruct}}
  {{template "struct_base" .}}
  {{- end}}
{{end}}
{{- if .BitField -}}
  struct BitField {
{{- range .BitField}}
    {{if .IsBool}}unsigned {{.Name}} : 1;
{{- else}}{{if .IsSigned}}signed  {{else}}unsigned{{end}} {{.Name}} : {{.Bits}};{{end}}
{{- end}}
  };
{{end}}
protected:
  // members
{{if .BitField -}}
  BitField bit_field;
{{end}}
{{- range .Members}}
{{- if .Container}}
  {{.Container}} {{.Name}};
{{- else}}
  {{.Type}} {{.Name}};
{{- end}}
{{- end}}
public:
  // interface
{{- range .BitField -}}
  {{if .IsBool}}bool get{{.CapName}}() const { return bit_field.{{.Name}}; }
  void set{{.CapName}}(bool f) { bit_field.{{.Name}} = f; }
{{- else}}
  {{if .IsSigned}}signed get{{.CapName}}() const { return bit_field.{{.Name}}; }
  void set{{.CapName}}(signed n) { bit_field.{{.Name}} = n; }
  {{else}}unsigned get{{.CapName}}() const { return bit_field.{{.Name}}; }
  void set{{.CapName}}(unsigned n) { bit_field.{{.Name}} = n; }{{end}}
{{- end}}
{{- end}}
{{- range .Members}}
{{- if .Container}}
  const {{.Type}}{{.Ref}} get{{.CapName}}(int idx) const { return {{.Name}}[idx]; }
  void set{{.CapName}}(int idx, {{.Type}}{{.Ref}} n) { {{.Name}}[idx] = n; }
{{- else}}
  const {{.Type}}{{.Ref}} get{{.CapName}}() const { return {{.Name}}; }
  void set{{.CapName}}({{.Type}}{{.Ref}} n) { {{.Name}} = n; }
{{- end}}
{{- end}}
};
{{end}}
