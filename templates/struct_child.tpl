{{- define "struct_base"}}
//
{{if .IsClass}}class{{else}}struct{{end}} {{.Name}} {
{{- if .IsClass}}
public:{{end -}}
{{- with .EnumList}}
{{- range .}}
  enum class {{.Name}} : uint8_t {
{{range .List}}    {{.}},{{end}}
  };
{{- end}}
{{end}}
{{- with .ChildStruct}}
  // child class
{{- range .}}
{{template "struct_base" .}}
{{- end}}
{{end}}
{{- if .IsClass}}
protected:{{end -}}
{{- with .BitField}}
  struct BitField {
{{- range .}}
    {{if .IsBool}}unsigned {{.Name}} : 1;
{{- else}}{{if .IsSigned}}signed  {{else}}unsigned{{end}} {{.Name}} : {{.Bits}};{{end}}
{{- end}}
  };
  BitField bit_field;
{{- end}}
  // members
{{- range .Members}}
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
{{- if .IsClass}}
public:{{end -}}
{{- if .ReserveList}}
  // constructor
  {{.Name}}() {
{{- range .ReserveList}}
    {{.Name}}.resize({{.Size}});{{end}}
  }{{end}}
{{- if .Serializer}}
  //
  void serialize({{.Serializer}}& ser);
  void deserialize({{.Serializer}}& ser);{{end}}
{{- if .SJson}}
  //
  void serializeJSON({{.SJson}}& json);
  void deserializeJSON({{.SJson}}& json);{{end}}
  // interface
{{- range .BitField}}
{{- if .IsBool}}
  //
  bool get{{.CapName}}() const { return bit_field.{{.Name}}; }
  void set{{.CapName}}(bool f) { bit_field.{{.Name}} = f; }
{{- else if .Cast}}
  //
  {{.Cast}} get{{.CapName}}() const { return static_cast<{{.Cast}}>(bit_field.{{.Name}}); }
  void set{{.CapName}}({{.Cast}} n) { bit_field.{{.Name}} = static_cast<unsigned>(n); }
{{- else if .IsSigned}}
  //
  signed get{{.CapName}}() const { return bit_field.{{.Name}} * {{.Scale}} + {{.Offset}}; }
  void set{{.CapName}}(signed n) { bit_field.{{.Name}} = (n - {{.Offset}}) / {{.Scale}}; }
{{- else}}
  //
  unsigned get{{.CapName}}() const { return bit_field.{{.Name}} * {{.Scale}} + {{.Offset}}; }
  void set{{.CapName}}(unsigned n) { bit_field.{{.Name}} = (n - {{.Offset}}) / {{.Scale}}; }{{end}}
{{- end}}
{{- range .Members}}
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
{{- end}}
