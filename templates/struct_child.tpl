{{- define "struct_base"}}
//
{{if .IsClass}}class{{else}}struct{{end}} {{.Name}} {
{{- if .IsClass}}
public:{{end -}}
{{- with .EnumList}}
{{- range .}}
  enum class {{.Name}} : uint8_t {
{{- range .List}}
    {{.}},{{end}}
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
{{- if .Container }}
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
{{- if or .ReserveList .InitialList}}
  // constructor
  {{.Name}}() {
{{- range .ReserveList}}
    {{.Name}}.resize({{.Size}});{{end}}
{{- range .InitialList}}
    set{{.CapName}}({{.Value}});{{end}}
  }{{end}}
{{- if getFlag "Compare"}}
  //
  bool operator == (const {{.Name}}& other) const {
{{- range .BitField}}
    if (bit_field.{{.Name}} != other.bit_field.{{.Name}}) return false;{{end}}
{{- range .Members}}
{{- if .Container}}
  {{- if not .IsStatic}}
    if ({{.Name}}.size() != other.{{.Name}}.size()) return false;{{end}}
    for (size_t i = 0; i < {{.Name}}.size(); i++)
    {
      if ({{.Name}}[i] != other.{{.Name}}[i]) return false;
    }
{{- else}}
    if ({{.Name}} != other.{{.Name}}) return false;{{end}}{{end}}
    return true;
  }
  bool operator != (const {{.Name}}& other) const {
    return !(*this == other);
  }{{end}}
{{- if getFlag "Copy"}}
  //
  void copyFrom(const {{.Name}}& other) {
{{- range .BitField}}
    bit_field.{{.Name}} = other.bit_field.{{.Name}};{{end}}
{{- range .Members}}
    {{.Name}} = other.{{.Name}};{{end}}
  }
  {{.Name}}& operator=(const {{.Name}}& other) {
    copyFrom(other);
    return *this;
  }{{end}}
{{- if .Serializer}}
  //
  void serialize({{.Serializer}}& ser);
  void deserialize({{.Serializer}}& ser);
  size_t getSerializeSize() const;{{end}}
{{- if .SJson}}
  //
  void serializeJSON({{.SJson}}& json);
  void deserializeJSON({{.SJson}}& json);{{end}}
{{- if .UseLua}}
  //
  static void setLUA(sol::state& lua);{{end}}
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
  const {{.Type}}{{.Ref}}{{.GetRef}} get{{.CapName}}() const { return {{.Name}}; }
  void set{{.CapName}}({{.Type}}{{.Ref}} n) { {{.Name}} = n; }
{{- end}}
{{- end}}
};
{{- end}}
